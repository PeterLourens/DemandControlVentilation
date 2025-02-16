#ifndef FANCONTROL_H
#define FANCONTROL_H

#include <Arduino.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <LittleFS.h>
#include <HTTPClient.h>

#include "globals.h"

// function declarations
void set_fanspeed(String speed);

#endif