#ifndef WIFI_H
#define WIFI_H

#include "Adafruit_CC3000.h"
#include <ccspi.h>
#include <SPI.h>
#define CC3000_TINY_DRIVER // Use tiny driver for wifi card to save on program space
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
#define WLAN_SSID "Radio Pinto"
#define WLAN_PASS "mankeydog"
#define WLAN_SECURITY WLAN_SEC_WPA2

class Wifi {
public:
	static void init();
	static void connect();
	static bool isConnected();
	
	static Adafruit_CC3000 cc3000;
};

#endif