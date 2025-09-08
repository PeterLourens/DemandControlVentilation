#ifndef TASK_WIFI_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define TASK_WIFI_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_wifi.h>

#include "wifi_config.h"
#include "general.h"

// function declarations
void start_task_wifi(void);
void task_wifi_code(void * pvParameters);
char* create_webserial_url(void);

#endif