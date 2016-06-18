#include "Watchdogs.h"

#include "Arduino.h"
#include "IRC.h"
#include "PWM.h"
#include "Error.h"

long WatchdogValues[PORTS];
bool WatchdogEnabled[PORTS];
bool masterEnable = false;
bool sleeping = false;

void pasueWatchdogs()
{
	sleeping = true;
}

void resumeWatchdogs()
{
	for(int i = 0; i < PORTS; i++)
	{
		Watchdog_feedMotor(i);
	}
	sleeping = false;
}

void disableWatchdogs()
{
	masterEnable = false;
}

bool watchdogsEnabled()
{
	return masterEnable;
}

void checkWatchdogs()
{
	if(!masterEnable || sleeping)
		return;
	for(int i = 0; i < PORTS; i++)
	{
		if(WatchdogEnabled[i])
		{
			if(millis() - WatchdogValues[i] > WATCHDOG_TIMEOUT)
				fatal(F("Watchdog starved."));
		}
	}
}

void Watchdog_addMotor(int port)
{
	WatchdogEnabled[port] = true;
	Watchdog_feedMotor(port);
}

void Watchdog_removeMotor(int port)
{
	WatchdogEnabled[port] = false;
}

void Watchdog_feedMotor(int port)
{
	if(WatchdogEnabled[port])
		WatchdogValues[port] = millis();
}

void disableAllMotors()
{
	for(int i = 0; i < PORTS; i++)
	{
		setPWM(i, 0);
	}
}

short computeOCR1A(long processorSpeed, short prescaler, short frequency)
{
	return (short)(processorSpeed / (prescaler * 1000) / frequency) - 1;
}

void enableWatchdogs()
{
	if(masterEnable)
		return;
	
	cli();//stop interrupts

	//TODO: change values and move to seperate function
	//set timer1 interrupt at speed desired
	TCCR1A = 0;// set entire TCCR1A register to 0
	TCCR1B = 0;// same for TCCR1B
	TCNT1  = 0;//initialize counter value to 0
	//TODO: So as it turns out preprocessor can't handle itself here so I need to fix that someday... or not idc
	OCR1A = computeOCR1A(PROCESSOR_SPEED, WATCHDOG_PRESCALER, WATCHDOG_FREQUENCY); //(must be < 65536)
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	
	// Logic for prescaler pin settings
#if WATCHDOG_PRESCALER == 1
		// Set CS10 bit for 1 prescaler
		TCCR1B |= (1 << CS10);
#elif WATCHDOG_PRESCALER == 8
		// Set CS11 bit for 8 prescaler
		TCCR1B |= (1 << CS11);
#elif WATCHDOG_PRESCALER == 64
		// Set CS11 bit for 64 prescaler
		TCCR1B |= (1 << CS11) | (1 << CS10);
#elif WATCHDOG_PRESCALER == 256
		// Set CS12 bit for 256 prescaler
		TCCR1B |= (1 << CS12);
#elif WATCHDOG_PRESCALER == 1024
		// Set CS12 and CS10 bits for 1024 prescaler
		TCCR1B |= (1 << CS12) | (1 << CS10);
#endif
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);

	sei();//allow interrupts
	
	masterEnable = true;
	
	sleeping = false;
	
	//feed all watchdogs to make sure old woken up ones don't bite
	for(int i = 0; i < PORTS; i++)
	{
		Watchdog_feedMotor(i);
	}
	
}

ISR(TIMER1_COMPA_vect){
	checkWatchdogs();
}
