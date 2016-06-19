#include "FMS.h"

#include "Wifi.h"
#include "Error.h"
#include "IRC.h"
#include "util/CharHelper.h"

#include "Arduino.h"
#include "Adafruit_CC3000.h"

Adafruit_CC3000_Client FMS::client;
bool FMS::connectedToFMS = false;
long FMS::fmsLastSeen = -1;

void FMS::periodic()
{
	if(isConnected())
	{
		//If it's time then send a heartbeat request
		if(millis() - fmsLastSeen > FMS_HEARTBEAT_DELAY) //Add stuff to prevent dropped packets maybe idc
		{
			disableWatchdogs(); // Make sure watchdogs don't die when checking on heartbeat
			//Serial.println(F("Checking on heartbeat")); //TODO: dleete this
			
			sendPacket(F("HB_LUB"));
			long hbStarted = millis();
			
			while(str_equals(readLine(), ""))
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
		char* input = readLine();
		
		if(!str_equals(input, ""))
		{
			if(str_startsWith(input, "S_S_"))
			{
				str_replace(input, "S_S_", "");
				if(str_startsWith(input, "A"))
				{
					setState(AUTONOMOUS);
				} else
				if(str_startsWith(input, "D"))
				{
					setState(DISABLED);
				} else
				if(str_startsWith(input, "T"))
				{
					setState(TELEOP);
				}
				sendPacket(F("S_ACK"));
			}
			else
				Serial.println(input); //Something wrong happened so lets output it for debug
			
			fmsLastSeen = millis();
		}
	} else {
		disableWatchdogs();
		connect();
		enableWatchdogs();
	}
}



bool FMS::isConnected()
{
	return connectedToFMS;
}

void FMS::connect()
{
	if(isConnected())
		return;
	
	client = Wifi::cc3000.connectTCP(Wifi::cc3000.IP2U32(10, 0, 1, 49), 4159); // Using port 4159 cause why not
	if(!client.connected())
	{
		return;
	}
	
	Serial.print(F("Connecting to FMS... "));
	
	char* in;
	
	while(str_equals((in = readLine()), ""));
	
	if(str_startsWith(in, "I_G"))
	{
		Wifi::cc3000.disconnect();
		return;
	}
	
	client.fastrprint(F("I_"));
	char ca[1] = {(char)(((int)'0') + TEAM_NUMBER)};
	client.fastrprintln(ca); //It's janky but whatever it will do
	
	while(str_equals((in = readLine()), ""));
	if(str_startsWith(in, "I_ACK"))
	{
		Wifi::cc3000.disconnect();
		return;
	}
	
	connectedToFMS = true;
	
	Serial.println(F("done"));
	
	fmsLastSeen = millis();
}

void FMS::log(ErrorLevel level, char * data) 
{
	switch(level)
	{
	case WARNING:
		client.fastrprint(F("[WARNING] "));
		break;
	case ERROR:
		client.fastrprint(F("[ERROR] "));
		break;
	case FATAL:
		client.fastrprint(F("[FATAL] "));
		break;
	}
	
	client.fastrprintln(data);
}

void FMS::log(ErrorLevel level, const __FlashStringHelper * data) 
{
	switch(level)
	{
	case WARNING:
		client.fastrprint(F("[WARNING] "));
		break;
	case ERROR:
		client.fastrprint(F("[ERROR] "));
		break;
	case FATAL:
		client.fastrprint(F("[FATAL] "));
		break;
	}
	
	client.fastrprintln(data);
}

void FMS::sendPacket(char * data)
{
	client.fastrprintln(data);
}

void FMS::sendPacket(const __FlashStringHelper * data)
{
	client.fastrprintln(data);
}

//Warning: function hangs until full message or line line recieved
char* FMS::readLine() //TODO: change to char*
{
	char* in = "";
	
	while (client.available()) {
		char c = client.read();
		if(c == '\n')
			return in;
		
		//Serial.print(c); //debug
		char thisIsStupid[1] = {c};
		str_concat(in, thisIsStupid); //Oh lawd the jank
	}
	
	return in;
}