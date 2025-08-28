#include "general.h"

void print_message(String message)
{

    char txBuffer[400];

    if (debug_mode == true)
    {
        strcpy(txBuffer, message.c_str());
        if (webserial_queue != 0)
        {
            if (xQueueSend(webserial_queue, txBuffer, 50))
            {
            }
            else
            {
                Serial.println("Failed to send message to webserial queue.");
            }
        }
    }
}

String formatted_datetime(void)
{

    String temp_datetime = "";
    String temp_year = "";
    String temp_month = "";
    String temp_day = "";
    String temp_hour = "";
    String temp_minute = "";
    String temp_second = "";

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_year = yearStr;
            temp_month = monthStr;
            temp_day = dayStr;
            temp_hour = hourStr;
            temp_minute = minuteStr;
            temp_second = secondStr;
            xSemaphoreGive(date_time_mutex);
        }
    }
    // temp_datetime =  + "/" + monthStr + "/" + dayStr + " - " + hourStr + ":" + minuteStr + ":" + secondStr;
    temp_datetime = temp_year + "/" + temp_month + "/" + temp_day + "-" + temp_hour + ":" + temp_minute + ":" + temp_second;
    return temp_datetime;
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

String formatted_uptime(void)
{
    int uptime_day;
    int uptime_hour;
    int uptime_minute;
    int uptime_second;

    char formatted_uptime[64];

    uint64_t uptime = esp_timer_get_time() / 1000000; // in sec

    uptime_day = uptime / (60 * 60 * 24);    // in full days
    uptime_hour = (uptime / (60 * 60)) % 24; // in full hours
    uptime_minute = (uptime / 60) % 60;      // in full minutes
    uptime_second = uptime % 60;             // in full seconds

    snprintf(formatted_uptime, sizeof(formatted_uptime), "%d d, %d h, %02d m, %02d s",
             uptime_day, uptime_hour, uptime_minute, uptime_second);

    return String(formatted_uptime);
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

    float sensor_data[2][8][3];     // actual sensor data from queque
    float sensor_avg_data[2][8][3]; // Average sensor data to queque
    int pos;

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
                    // sensor_avg_data[bus][slot][measurement] = fifoAverage(bus, slot, measurement, pos);
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
            Serial.print("Send - Average sensor data queue handle is NULL");
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

    /*Serial.print("\n");
    Serial.print(fifoHead[pos]);
    Serial.print("\t\t");
    Serial.print(fifoSize[pos]);
    Serial.print("\t\t");
    Serial.print(value);
    Serial.print("\t\t");
    Serial.print(sum[pos]);
    Serial.print("\t\t");
    Serial.print(avg[pos]);*/
}

float roundToTwoDecimals(float value)
{
    return roundf(value * 100) / 100;
}

bool is_weekend(void)
{
    String temp_day_of_week = "";

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_day_of_week = dayOfWeek;
        xSemaphoreGive(date_time_mutex);
    }

    if (temp_day_of_week == "Saturday" || temp_day_of_week == "Sunday")
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_day(void)
{
    int weekday_day_hour_start = 0;
    int weekday_day_minute_start = 0;
    int weekday_night_hour_start = 0;
    int weekday_night_minute_start = 0;
    int weekend_day_hour_start = 0;
    int weekend_day_minute_start = 0;
    int weekend_night_hour_start = 0;
    int weekend_night_minute_start = 0;
    int temp_hour = 0;
    int temp_minute = 0;

    if (settings_statemachine_mutex && xSemaphoreTake(settings_statemachine_mutex, (TickType_t)10))
    {
        weekday_day_hour_start = statemachinesettings.weekday_day_hour_start;
        weekday_day_minute_start = statemachinesettings.weekday_day_minute_start;
        weekday_night_hour_start = statemachinesettings.weekday_night_hour_start;
        weekday_night_minute_start = statemachinesettings.weekday_night_minute_start;
        weekend_day_hour_start = statemachinesettings.weekend_day_hour_start;
        weekend_day_minute_start = statemachinesettings.weekend_day_minute_start;
        weekend_night_hour_start = statemachinesettings.weekend_night_hour_start;
        weekend_night_minute_start = statemachinesettings.weekend_night_minute_start;
        xSemaphoreGive(settings_statemachine_mutex);
    }

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_hour = hourStr.toInt();
        temp_minute = minuteStr.toInt();
        xSemaphoreGive(date_time_mutex);
    }

    if (is_weekend() == true)
    {
        // Weekend
        // if ((temp_hour >= weekend_day_hour_start && temp_minute >= weekday_day_minute_start) && (temp_hour < weekend_night_hour_start && temp_minute < weekend_night_minute_start))
        if (temp_hour >= weekend_day_hour_start && temp_hour < weekend_night_hour_start)
        {
            return true; // Day
        }
        else
        {
            return false; // Night
        }
    }
    else
    {
        // Weekday
        //if ((temp_hour >= weekday_day_hour_start && temp_minute >= weekday_day_minute_start) && (temp_hour < weekday_night_hour_start && temp_minute < weekday_night_minute_start))
        if (temp_hour >= weekday_day_hour_start  && temp_hour < weekday_night_hour_start)
        {
            return true; // Day
        }
        else
        {
            return false; // Night
        }
    }
}