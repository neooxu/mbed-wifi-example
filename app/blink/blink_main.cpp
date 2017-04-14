#include "mbed.h"
#include "mico.h"

#define TARGET_AZ3166

#ifdef TARGET_AZ3166

#include "oled.h"
#include "lps25hb.h"

Serial pc(STDIO_UART_TX, STDIO_UART_RX, 115200);

#define app_log(format, ...)  custom_log("MiCOKit_STmems", format, ##__VA_ARGS__)

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

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

int app_blink( )
{
    OSStatus err = kNoErr;
    float lps25hb_temp_data = 0;
    float lps25hb_pres_data = 0;
    char oled_show_line[OLED_DISPLAY_MAX_CHAR_PER_ROW+1] = {'\0'};   // max char each line

    pc.printf( "helloworld!\r\n" );

    // init OLED
    OLED_Init();
    OLED_Clear();
    snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "%s", MODEL);
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_1, oled_show_line);
    memset(oled_show_line, '\0', OLED_DISPLAY_MAX_CHAR_PER_ROW+1);
    snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "%s", "MiCO            ");
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_2,  oled_show_line);
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3,  "   Running...   ");
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4,  "                ");

    /*init LPS25HB */
    err = lps25hb_sensor_init();
    require_noerr_string( err, exit, "ERROR: Unable to Init LPS25HB" );

    while ( true ) {
        led1 = !led1;
        led2 = !led2;
        led3 = !led3;

        err = lps25hb_Read_Data( &lps25hb_temp_data, &lps25hb_pres_data );
        require_noerr_string( err, exit, "ERROR: Can't Read LPS25HB Data" );
        sprintf( oled_show_line, "%.2fm", lps25hb_pres_data );
        OLED_ShowString( 0, OLED_DISPLAY_ROW_3, oled_show_line );
        app_log("Pressure: %.2fm", lps25hb_pres_data);

        Thread::wait( 500 );
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





