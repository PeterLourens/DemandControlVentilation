#ifndef GENERAL_H
#define GENERAL_H

#define MAX_FIFO_SIZE 30

#include <Arduino.h>
#include <math.h>

#include "globals.h"

// Function definitions
void print_message(String);
String formatted_datetime(void);
String concatJson(String, String);
String formatted_uptime(void);
void datetime(char *, size_t);
void sensor_data_average(void);
void fifoPush(int, int, int, float, int);
float roundToTwoDecimals(float);

#endif