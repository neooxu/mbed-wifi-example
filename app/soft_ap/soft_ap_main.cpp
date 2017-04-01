#include "mbed.h"
#include "mico.h"

#include "TCPSocket.h"
#include "EMW10xxInterface.h"


EMW10xxInterface wlan;

/* MiCO GPIO IO driver demo */
int app_soft_ap( )
{
    printf( "start soft ap!\r\n" );

    wlan.set_interface( Soft_AP );
    int ret = wlan.connect( "mbed_softAP", "12345678", NSAPI_SECURITY_WPA_WPA2, 0 );

    if ( ret != NSAPI_ERROR_OK ) {
        printf("Soft ap creation failed\r\n");
        return -1;
    }

    return 0;
}



