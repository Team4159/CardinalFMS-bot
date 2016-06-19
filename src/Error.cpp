#include "Error.h"

#include "Watchdogs.h"
#include "FMS.h"

#include "Arduino.h"

void setupErrors()
{
	pinMode(8, OUTPUT);
	digitalWrite(8, LOW); //Fatal error hasn't happened yet
}

void warning(const __FlashStringHelper* str)
{
	Serial.print(F("[WARNING] ")); Serial.println(str);
	FMS::log(WARNING, str);
}

void error(const __FlashStringHelper* str)
{
	Serial.print(F("[ERROR] ")); Serial.println(str);
	FMS::log(ERROR, str);
}

void fatal(const __FlashStringHelper* str)
{
	Serial.print(F("[FATAL] ")); Serial.println(str); //Doesn't usually get through 
	FMS::log(FATAL, str);
	//Set error pin to high
	digitalWrite(8, HIGH);
	disableAllMotors();
	//Serial.flush(); // Wait for serial output to finish before dying
	for(;;);
}