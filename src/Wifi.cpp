#include "Wifi.h"

#include "IRC.h"
#include "Error.h"

/*
	static void log(char * data);
	static void log(const __FlashStringHelper * data);
private:
	static void sendPacket(char * data);
	static void sendPacket(const __FlashStringHelper * data);
	
	static char* readLine();
}
*/
 
Adafruit_CC3000 Wifi::cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER); //WIFI

void Wifi::init()
{
	Serial.print(F("CC3000 init... "));
	if (!cc3000.begin())
	{
		fatal(F("CC3000 init failed."));
	}
	Serial.println(F("done"));
	
	if (!cc3000.deleteProfiles()) {
		fatal(F("Failed to delete old connection profiles"));
	}
	
	//Setup static ip based on team number
	uint32_t ipAddress = cc3000.IP2U32(10, 0, 1, TEAM_NUMBER + 60);
	uint32_t netMask = cc3000.IP2U32(255, 255, 255, 0);
	uint32_t defaultGateway = cc3000.IP2U32(10, 0, 1, 1);
	uint32_t dns = cc3000.IP2U32(8, 8, 4, 4);
	if (!cc3000.setStaticIPAddress(ipAddress, netMask, defaultGateway, dns)) {
		fatal(F("Failed to set static IP!"));
	}
	
	connect();
}

bool Wifi::isConnected()
{
	return cc3000.checkConnected(); //Note: currently broken because cc3000...
}

void Wifi::connect()
{
	Serial.print(F("Wifi connect... "));
	if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
		fatal(F("Wifi connect failed"));
	}
	Serial.println(F("done"));
}
