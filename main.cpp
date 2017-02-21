#include "mbed.h"
#include "mico.h"

DigitalOut led1(MBED_SYS_LED);
Serial pc(STDIO_UART_TX, STDIO_UART_RX, 115200);

// main() runs in its own thread in the OS
// (note the calls to wait below for delays)
int main() {
	int i = 1;
	pc.printf("Hello World !\r\n");
	mico_gpio_initialize( MICO_RF_LED, OUTPUT_PUSH_PULL );

    while (true) {
        led1 = !led1;
        mico_gpio_output_trigger( MICO_RF_LED );
        pc.printf("This program runs since %d seconds.\r\n", i++);
        wait(1);
    }
}

