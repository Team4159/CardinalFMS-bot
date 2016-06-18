#ifndef WIFI_H
#define WIFI_H

#include "Adafruit_CC3000.h"
#include <ccspi.h>
#include <SPI.h>
#define CC3000_TINY_DRIVER // Use tiny driver for wifi card to save on program space
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
#define WLAN_SSID "Radio Pinto"
#define WLAN_PASS "mankeydog"
#define WLAN_SECURITY WLAN_SEC_WPA2

#define FMS_IP "10.0.1.49"

#define FMS_HEARTBEAT_DELAY 5000 //Time between required heartbeats to check connection
#define FMS_HEARTBEAT_TIMEOUT 100 //Time heartbeat has to live

#include "Arduino.h"

void setupWifi();
void wifiPeriodic(); //Eh bad name but whatever

bool wifiConnected();
void wifiConnect();

bool fmsConnected();
void fmsConnect();

void sendPacket(char * data);
void sendPacket(const __FlashStringHelper * data);

//These should be removed later
void queueData(char * data);
void queueData(const __FlashStringHelper * data);
String readLine();

#endif