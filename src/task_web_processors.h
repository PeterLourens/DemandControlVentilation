#ifndef TASK_WEB_PROCESSORS_H
#define TAKS_WEB_PROCESSORS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#include "config_files.h"
#include "globals.h"
#include "general.h"
#include "task_wifi.h"

// Function definitions
/*
String valvecontrol_processor(const String&);
String sensor_config_processor(const String&);
String status_processor(const String&);
String settings_processor(const String&);
String settings_valve_state(const String&);*/
String webserial_processor(const String &);

#endif