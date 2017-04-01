#include "mbed.h"
#include "mico.h"

#include "TCPSocket.h"
#include "EMW10xxInterface.h"


Serial soft_ap_pc(STDIO_UART_TX, STDIO_UART_RX, 115200);


EMW10xxInterface soft_ap_wifi;

/* MiCO GPIO IO driver demo */
int app_soft_ap( )
{
    printf( "start soft ap!\r\n" );

    soft_ap_wifi.set_interface( Soft_AP );
    int ret = soft_ap_wifi.connect( "mbed_softAP", "12345678", NSAPI_SECURITY_WPA_WPA2, 0 );

    if ( ret != NSAPI_ERROR_OK ) {
        printf("Soft ap creation failed\r\n");
        return -1;
    }

    return 0;
}



