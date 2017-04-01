#include "mbed.h"
#include "mico.h"

#include "TCPSocket.h"
#include "EMW10xxInterface.h"


DigitalOut soft_ap_led1(MBED_SYS_LED);
Serial soft_ap_pc(STDIO_UART_TX, STDIO_UART_RX, 115200);


#define os_soft_ap_log(format, ...)  custom_log("helloworld", format, ##__VA_ARGS__)


EMW10xxInterface soft_ap_wifi;

const char *sec2str1(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return "None";
        case NSAPI_SECURITY_WEP:
            return "WEP";
        case NSAPI_SECURITY_WPA:
            return "WPA";
        case NSAPI_SECURITY_WPA2:
            return "WPA2";
        case NSAPI_SECURITY_WPA_WPA2:
            return "WPA/WPA2";
        case NSAPI_SECURITY_UNKNOWN:
        default:
            return "Unknown";
    }
}


void scan_demo(EMW10xxInterface *wifi)
{
    WiFiAccessPoint *ap;

    printf("Scan:\r\n");

    int count = wifi->scan(NULL,0);

    /* Limit number of network arbitrary to 15 */
    count = count < 15 ? count : 15;

    ap = new WiFiAccessPoint[count];
    count = wifi->scan(ap, count);
    for (int i = 0; i < count; i++)
    {
        printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\r\n", ap[i].get_ssid(),
               sec2str1(ap[i].get_security()), ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
               ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5], ap[i].get_rssi(), ap[i].get_channel());
    }
    printf("%d networks available.\r\n", count);

    delete[] ap;
}


/* MiCO GPIO IO driver demo */
int app_soft_ap( )
{
    soft_ap_pc.printf( "start!\r\n" );




	scan_demo(&soft_ap_wifi);

    soft_ap_pc.printf( "connecting\r\n" );
	
    int ret = soft_ap_wifi.connect( "mbed_softAP", "12345678", NSAPI_SECURITY_WPA_WPA2, 0, Soft_AP);
    if (ret != 0) {
        printf("\r\nConnection error\r\n");
        return -1;
    }

	 soft_ap_wifi.disconnect();

	 return 0;
}



