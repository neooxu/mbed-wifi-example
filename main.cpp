#include "mbed.h"
#include "mico.h"


DigitalOut led1(MBED_SYS_LED);


#if 0
#define os_helloworld_log(format, ...)  custom_log("helloworld", format, ##__VA_ARGS__)

mico_semaphore_t sem = NULL;

void easylink_pressed_callback(void *arg)
{
    mico_rtos_set_semaphore( &sem );
    mico_gpio_output_trigger( MICO_RF_LED );
}

// main() runs in its own thread in the OS
// (note the calls to wait below for delays)
int main() {
	int i = 1;

	mico_gpio_initialize( MICO_RF_LED, OUTPUT_PUSH_PULL );
	mico_gpio_initialize( EasyLink_BUTTON, INPUT_PULL_UP );

	mico_gpio_enable_irq( EasyLink_BUTTON, IRQ_TRIGGER_FALLING_EDGE, easylink_pressed_callback, NULL );

	mico_rtos_init_semaphore( &sem, 1 );

	pc.printf( "Helloworld" );

    while (true) {
//        if( kNoErr != mico_rtos_get_semaphore( &sem, 5000 ) ) {
//            pc.printf( "Get semaphore timeout\r\n" );
//        }
//        else{
//            os_helloworld_log(" Get semaphore success\r\n");
//            led1 = !led1;
//        }
        led1 = !led1;
        mico_rtos_delay_milliseconds( 1000 );
        //wait(1);

    }
}
#endif




#if 0
#define wifi_station_log(M, ...) custom_log("WIFI", M, ##__VA_ARGS__)

static void micoNotify_ConnectFailedHandler(OSStatus err, void* inContext)
{
  wifi_station_log("join Wlan failed Err: %d", err);
}

static void micoNotify_WifiStatusHandler(WiFiEvent event,  void* inContext)
{
  switch (event)
  {
  case NOTIFY_STATION_UP:
    wifi_station_log("Station up");
    break;
  case NOTIFY_STATION_DOWN:
    wifi_station_log("Station down");
    break;
  default:
    break;
  }
}

int main( void )
{
  OSStatus err = kNoErr;
  network_InitTypeDef_adv_st  wNetConfigAdv;

  NVIC_SetPriority( RTC_WKUP_IRQn    ,  1 ); /* RTC Wake-up event   */
  NVIC_SetPriority( SDIO_IRQn        ,  2 ); /* WLAN SDIO           */
  NVIC_SetPriority( DMA2_Stream3_IRQn,  3 ); /* WLAN SDIO DMA       */
  NVIC_SetPriority( USART6_IRQn      ,  6 ); /* MICO_UART_1         */
  NVIC_SetPriority( DMA2_Stream6_IRQn,  7 ); /* MICO_UART_1 TX DMA  */
  NVIC_SetPriority( DMA2_Stream1_IRQn,  7 ); /* MICO_UART_1 RX DMA  */
  NVIC_SetPriority( USART2_IRQn      ,  6 ); /* BT UART             */
  NVIC_SetPriority( DMA1_Stream5_IRQn,  7 ); /* BT UART RX DMA      */
  NVIC_SetPriority( DMA1_Stream6_IRQn,  7 ); /* BT UART TX DMA      */
  NVIC_SetPriority( EXTI0_IRQn       , 14 ); /* GPIO                */
  NVIC_SetPriority( EXTI1_IRQn       , 14 ); /* GPIO                */
  NVIC_SetPriority( EXTI2_IRQn       , 14 ); /* GPIO                */
  NVIC_SetPriority( EXTI3_IRQn       , 14 ); /* GPIO                */
  NVIC_SetPriority( EXTI4_IRQn       , 14 ); /* GPIO                */
  NVIC_SetPriority( EXTI9_5_IRQn     , 14 ); /* GPIO                */
  NVIC_SetPriority( EXTI15_10_IRQn   , 14 ); /* GPIO                */

  MicoInit( );

  /* Register user function when wlan connection status is changed */
  err = mico_system_notify_register( mico_notify_WIFI_STATUS_CHANGED, (void *)micoNotify_WifiStatusHandler, NULL );
  require_noerr( err, exit );

  /* Register user function when wlan connection is faile in one attempt */
  err = mico_system_notify_register( mico_notify_WIFI_CONNECT_FAILED, (void *)micoNotify_ConnectFailedHandler, NULL );
  require_noerr( err, exit );

  /* Initialize wlan parameters */
  memset( &wNetConfigAdv, 0x0, sizeof(wNetConfigAdv) );
  strcpy((char*)wNetConfigAdv.ap_info.ssid, "Xiaomi.Router");   /* wlan ssid string */
  strcpy((char*)wNetConfigAdv.key, "stm32f215");                /* wlan key string or hex data in WEP mode */
  wNetConfigAdv.key_len = strlen("stm32f215");                  /* wlan key length */
  wNetConfigAdv.ap_info.security = SECURITY_TYPE_AUTO;          /* wlan security mode */
  wNetConfigAdv.ap_info.channel = 0;                            /* Select channel automatically */
  wNetConfigAdv.dhcpMode = DHCP_Client;                         /* Fetch Ip address from DHCP server */
  wNetConfigAdv.wifi_retry_interval = 100;                      /* Retry interval after a failure connection */

  /* Connect Now! */
  wifi_station_log("connecting to %s...", wNetConfigAdv.ap_info.ssid);
  micoWlanStartAdv(&wNetConfigAdv);

exit:
  mico_rtos_delete_thread(NULL);
  return err;
}

#endif


#if 0

#define wifi_scan_log(M, ...) custom_log("WIFI", M, ##__VA_ARGS__)

static void micoNotify_ApListCallback(ScanResult *pApList)
{
  int i=0;
  wifi_scan_log("got %d AP", pApList->ApNum);
  for(i=0; i<pApList->ApNum; i++)
  {
    wifi_scan_log("ap%d: name = %s  | strenth=%d",
                  i,pApList->ApList[i].ssid, pApList->ApList[i].ApPower);

  }
}

int main( void )
{
    mico_board_init();

    MicoInit( );

    /* Register user function when wlan scan is completed */
    mico_system_notify_register( mico_notify_WIFI_SCAN_COMPLETED, (void *) micoNotify_ApListCallback, NULL );

    wifi_scan_log("start scan mode, please wait...");
    micoWlanStartScan( );

    mico_rtos_thread_sleep( MICO_WAIT_FOREVER );
}


#endif


#if 0
int main( void )
{
  OSStatus err = kNoErr;
  /* Start MiCO system functions according to mico_config.h,
     Define macro MICO_WLAN_CONNECTION_ENABLE to enable wlan connection function
     Select wlan configuration mode: MICO_WLAN_CONFIG_MODE
     Define EasyLink settings */
  err = mico_system_init( (mico_Context_t *)mico_system_context_init( 0 ) );

  mico_rtos_delete_thread(NULL);
  return err;
}
#endif

#if 1
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

int main( void )
{
    OSStatus err = kNoErr;
    network_InitTypeDef_adv_st  wNetConfigAdv;
    mico_rtos_init_semaphore( &wait_sem, 1 );

    /*Register user function for MiCO nitification: WiFi status changed */
    err = mico_system_notify_register( mico_notify_WIFI_STATUS_CHANGED,
                                       (void *) micoNotify_WifiStatusHandler, NULL );
    require_noerr( err, exit );

    /* Start MiCO system functions according to mico_config.h */
    mico_board_init();
    //mico_system_init( (mico_Context_t *)mico_system_context_init( 0 ) );
#if 1
    MicoInit( );
    cli_init( );
    /* Initialize wlan parameters */
    memset( &wNetConfigAdv, 0x0, sizeof(wNetConfigAdv) );
    strcpy( (char*) wNetConfigAdv.ap_info.ssid, "SWYANG" ); /* wlan ssid string */
    strcpy( (char*) wNetConfigAdv.key, "yangbatian" ); /* wlan key string or hex data in WEP mode */
    wNetConfigAdv.key_len = strlen( "yangbatian" ); /* wlan key length */
    wNetConfigAdv.ap_info.security = SECURITY_TYPE_AUTO; /* wlan security mode */
    wNetConfigAdv.ap_info.channel = 0; /* Select channel automatically */
    wNetConfigAdv.dhcpMode = DHCP_Client; /* Fetch Ip address from DHCP server */
    wNetConfigAdv.wifi_retry_interval = 100; /* Retry interval after a failure connection */

    /* Connect Now! */
    iperf_test_log( "connecting to %s...", wNetConfigAdv.ap_info.ssid );
    micoWlanStartAdv (&wNetConfigAdv);
#endif
    /* Wait for wlan connection*/
    mico_rtos_get_semaphore( &wait_sem, MICO_WAIT_FOREVER );
    iperf_test_log( "Wlan connected successful" );

    /* Register iperf command to test   */
    iperf_cli_register();
    iperf_test_log( "iPerf tester started, input \"iperf -h\" for help." );

exit:
    return err;
}
#endif
