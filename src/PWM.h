#ifndef PWM_H
#define PWM_H

#define PWM_FREQUENCY 500
#define PWM_MIN_VAL 2215
#define PWM_MAX_VAL 3160

void setupPWM();
void setPWM(int channel, double value);
void resetAllPWM();
 
#endif