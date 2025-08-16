#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "globals.h"
#include "config_files.h"
#include "i2c.h"


//Function definitions
void config_wifi(void);
int** splitStringsToInts(String[]);

#endif