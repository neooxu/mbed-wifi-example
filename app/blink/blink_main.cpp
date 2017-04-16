#include "mbed.h"
#include "mico.h"
#include "EMW10xxInterface.h"

#define TARGET_AZ3166

#ifdef TARGET_AZ3166

#include "oled.h"
#include "lps22hb.h"


typedef enum
{
  JOY_NONE = 0,
  JOY_CENTER = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOY_State_TypeDef;


Serial pc(STDIO_UART_TX, STDIO_UART_RX, 115200);

#define app_log(format, ...)  custom_log("MiCOKit_STmems", format, ##__VA_ARGS__)

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

PwmOut rgb_r(RGB_R);
PwmOut rgb_g(RGB_G);
PwmOut rgb_b(RGB_B);

IRDA_HandleTypeDef IrdaHandle;

class Counter {
public:
    Counter(PinName pin) : _interrupt(pin), _count(0) {        // create the InterruptIn on the pin specified to Counter
        _interrupt.rise(callback(this, &Counter::increment)); // attach increment function of this counter instance
    }

    void increment() {
        _count++;
        pc.printf("Current count is %d\r\n", _count);
    }

    int read() {
        return _count;
    }

private:
    InterruptIn _interrupt;
    volatile int _count;
};

Counter CountA(USER_BUTTON_A);
Counter CountB(USER_BUTTON_B);

void USART3_IRQHandler(void)
{
    HAL_IRDA_IRQHandler(&IrdaHandle);
}

void HAL_IRDA_RxCpltCallback(IRDA_HandleTypeDef *hirda)
{
    uint8_t data;
    data = HAL_IRDA_Receive( hirda, &data, 1, 0 );
    led3 = !led3;

exit:
    return;
}

static OSStatus irda_init()
{
    OSStatus err = kNoErr;

    pinmap_pinout(PB_10, PinMap_UART_TX);
    pin_mode(PB_10, PushPullNoPull);

    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_USART3_FORCE_RESET();
    __HAL_RCC_USART3_RELEASE_RESET();

    IrdaHandle.Instance        = USART3;

    IrdaHandle.Init.BaudRate   = 9600;
    IrdaHandle.Init.WordLength = UART_WORDLENGTH_8B;
    IrdaHandle.Init.Parity     = UART_PARITY_NONE;
    IrdaHandle.Init.Mode       = UART_MODE_TX_RX;
    IrdaHandle.Init.Prescaler  = 1;
    IrdaHandle.Init.IrDAMode   = IRDA_POWERMODE_NORMAL;

    /* Enable and set I2Sx Interrupt to a lower priority */
    HAL_NVIC_SetPriority( USART3_IRQn, 0x0F, 0x00 );
    NVIC_SetVector( USART3_IRQn, (uint32_t)&USART3_IRQHandler );
    HAL_NVIC_EnableIRQ( USART3_IRQn );

    err = HAL_IRDA_Init(&IrdaHandle);
    require_noerr( err, exit );
exit:
    return err;
}

int app_blink( )
{
    OSStatus err = kNoErr;

    uint8_t cur_rgb = 0;
    uint8_t joy_status = JOY_NONE;

    float lps22hb_temp_data = 0;
    float lps22hb_pres_data = 0;
    char oled_show_line[OLED_DISPLAY_MAX_CHAR_PER_ROW+1] = {'\0'};   // max char each line

    EMW10xxInterface wlan_blink;

    pc.printf( "helloworld!\r\n" );

    rgb_r.period( 0.001 );
    rgb_g.period( 0.001 );
    rgb_b.period( 0.001 );

    irda_init();

    // init OLED
    OLED_Init();
    OLED_Clear();
    snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "%s", MODEL);
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_1, oled_show_line);

    snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "%s", wlan_blink.get_mac_address());
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_2,  oled_show_line);

    wlan_blink.connect("William Xu", "mx099555", NSAPI_SECURITY_WPA_WPA2 );
    snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "%s", wlan_blink.get_ip_address());
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3,  oled_show_line);

    /*init LPS22HB */
    err = lps25hb_sensor_init();
    require_noerr_string( err, exit, "ERROR: Unable to Init LPS22HB" );

    while ( true ) {
        if( cur_rgb%3 == 0 )
            led1 = !led1;
        if( cur_rgb%3 == 1 )
            led2 = !led2;
        if( cur_rgb%3 == 2 )
            led3 = !led3;


        rgb_r = (cur_rgb %3 == 0)? 0.5:0;
        rgb_g = (cur_rgb %3 == 1)? 0.5:0;
        rgb_b = (cur_rgb %3 == 2)? 0.5:0;

        err = lps25hb_Read_Data( &lps22hb_temp_data, &lps22hb_pres_data );
        require_noerr_string( err, exit, "ERROR: Can't Read LPS22HB Data" );
        sprintf( oled_show_line, "%.2fm", lps22hb_pres_data );
        OLED_ShowString( 0, OLED_DISPLAY_ROW_4, oled_show_line );
        app_log("Pressure: %.2fm", lps22hb_pres_data);

        joy_status = cur_rgb%5 + 1;
        HAL_IRDA_Transmit(&IrdaHandle, &joy_status, 1, 100);

        Thread::wait( 500 );

        cur_rgb ++;
    }
exit:
    return 0;
}
#else

DigitalOut led1(MBED_SYS_LED);
Serial pc(STDIO_UART_TX, STDIO_UART_RX, 115200);


#define os_helloworld_log(format, ...)  custom_log("helloworld", format, ##__VA_ARGS__)

mico_semaphore_t sem = NULL;

void easylink_pressed_callback(void *arg)
{
    mico_rtos_set_semaphore( &sem );
    mico_gpio_output_trigger( MICO_RF_LED );
}

/* MiCO GPIO IO driver demo */
int app_blink( )
{
    mico_gpio_initialize( MICO_RF_LED, OUTPUT_PUSH_PULL );
    mico_gpio_initialize( EasyLink_BUTTON, INPUT_PULL_UP );

    mico_gpio_enable_irq( EasyLink_BUTTON, IRQ_TRIGGER_FALLING_EDGE, easylink_pressed_callback, NULL );

    mico_rtos_init_semaphore( &sem, 1 );

    pc.printf( "Helloworld\r\n" );

    while ( true ) {
        if ( kNoErr != mico_rtos_get_semaphore( &sem, 5000 ) ) {
            pc.printf( "Get semaphore timeout\r\n" );
        }
        else {
            pc.printf(" Get semaphore success\r\n");
            led1 = !led1;
        }
    }
}
#endif





