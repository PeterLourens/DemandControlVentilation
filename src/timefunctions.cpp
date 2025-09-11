#include "timefunctions.h"

bool cooking_times(void)
{
    int temp_hour = 0;
    int temp_minute = 0;
    int start_hour_state_cooking = 0;
    int start_min_state_cooking = 0;
    int stop_hour_state_cooking = 0;
    int stop_min_state_cooking = 0;

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = rtcdatetime.hour;
            temp_minute = rtcdatetime.minute;
            xSemaphoreGive(date_time_mutex);
        }
    }

    // Read RH levels for transition to highrhday state from global jsonDocument
    if (settings_state_cooking_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_cooking_mutex, (TickType_t)100) == pdTRUE)
        {
            start_hour_state_cooking = statecookingsettings.start_hour_state_cooking;
            start_min_state_cooking = statecookingsettings.start_minute_state_cooking;
            stop_hour_state_cooking = statecookingsettings.stop_hour_state_cooking;
            stop_min_state_cooking = statecookingsettings.stop_minute_state_cooking;
            xSemaphoreGive(settings_state_cooking_mutex);
        }
    }
    if (temp_hour == start_hour_state_cooking && temp_minute > start_min_state_cooking)
        return true;
    if (temp_hour == stop_hour_state_cooking && temp_minute >= stop_min_state_cooking)
        return false;
    else
        return false;
}

bool valve_cycle_times_day(void)
{
    int temp_hour = 0;
    int temp_minute = 0;

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = rtcdatetime.hour;
            temp_minute = rtcdatetime.minute;
            xSemaphoreGive(date_time_mutex);
        }
    }

    if (temp_hour == 10 && temp_minute < 30)
        return true;
    else if (temp_hour == 10 && temp_minute >= 30)
        return false;
    else if (temp_hour == 12 && temp_minute < 30)
        return true;
    else if (temp_hour == 12 && temp_minute >= 30)
        return false;
    else if (temp_hour == 14 && temp_minute < 30)
        return true;
    else if (temp_hour == 14 && temp_minute >= 30)
        return false;
    else if (temp_hour == 16 && temp_minute < 30)
        return true;
    else if (temp_hour == 16 && temp_minute >= 30)
        return false;
    else if (temp_hour == 18 && temp_minute > 30)
        return true;
    else if (temp_hour == 18 && temp_minute >= 58)
        return false;
    else if (temp_hour == 20 && temp_minute < 30)
        return true;
    else if (temp_hour == 20 && temp_minute >= 30)
        return false;
    else
        return false;
}

bool valve_cycle_times_night(void)
{

    int temp_hour = 0;
    int temp_minute = 0;

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = rtcdatetime.hour;
            temp_minute = rtcdatetime.minute;
            xSemaphoreGive(date_time_mutex);
        }
    }

    if (temp_hour == 23 && temp_minute < 30)
        return true;
    else if (temp_hour == 23 && temp_minute >= 30)
        return false;
    else if (temp_hour == 1 && temp_minute < 30)
        return true;
    else if (temp_hour == 1 && temp_minute >= 30)
        return false;
    else if (temp_hour == 3 && temp_minute < 30)
        return true;
    else if (temp_hour == 3 && temp_minute >= 30)
        return false;
    else if (temp_hour == 5 && temp_minute < 30)
        return true;
    else if (temp_hour == 5 && temp_minute >= 30)
        return false;
    else
        return false;
}

bool is_weekend(void)
{
    char day_buffer[10];
    formatted_day(day_buffer, sizeof(day_buffer));

    //if (strcmp(day_buffer, "Saturday") == 0 || strcmp(day_buffer, "Sunday") == 0)
    if (String(day_buffer) == "Saturday" || String(day_buffer) == "Sunday")
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
    int current_minutes = 0;

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
        current_minutes = rtcdatetime.hour * 60 + rtcdatetime.minute;
        xSemaphoreGive(date_time_mutex);
    }

    if (is_weekend())
    {
        int day_start = weekend_day_hour_start * 60 + weekend_day_minute_start;
        int night_start = weekend_night_hour_start * 60 + weekend_night_minute_start;
        return current_minutes >= day_start && current_minutes < night_start;
    }
    else
    {
        int day_start = weekday_day_hour_start * 60 + weekday_day_minute_start;
        int night_start = weekday_night_hour_start * 60 + weekday_night_minute_start;
        return current_minutes >= day_start && current_minutes < night_start;
    }
}
