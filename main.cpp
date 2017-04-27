#include "mbed.h"
#include "mico.h"



#define RUN_APPLICATION( app )     extern int app_##app(void); return app_##app();


int main( void )
{
    /* APPLICATION can be assigned to the folder names under folder "APP" */
    //RUN_APPLICATION( iperf );
    RUN_APPLICATION( audio );
    //RUN_APPLICATION( mbed_wifi );
    //RUN_APPLICATION( mbed_tls_client );
    //RUN_APPLICATION( mbed_tcp_udp );
    //RUN_APPLICATION( soft_ap );

	return 0;

}

