#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <LittleFS.h>

#include "task_valvecontrol.h"
#include "task_web.h"
#include "task_i2c.h"
#include "task_mqtt.h"
#include "task_statemachine.h"
#include "task_system.h"
#include "task_influxdb.h"
#include "task_neopixel.h"
#include "task_wifi.h"
#include "task_system.h"
#include "task_espnow.h"
#include "task_websocket.h"

#include "rtc.h"
#include "LiquidCrystal_I2C.h"

#include "valvecontrol.h"
#include "globals.h"
#include "general.h"

// function declarations
void setup_wifi(void);
void init_registers(void);

#endif
