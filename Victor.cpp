#include "Victor.h"

#include "Arduino.h"
#include "PWM.h"
#include "Error.h"
#include "Watchdogs.h"
#include "IRC.h"

Victor::Victor(int channel)
{
	if(channel > 15 || channel < 0)
	{
		fatal(F("Victor initialized with out of bounds channel. Acceptable range 0-15"));
	}
	this->channel = channel;
	set(0);
	setInverted(false);
	
	enable();
}

Victor::~Victor()
{
	Watchdog_removeMotor(channel);
}

void Victor::set(double value)
{
	if(enabled)
	{
		if(getState() == DISABLED)
		{
			setPWM(channel, 0);
		} else {
			setPWM(channel, value * (inverted ? -1 : 1));
			this->value = value;
		}
		
		Watchdog_feedMotor(channel);
	}
}

double Victor::get()
{
	return value;
}

void Victor::setInverted(bool inverted)
{
	//Only update if value is different
	if(this->inverted != inverted)
	{
		this->inverted = inverted;
		//Update value with inversion 
		set(get());
	}
}

bool Victor::getInverted()
{
	return inverted;
}

int Victor::getChannel()
{
	return channel;
}

void Victor::stopMotor()
{
	set(0);
}

void Victor::disable()
{
	stopMotor();
	Watchdog_removeMotor(channel);
	enabled = false;
}

void Victor::enable()
{
	stopMotor();
	Watchdog_addMotor(channel);
	enabled = true;
}

bool Victor::isEnabled()
{
	return enabled;
}