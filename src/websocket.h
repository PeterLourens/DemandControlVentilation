#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#include "config_files.h"
#include "globals.h"
#include "general.h"

// function declarations
String create_index_page_json(void);
String create_settings_json(void);
String create_valvecontrol_json(void);
String create_sensors_json(void);
String create_statemachine_json(void);

#endif
