#include "Error.h"

#include "Arduino.h"
#include "Watchdogs.h"
#include "Wifi.h"

void setupErrors()
{
	pinMode(8, OUTPUT);
	digitalWrite(8, LOW); //Fatal error hasn't happened yet
}

void warning(const __FlashStringHelper* str)
{
	Serial.print(F("[WARNING] ")); Serial.println(str);
	queueData(F("[WARNING] ")); sendPacket(str);
}

void error(const __FlashStringHelper* str)
{
	Serial.print(F("[ERROR] ")); Serial.println(str);
	queueData(F("[ERROR] ")); sendPacket(str);
}

void fatal(const __FlashStringHelper* str)
{
	Serial.print(F("[FATAL] ")); Serial.println(str); //Doesn't usually get through 
	queueData(F("[FATAL] ")); sendPacket(str);
	//Set error pin to high
	digitalWrite(8, HIGH);
	disableAllMotors();
	//Serial.flush(); // Wait for serial output to finish before dying
	for(;;);
}