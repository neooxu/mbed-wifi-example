#include "mbed.h"
#include "mico.h"


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




