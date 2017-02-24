#include "mbed.h"
#include "mico.h"


DigitalOut led1(MBED_SYS_LED);
Serial pc(STDIO_UART_TX, STDIO_UART_RX, 115200);

#if 0
#define os_helloworld_log(format, ...)  custom_log("helloworld", format, ##__VA_ARGS__)

// main() runs in its own thread in the OS
// (note the calls to wait below for delays)
int main() {
	int i = 1;

	mico_gpio_initialize( MICO_RF_LED, OUTPUT_PUSH_PULL );
	mico_gpio_initialize( EasyLink_BUTTON, INPUT_PULL_UP );

	mico_gpio_enable_irq( EasyLink_BUTTON, IRQ_TRIGGER_FALLING_EDGE, easylink_pressed_callback, NULL );

    while (true) {
        led1 = !led1;
        os_helloworld_log("This program runs since %d seconds.", mico_rtos_get_time() );
        wait(1);
    }
}
#endif

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

