#include "mbed.h"
#include "mico.h"

#include "TCPSocket.h"
#include "EMW10xxInterface.h"


Serial soft_ap_pc(STDIO_UART_TX, STDIO_UART_RX, 115200);


#define os_soft_ap_log(format, ...)  custom_log("softAP", format, ##__VA_ARGS__)

EMW10xxInterface soft_ap_wifi;

/* MiCO GPIO IO driver demo */
int app_soft_ap( )
{
	os_soft_ap_log( "start!\r\n" );

	int ret = soft_ap_wifi.connect( "mbed_softAP", "12345678", NSAPI_SECURITY_WPA_WPA2, 0, Soft_AP);
	if (ret != 0) {
		os_soft_ap_log("\r\nConnection error\r\n");
		return -1;
	}

	soft_ap_wifi.disconnect();

	return 0;
}



