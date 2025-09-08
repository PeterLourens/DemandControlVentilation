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
char* webserial_processor(const String &);

#endif