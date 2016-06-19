#include "PWM.h"

#include "Arduino.h"
#include "Adafruit_PWMServoDriver.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setupPWM(){
	Serial.print(F("PWM init.. "));
	pwm.begin();
	pwm.setPWMFreq(PWM_FREQUENCY);
	resetAllPWM();
	Serial.println(F("done"));
}

void setPWM(int channel, double value) //remember to -1
{
	if(value < -1)
		value = -1;
	if(value > 1)
		value = 1;
	
	value *= -1; // Flip polarity because victors...
	//TODO: Comment this
	pwm.setPWM(channel, ((PWM_MAX_VAL - PWM_MIN_VAL) / 2) * value + (PWM_MIN_VAL + ((PWM_MAX_VAL - PWM_MIN_VAL) / 2)), 4096 - (((PWM_MAX_VAL - PWM_MIN_VAL) / 2) * value + (PWM_MIN_VAL + ((PWM_MAX_VAL - PWM_MIN_VAL) / 2))));
}

void resetAllPWM()
{
	for(int i = 0; i < 16; i++)
		pwm.setPWM(i, 0, 4096);
}