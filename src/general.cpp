#include "general.h"

LogLevel min_log_level = MIN_LOG_LEVEL;

const char *log_level_str[] = {"DEBUG", "INFO", "WARN", "ERROR"};

void printmessage(LogLevel level, const char *message)
{
    char txBuffer[400] = {};
    char msg[MSG_SIZE] = {};

    if (debug_mode && level >= min_log_level)
    {
        // Prefix message with log level
        snprintf(txBuffer, sizeof(txBuffer), "[%s] %s", log_level_str[level], message);

        if (webserial_queue != 0)
        {
            if (!xQueueSend(webserial_queue, txBuffer, 50))
            {
                snprintf(msg, sizeof(msg), "Failed to send message to webserial queue.");
                // Use LOG_ERROR for recursive call to avoid infinite recursion
                if (level != LOG_ERROR)
                {
                    printmessage(LOG_ERROR, msg);
                }
            }
        }
    }
}

void formatted_daydatetime(char *buf, size_t bufsize)
{
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
    int dayofweek = 0;

    static const char *day_names[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        year = rtcdatetime.year;
        month = rtcdatetime.month;
        day = rtcdatetime.day;
        hour = rtcdatetime.hour;
        minute = rtcdatetime.minute;
        second = rtcdatetime.second;
        dayofweek = rtcdatetime.day_of_week;
        xSemaphoreGive(date_time_mutex);
    }

    // Clamp dayofweek to valid range just in case
    if (dayofweek < 0 || dayofweek > 6)
    {
        dayofweek = 0;
    }

    snprintf(buf, bufsize, "%s %04d/%02d/%02d - %02d:%02d:%02d", day_names[dayofweek], year, month, day, hour, minute, second);
}

void formatted_datetime(char *buf, size_t bufsize)
{
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        year = rtcdatetime.year;
        month = rtcdatetime.month;
        day = rtcdatetime.day;
        hour = rtcdatetime.hour;
        minute = rtcdatetime.minute;
        second = rtcdatetime.second;
        xSemaphoreGive(date_time_mutex);
    }
    snprintf(buf, bufsize, "%04d/%02d/%02d - %02d:%02d:%02d", year, month, day, hour, minute, second);
}

void formatted_date(char *buf, size_t bufsize)
{
    int year = 0;
    int month = 0;
    int day = 0;

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        year = rtcdatetime.year;
        month = rtcdatetime.month;
        day = rtcdatetime.day;
        xSemaphoreGive(date_time_mutex);
    }
    snprintf(buf, bufsize, "%04d/%02d/%02d", year, month, day);
}

void formatted_time(char *buf, size_t bufsize)
{
    int hour = 0;
    int minute = 0;
    int second = 0;

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        hour = rtcdatetime.hour;
        minute = rtcdatetime.minute;
        second = rtcdatetime.second;
        xSemaphoreGive(date_time_mutex);
    }
    snprintf(buf, bufsize, "%02d:%02d:%02d", hour, minute, second);
}

void formatted_day(char *buf, size_t bufsize)
{
    int dayofweek = 0;
    static const char *day_names[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        dayofweek = rtcdatetime.day_of_week;
        xSemaphoreGive(date_time_mutex);
    }

    // Clamp dayofweek to valid range just in case
    if (dayofweek < 0 || dayofweek > 6)
    {
        dayofweek = 0;
    }

    snprintf(buf, bufsize, "%s", day_names[dayofweek]);
}

void ip_address(char *buf, size_t bufsize)
{
    IPAddress ip = WiFi.localIP();

    snprintf(buf, bufsize, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
}

String concatJson(String json1, String json2)
{
    // Remove the closing brace from json1
    if (json1.endsWith("}"))
    {
        json1.remove(json1.length() - 1);
    }

    // Remove the opening brace from json2
    if (json2.startsWith("{"))
    {
        json2.remove(0, 1);
    }

    // Concatenate with a comma in between
    String result = json1 + "," + json2;

    return result;
}

void formatted_uptime(char *buf, size_t bufsize)
{
    uint64_t uptime = esp_timer_get_time() / 1000000; // uptime in seconds

    int uptime_day = uptime / (60 * 60 * 24);
    int uptime_hour = (uptime / (60 * 60)) % 24;
    int uptime_minute = (uptime / 60) % 60;
    int uptime_second = uptime % 60;

    snprintf(buf, bufsize, "%dd %dh %02dm %02ds", uptime_day, uptime_hour, uptime_minute, uptime_second);
}

/*
Rolling  average for sensors

- This function is called after sensor is read.
- The oldest value is removed from the array
- All other values are moved one position up
- The new value is moved into the first position
- The new average value of the array is calculated
- The new average function is stored globally (similar structure to sensor readings)
*/

float sensor_fifo[2][8][3][MAX_FIFO_SIZE];
float sum[48] = {0.0};
float avg[48] = {0.0};
int fifoSize[48] = {0}; // Current number of elements in the FIFO.
int fifoHead[48] = {0}; // Index of the oldest element in the FIFO.

void sensor_data_average(void)
{
    int pos;
    float sensor_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];     // actual sensor data from queque
    float sensor_avg_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS]; // Average sensor data to queque
    char msg[MSG_SIZE] = {};

    if (xQueuePeek(sensor_queue, &sensor_data, 0) == pdTRUE)
    {
        pos = 0;
        for (int bus = 0; bus < 2; bus++)
        {
            for (int slot = 0; slot < 8; slot++)
            {
                for (int measurement = 0; measurement < 3; measurement++)
                {
                    fifoPush(bus, slot, measurement, sensor_data[bus][slot][measurement], pos);
                    sensor_avg_data[bus][slot][measurement] = avg[pos];
                    pos++; // counter for fifoSize and fifoHead
                }
            }
        }
        if (sensor_avg_queue != NULL)
        {
            xQueueOverwrite(sensor_avg_queue, &sensor_avg_data);
        }
        else
        {
            snprintf(msg, sizeof(msg), "Average sensor data queue handle is NULL.");
            printmessage(LOG_ERROR, msg);
        }
    }
}

void fifoPush(int bus, int slot, int measurement, float value, int pos)
{
    if (fifoSize[pos] < MAX_FIFO_SIZE)
    {
        // Add the value to the next available position.
        sensor_fifo[bus][slot][measurement][(fifoHead[pos] + fifoSize[pos]) % MAX_FIFO_SIZE] = value;
        sum[pos] += value;
        fifoSize[pos] = fifoSize[pos] + 1;
    }
    else
    {
        // Overwrite the oldest value and move the head forward.
        sum[pos] -= sensor_fifo[bus][slot][measurement][fifoHead[pos]];
        sensor_fifo[bus][slot][measurement][fifoHead[pos]] = value;
        sum[pos] += value;
        fifoHead[pos] = (fifoHead[pos] + 1) % MAX_FIFO_SIZE;
    }
    avg[pos] = sum[pos] / fifoSize[pos];
}

float roundToTwoDecimals(float value)
{
    return roundf(value * 100) / 100;
}

int get_free_heap_size(void)
{
    return xPortGetFreeHeapSize();
}

int get_min_ever_heap_size(void)
{
    return xPortGetMinimumEverFreeHeapSize();
}

void check_task_status(void) {

    char msg[MSG_SIZE] = {};

    /*eTaskState wifi_state = eTaskGetState(task_wifi);
    eTaskState i2c_state = eTaskGetState(task_i2c);
    eTaskState statemachine_state = eTaskGetState(task_statemach);
    eTaskState influxdb_state = eTaskGetState(task_influxdb);
    eTaskState mqtt_state = eTaskGetState(task_mqtt);
    eTaskState np_state = eTaskGetState(task_np);
    eTaskState system_state = eTaskGetState(task_sys);
    eTaskState valvectrl_state = eTaskGetState(task_valvectrl);
    eTaskState h_Task_web_state = eTaskGetState(h_Task_web);
    eTaskState websocket_state = eTaskGetState(task_websocket);

    if (wifi_state != eDeleted && wifi_state != eInvalid)
    {
        snprintf(msg, sizeof(msg), "Task WIFI has started or is running.");
        printmessage(LOG_INFO, msg);
    }
    if (i2c_state != eDeleted && i2c_state != eInvalid)
    {
        snprintf(msg, sizeof(msg), "Task I2C has started or is running.");
        printmessage(LOG_INFO, msg);
    }
    if (influxdb_state != eDeleted && influxdb_state != eInvalid)
    {
        snprintf(msg, sizeof(msg), "Task InfluxDB has started or is running.");
        printmessage(LOG_INFO, msg);
    }
    if (mqtt_state != eDeleted && mqtt_state != eInvalid)
    {
        snprintf(msg, sizeof(msg), "Task MQTT has started or is running.");
        printmessage(LOG_INFO, msg);
    }
    if (np_state != eDeleted && np_state != eInvalid)
    {
        snprintf(msg, sizeof(msg), "Task Neopixel has started or is running.");
        printmessage(LOG_INFO, msg);
    }
    if (statemachine_state != eDeleted && statemachine_state != eInvalid)
    {
        snprintf(msg, sizeof(msg), "Task statemachine has started or is running.");
        printmessage(LOG_INFO, msg);
    }
    if (system_state != eDeleted && system_state != eInvalid)
    {
        snprintf(msg, sizeof(msg), "Task system has started or is running.");
        printmessage(LOG_INFO, msg);
    }
    if (valvectrl_state != eDeleted && valvectrl_state != eInvalid)
    {
        snprintf(msg, sizeof(msg), "Task valvecontrol has started or is running.");
        printmessage(LOG_INFO, msg);
    }
    if (h_Task_web_state != eDeleted && h_Task_web_state != eInvalid)
    {
        snprintf(msg, sizeof(msg), "Task web has started or is running.");
        printmessage(LOG_INFO, msg);
    }
    if (websocket_state != eDeleted && websocket_state != eInvalid)
    {
        snprintf(msg, sizeof(msg), "Task web has started or is running.");
        printmessage(LOG_INFO, msg);
    }*/
}