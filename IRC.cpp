#include "IRC.h"

#include "Arduino.h"
#include "PWM.h"
#include "Error.h"
#include "Wifi.h"

int robotState;

// main overrides main() defined in main.cpp
int main(void)
{
	init();
	
	#if defined(USBCON)
	USBDevice.attach();
	#endif
	
	Serial.begin(9600);
	Serial.println(); Serial.println(); // Clear possible stuff left behind on serial monitor
	
	Serial.println(F("Arduino init finished"));
	
	//setup error handler
	setupErrors();
	
	//setup PWM
	setupPWM();
	
	//Setup wifi
	setupWifi();
	
	delay(200); //Wait for output and stuff before setting up watchdogs
	            //Idk if this is necessary 
	
	// Setup interrupts for watchdog interrupts
	interrupts();
	Serial.print(F("Watchdogs init... "));
	enableWatchdogs();
	Serial.println(F("done"));
	
	if(robotInit) robotInit();
	
	setState(DISABLED);
	
	while(true)
	{
		if(!fmsConnected() && getState() != DISABLED)
		{
			setState(DISABLED);
		}
		
		//Wifi stuff seems to be broken atm
		/*if(!wifiConnected())
		{
			disableWatchdogs();
			wifiConnect();
			enableWatchdogs();
		} else {*/
			if(!fmsConnected())
			{
				disableWatchdogs();
				fmsConnect();
				enableWatchdogs();
			}
			wifiPeriodic();
		//}
		
		//Call user code
		switch(getState())
		{
		case AUTONOMOUS:
			if(autonomousPeriodic) autonomousPeriodic();
			break;
		case TELEOP:
			if(teleopPeriodic) teleopPeriodic();
			break;
		case DISABLED:
			if(disabledPeriodic) disabledPeriodic();
			break;
		}
	}
	return 0;    
}

int getState()
{
	return robotState;
}

void setState(int state)
{
	robotState = state;
	
	switch(state)
	{
	case AUTONOMOUS:
		enableWatchdogs();
		if(autonomousInit) autonomousInit();
		break;
	case TELEOP:
		enableWatchdogs();
		if(teleopInit) teleopInit();
		break;
	case DISABLED:
		disableWatchdogs();
		resetAllPWM();
		if(disabledInit) disabledInit();
		break;
	}
}