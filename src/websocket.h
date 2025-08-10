#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#include "config_files.h"
#include "globals.h"
#include "general.h"

// function declarations
String create_json(void);

#endif
