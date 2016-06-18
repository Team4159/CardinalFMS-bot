#ifndef IRC_H
#define IRC_H

#warning should be changed for prod
#define TEAM_NUMBER 1

#ifndef TEAM_NUMBER
#error Team number must be defined
#endif

#define PORTS 16

#include "Arduino.h"

#include "Watchdogs.h"
#include "Wifi.h"
#include "PWM.h"

/** PORTS:
 * 0 - OPEN
 * 1 - OPEN
 * 2 - OPEN
 * 3 - CC3000 IRQ
 * 4 - CC3000 CS SD_CARD
 * 5 - CC3000 VBAT_EN
 * 6 - OPEN
 * 7 - OPEN
 * 8 - OPEN
 * 9 - OPEN
 * 10 - CC3000 CS
 * 11 - CC3000 MOSI
 * 12 - CC3000 MISO
 * 13 - CC3000 SCK
*/

// Base robot functions implemented by user
void robotInit() __attribute__((weak));
void autonomousInit() __attribute__((weak));
void autonomousPeriodic() __attribute__((weak));
void teleopInit() __attribute__((weak));
void teleopPeriodic() __attribute__((weak));
void disabledInit() __attribute__((weak));
void disabledPeriodic() __attribute__((weak));

enum RobotState {
	AUTONOMOUS, TELEOP, DISABLED
};

int getState();
void setState(int state);

#endif