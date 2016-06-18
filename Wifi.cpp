#include "Wifi.h"

#include "IRC.h"
#include "Error.h"

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER);

Adafruit_CC3000_Client client;

bool connectedToFMS = false;

long fmsLastSeen = 0;

void setupWifi()
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
	
	wifiConnect();
}

void wifiPeriodic()
{
	if(fmsConnected())
	{
		//If it's time then send a heartbeat request
		if(millis() - fmsLastSeen > FMS_HEARTBEAT_DELAY) //Add stuff to prevent dropped packets maybe idc
		{
			disableWatchdogs(); // Make sure watchdogs don't die when checking on heartbeat
			//Serial.println(F("Checking on heartbeat")); //TODO: dleete this
			
			sendPacket("HB_LUB");
			long hbStarted = millis();
			
			while(readLine().equals(""))
			{
				if(millis() - hbStarted > FMS_HEARTBEAT_TIMEOUT)
				{
					connectedToFMS = false;
					client.close();
					Serial.println(F("FMS connection dropped."));
					setState(DISABLED);
					enableWatchdogs();
					return;
				}
			}
			
			fmsLastSeen = millis();
			enableWatchdogs();
		}
		
		//handle packets from fms
		String input = readLine();
		
		if(!input.equals(""))
		{
			if(input.startsWith("S_S_"))
			{
				input.replace("S_S_", "");
				if(input.indexOf("A") != -1)
				{
					setState(AUTONOMOUS);
				} else
				if(input.indexOf("D") != -1)
				{
					setState(DISABLED);
				} else
				if(input.indexOf("T") != -1)
				{
					setState(TELEOP);
				}
				sendPacket("S_ACK");
			}
			else
				Serial.println(input); //SOmething wrong happened so lets output it for debug
			
			fmsLastSeen = millis();
		}
	}
}

bool wifiConnected()
{
	return cc3000.checkConnected();
}

void wifiConnect()
{
	Serial.print(F("Wifi connect... "));
	if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
		fatal(F("Wifi connect failed"));
	}
	Serial.println(F("done"));
}

bool fmsConnected()
{
	return connectedToFMS;
}

void fmsConnect()
{
	if(fmsConnected())
		return;
	
	client = cc3000.connectTCP(cc3000.IP2U32(10, 0, 1, 49), 4159); // Using port 4159 cause why not
	if(!client.connected())
	{
		return;
	}
	
	Serial.print(F("Connecting to FMS... "));
	
	String in;
	
	while((in = readLine()).equals(""));
	
	if(in.indexOf("I_G") == -1)
	{
		cc3000.disconnect();
		return;
	}
	
	client.fastrprint(F("I_"));
	//CHANGE LATER
	char ca[1] = {(char)(((int)'0') + TEAM_NUMBER)};
	client.fastrprintln(ca); //It's janky but whatever it will do
	
	while((in = readLine()).equals(""));
	if(in.indexOf("I_ACK") == -1)
	{
		cc3000.disconnect();
		return;
	}
	
	connectedToFMS = true;
	
	Serial.println(F("done"));
	
	fmsLastSeen = millis();
}

void sendPacket(char * data)
{
	client.fastrprintln(data);
}

void sendPacket(const __FlashStringHelper * data)
{
	client.fastrprintln(data);
}

void queueData(char * data)
{
	client.fastrprint(data);
}

void queueData(const __FlashStringHelper * data)
{
	client.fastrprint(data);
}

//Warning: function hangs until full message or line line recieved
String readLine()
{
	String in = "";
	
	while (client.available()) {
		char c = client.read();
		if(c == '\n')
			return in;
		
		//Serial.print(c); //debug
		in.concat(c);
	}
	
	return in;
}