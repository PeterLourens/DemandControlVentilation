#ifndef GENERAL_H
#define GENERAL_H

#define MAX_FIFO_SIZE 25
#define MIN_LOG_LEVEL LOG_INFO

#include <Arduino.h>
#include <math.h>
#include <WiFi.h>

#include "globals.h"

typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

// Function definitions
void printmessage(LogLevel level, const char *);
String concatJson(String, String);
void formatted_uptime(char *, size_t);
void formatted_daydatetime(char *, size_t);
void formatted_datetime(char *, size_t);
void formatted_date(char *, size_t);
void formatted_time(char *, size_t);
void formatted_day(char *, size_t);
void ip_address(char *, size_t);
void sensor_data_average(void);
void fifoPush(int, int, int, float, int);
float roundToTwoDecimals(float);
int get_free_heap_size(void);
int get_min_ever_heap_size(void);
void check_task_status(void);

#endif