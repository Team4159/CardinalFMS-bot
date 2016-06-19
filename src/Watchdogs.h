#ifndef WATCHDOGS_H
#define WATCHDOGS_H

#define PROCESSOR_SPEED 16000000 // Speed in Hz, change for non-Uno arduino

#define WATCHDOG_TIMEOUT 750 // Robot timeout to default to disabled in milliseconds 
#define WATCHDOG_FREQUENCY 100 //Frequency to check watchdogs in milliseconds
#define WATCHDOG_PRESCALER 1024 //Prescaler for interrupts. Do not change unless you know what you are doing

#include "IRC.h"

//PROCESSOR_SPEED / (WATCHDOG_PRESCALER * (1000 / WATCHDOG_FREQUENCY)) - 1 must be less than 65536

// Watchdog functions
void enableWatchdogs();
void disableWatchdogs();
bool watchdogsEnabled();
void checkWatchdogs();

//TODO: Make a class you lazy bum

void Watchdog_addMotor(int port);
void Watchdog_removeMotor(int port);
void Watchdog_feedMotor(int port);
void disableAllMotors();

#endif