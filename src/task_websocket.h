#ifndef TASK_WEBSOCKET_H // To make sure you don't declare the function more than once by including the header multiple times.
#define TASK_WEBSOCKET_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <MycilaWebSerial.h>

// Other includes
#include "globals.h"
#include "general.h"
#include "websocket.h"

// function declarations
void start_task_websocket(void);
void task_websocket_code(void *pvParameters);
void notifyClients(String);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *ws_server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

#endif