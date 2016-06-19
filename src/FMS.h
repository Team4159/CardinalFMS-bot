#ifndef FMS_H
#define FMS_H

#include "Error.h"

#include "Arduino.h"
#include "Adafruit_CC3000.h"

#define FMS_IP "10.0.1.49"

#define FMS_HEARTBEAT_DELAY 5000 //Time between required heartbeats to check connection
#define FMS_HEARTBEAT_TIMEOUT 100 //Time heartbeat has to live

class FMS {
public:
	static void periodic();
	static void connect();
	static bool isConnected();
	static void log(ErrorLevel level, char * data);
	static void log(ErrorLevel level, const __FlashStringHelper * data);
private:
	static void sendPacket(char * data);
	static void sendPacket(const __FlashStringHelper * data);
	
	static char* readLine();
	
	static Adafruit_CC3000_Client client;
	static bool connectedToFMS;
	static long fmsLastSeen;
};

#endif