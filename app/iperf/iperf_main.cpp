#include "mbed.h"
#include "mico.h"

#include "iperf_cli.h"

#define  iperf_test_log(M, ...) custom_log("Iperf", M, ##__VA_ARGS__)

static mico_semaphore_t wait_sem = NULL;

static void micoNotify_WifiStatusHandler( WiFiEvent status, void* const inContext )
{
    switch ( status )
    {
        case NOTIFY_STATION_UP:
            mico_rtos_set_semaphore( &wait_sem );
            break;
        case NOTIFY_STATION_DOWN:
        case NOTIFY_AP_UP:
        case NOTIFY_AP_DOWN:
            break;
    }
}

int app_iperf( void )
{
    OSStatus err = kNoErr;
    network_InitTypeDef_adv_st  wNetConfigAdv;
    mico_rtos_init_semaphore( &wait_sem, 1 );

    /*Register user function for MiCO notification: WiFi status changed */
    err = mico_system_notify_register( mico_notify_WIFI_STATUS_CHANGED,
                                       (void *) micoNotify_WifiStatusHandler, NULL );
    require_noerr( err, exit );

    /* Start MiCO system functions according to mico_config.h */
    mico_board_init();
    //mico_system_init( (mico_Context_t *)mico_system_context_init( 0 ) );

    MicoInit( );
    cli_init( );
    /* Initialize wlan parameters */
    memset( &wNetConfigAdv, 0x0, sizeof(wNetConfigAdv) );
    strcpy( (char*) wNetConfigAdv.ap_info.ssid, "William Xu" ); /* wlan ssid string */
    strcpy( (char*) wNetConfigAdv.key, "mx099555" ); /* wlan key string or hex data in WEP mode */
    wNetConfigAdv.key_len = strlen( "mx099555" ); /* wlan key length */
    wNetConfigAdv.ap_info.security = SECURITY_TYPE_AUTO; /* wlan security mode */
    wNetConfigAdv.ap_info.channel = 0; /* Select channel automatically */
    wNetConfigAdv.dhcpMode = DHCP_Client; /* Fetch Ip address from DHCP server */
    wNetConfigAdv.wifi_retry_interval = 100; /* Retry interval after a failure connection */

    /* Connect Now! */
    iperf_test_log( "connecting to %s...", wNetConfigAdv.ap_info.ssid );
    micoWlanStartAdv (&wNetConfigAdv);

    /* Wait for wlan connection*/
    mico_rtos_get_semaphore( &wait_sem, MICO_WAIT_FOREVER );
    iperf_test_log( "Wlan connected successful" );

    /* Register iperf command to test   */
    iperf_cli_register();
    iperf_test_log( "iPerf tester started, input \"iperf -h\" for help." );

exit:
    return err;
}
