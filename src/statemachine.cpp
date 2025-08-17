/* Statemachine code */

#include "statemachine.h"

float statemachine_sensor_data[2][8][3];
float statemachine_avg_sensor_data[2][8][3];

int co2_sensor_counter = 0;
int rh_sensor_counter = 0;

long old_time = 0;
long elapsed_time = 0;

String new_state = "";
String temp_fanspeed = "Low";

struct CO2_Sensors
{
    String valve;
    float co2_reading;
};
struct RH_Sensors
{
    String valve;
    float rh_reading;
};

CO2_Sensors co2_sensors[16];
RH_Sensors rh_sensors[16];

void init_statemachine(void)
{

    String temp_fanspeed = "";

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = "init";
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            fanspeed = temp_fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }
    set_fanspeed(temp_fanspeed);
}

void run_statemachine(void)
{
    String message = "";
    String temp_state = "";

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_state = state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    message = "Read sensor data from queue for statemachine.";
    print_message(message);
    if (xQueuePeek(sensor_queue, &statemachine_sensor_data, 0) == pdTRUE)
    {
    }

    message = "Read average sensor data from queue for statemachine.";
    print_message(message);
    if (xQueuePeek(sensor_avg_queue, &statemachine_avg_sensor_data, 0) == pdTRUE)
    {
    }

    // Refresh config for statemachine
    // process_statemachine_config();

    if (temp_state == "init")
    {
        init_transitions();
    }
    else if (temp_state == "stopped")
    {
        stopped_transitions();
    }
    else if (temp_state == "day")
    {
        day_transitions();
    }
    else if (temp_state == "night")
    {
        night_transitions();
    }
    else if (temp_state == "highco2day")
    {
        high_co2_day_transitions();
    }
    else if (temp_state == "highco2night")
    {
        high_co2_night_transitions();
    }
    else if (temp_state == "highrhday")
    {
        high_rh_day_transitions();
    }
    else if (temp_state == "highrhnight")
    {
        high_rh_night_transitions();
    }
    else if (temp_state == "cooking")
    {
        cooking_transitions();
    }
    else if (temp_state == "cyclingday")
    {
        valve_cycle_day_transitions();
    }
    else if (temp_state == "cyclingnight")
    {
        valve_cycle_night_transitions();
    }
    else if (temp_state == "fanhighspeed")
    {
        manual_high_speed_transitions();
    }
    else
    {
        // This state should normally never be entered. Back to init of statemachine to keep it running
        message = "Error in state number, back to init state";
        print_message(message);
        init_transitions();
    }
}

void stopped_transitions(void)
{
    String message = "";

    // Actions for this state
    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = "stopped";
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    // No further logic required because when start statemachine button is pushed the statemachine will go back to init state
    message = "Statemachine in stopped state. Push start statemachine button on the Valve Control web page to continue.";
    print_message(message);
}

void init_transitions(void)
{

    int temp_hour = 0;
    int temp_minute = 0;

    String statemachine_state = "init";
    String temp_fanspeed = "Low";
    String temp_day_of_week = "";
    String message = "";

    /*int weekday_day_hour_start_temp;
    int weekday_day_minute_start_temp;
    int weekday_night_hour_start_temp;
    int weekday_night_minute_start_temp;
    int weekend_day_hour_start_temp;
    int weekend_day_minute_start_temp;
    int weekend_night_hour_start_temp;
    int weekend_night_minute_start_temp;
    String weekend_day_1_temp;
    String weekend_day_2_temp;
    int minimum_state_time_temp;*/

    // Actions for this state
    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = statemachine_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = hourStr.toInt();
            temp_minute = minuteStr.toInt();
            temp_day_of_week = dayOfWeek;
            xSemaphoreGive(date_time_mutex);
        }
    }

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            fanspeed = temp_fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    /*if (settings_statemachine_mutex != NULL)
    {
        if (xSemaphoreTake(settings_statemachine_mutex, (TickType_t)20) == pdTRUE)
        {
            weekday_day_hour_start_temp = weekday_day_hour_start;
            weekday_day_minute_start_temp = weekday_day_minute_start;
            weekday_night_hour_start_temp = weekday_night_hour_start;
            weekday_night_minute_start_temp = weekday_night_minute_start;
            weekend_day_hour_start_temp = weekend_day_hour_start;
            weekend_day_minute_start_temp = weekend_day_minute_start;
            weekend_night_hour_start_temp = weekend_night_hour_start;
            weekend_night_minute_start_temp = weekday_night_minute_start;
            weekend_day_1_temp = weekend_day_1;
            weekend_day_2_temp = weekend_day_2;
            minimum_state_time_temp = minimum_state_time;
            xSemaphoreGive(settings_statemachine_mutex);
        }
    }*/

    message = "Statemachine in state " + statemachine_state + ". It is " + temp_hour + ":" + temp_minute + " and day of week is " + temp_day_of_week + ", fanspeed is " + temp_fanspeed;
    print_message(message);
    set_fanspeed(temp_fanspeed);

    // Conditions to transit to other state, only evalaution based on time and day of week
    /*if (temp_hour >= weekday_day_hour_start_temp &&
        temp_hour < weekday_night_hour_start_temp &&
        temp_day_of_week != weekend_day_1_temp &&
        temp_day_of_week != weekend_day_2_temp)*/
    if (temp_hour >= 8 && temp_hour < 21 && temp_day_of_week != "Saturday" && temp_day_of_week != "Sunday")
    { // Weekday
        message = "It is after 8, before 21 and a weekday. Transit to day.";
        print_message(message);
        new_state = "day";
    }
    //else if (temp_hour >= weekend_day_hour_start_temp && temp_hour < weekend_night_hour_start_temp && (temp_day_of_week == weekend_day_1_temp || temp_day_of_week == weekend_day_2_temp))
    else if (temp_hour >= 9 && temp_hour < 21 && (temp_day_of_week == "Saturday" || temp_day_of_week == "Sunday"))
    { // Weekend
        message = "It is after 9 and before 21 and weekend. Transit to day.";
        print_message(message);
        new_state = "day";
    }
    else
    {
        message = "No conditions met for day, so transit to night.";
        print_message(message);
        new_state = "night";
    }

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = new_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }
}

void day_transitions(void)
{

    int temp_hour = 0;
    int temp_minute = 0;
    int co2_sensors_high = 0;
    int rh_sensors_high = 0;
    int rhhighlevel = 0;
    int co2highlevel = 0;
    int min_state_time = 600; // minimum time in seconds the statemachine should stay in this state
    long new_time = 0;
    bool valve_move_locked = 0;

    String temp_fanspeed = "";
    String statemachine_state = "day";
    String temp_day_of_week = "";
    String message = "";

    // Actions for this state
    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = statemachine_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = hourStr.toInt();
            temp_minute = minuteStr.toInt();
            temp_day_of_week = dayOfWeek;
            xSemaphoreGive(date_time_mutex);
        }
    }

    if (settings_state_day_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_day_mutex, (TickType_t)100) == pdTRUE)
        {
            String state_fanspeed = settings_state_day[String("state_day_fanspeed")];
            temp_fanspeed = state_fanspeed;
            xSemaphoreGive(settings_state_day_mutex);
        }
    }

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            fanspeed = temp_fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    if (lock_valve_move_mutex != NULL)
    {
        if (xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
        {
            valve_move_locked = lock_valve_move;
            xSemaphoreGive(lock_valve_move_mutex);
        }
    }

    // Read CO2 levels for transition to highCO2day state from global jsonDocument
    if (settings_state_highco2day_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)100) == pdTRUE)
        {
            co2highlevel = settings_state_highco2day["co2_high_state_highco2day"];
            xSemaphoreGive(settings_state_highco2day_mutex);
        }
    }

    // Read RH levels for transition to highrhday state from global jsonDocument
    if (settings_state_highrhday_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highrhday_mutex, (TickType_t)100) == pdTRUE)
        {
            rhhighlevel = settings_state_highrhday["rh_high_state_highrhday"];
            xSemaphoreGive(settings_state_highrhday_mutex);
        }
    }

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + ", fanspeed is " + temp_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(temp_fanspeed);
    select_sensors();

    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    { // Just in case that a reboot happened and old_time is not set
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    message = "High CO2 detection level: " + String(co2highlevel) + ". High RH detection level: " + String(rhhighlevel);
    print_message(message);

    if (valve_move_locked == 0)
    {
        valve_position_statemachine(statemachine_state);
    }
    else
    {
        message = "Valves are locked for moving, will try again later";
        print_message(message);
    }

    for (int i = 0; i < co2_sensor_counter; i++)
    {
        if (co2_sensors[i].co2_reading > co2highlevel)
        {
            message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has high CO2 reading. Transit to highco2day state";
            print_message(message);
            co2_sensors_high++;
        }
    }

    for (int i = 0; i < rh_sensor_counter; i++)
    {
        if (rh_sensors[i].rh_reading > rhhighlevel)
        {
            message = "Sensor" + String(i) + " which is located at " + String(rh_sensors[i].valve) + " has high RH reading. Transit to highrhday state";
            print_message(message);
            rh_sensors_high++;
        }
    }

    message = "Number of sensors measure high CO2: " + String(co2_sensors_high) + ". Number of sensors measure high RH: " + String(rh_sensors_high);
    print_message(message);

    if (co2_sensors_high > 0 && elapsed_time >= min_state_time)
    {
        new_state = "highco2day";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (rh_sensors_high > 0 && elapsed_time >= min_state_time)
    {
        new_state = "highrhday";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (temp_hour >= 21)
    {
        message = "It's night. Transit to night.";
        print_message(message);
        new_state = "night";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (cooking_times() == true)
    {
        message = "It's day and cooking time. Transit to cooking state.";
        print_message(message);
        new_state = "cooking";
    }
    else if (valve_cycle_times_day() == true)
    {
        message = "It's day and valve_cycle_time_day. Transit to valvecycleday state";
        print_message(message);
        new_state = "cyclingday";
    }
    // Manual high speed mode is ignored for now
    else
    {
        message = "Conditions have not changed, it's still day";
        print_message(message);
        new_state = "day";
    }

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = new_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }
}

void night_transitions(void)
{

    int temp_hour = 0;
    int temp_minute = 0;
    int co2_sensors_high = 0;
    int rh_sensors_high = 0;
    int co2highlevel = 0;
    int rhhighlevel = 0;
    int min_state_time = 600; // minimum time in seconds the statemachine should stay in this state
    long new_time = 0;
    bool valve_move_locked = 0;

    String statemachine_state = "night";
    String temp_fanspeed = "";
    String temp_day_of_week = "";
    String message = "";

    // Actions for this sate
    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = statemachine_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = hourStr.toInt();
            temp_minute = minuteStr.toInt();
            temp_day_of_week = dayOfWeek;
            xSemaphoreGive(date_time_mutex);
        }
    }

    if (settings_state_night_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_night_mutex, (TickType_t)100) == pdTRUE)
        {
            String state_fanspeed = settings_state_night[String("state_night_fanspeed")];
            temp_fanspeed = state_fanspeed;
            xSemaphoreGive(settings_state_night_mutex);
        }
    }

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            fanspeed = temp_fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    if (lock_valve_move_mutex != NULL)
    {
        if (xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
        {
            valve_move_locked = lock_valve_move;
            xSemaphoreGive(lock_valve_move_mutex);
        }
    }

    // Read CO2 levels for transition to highCO2day state from global jsonDocument
    if (settings_state_highco2night_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)100) == pdTRUE)
        {
            co2highlevel = settings_state_highco2night["co2_high_state_highco2night"];
            xSemaphoreGive(settings_state_highco2night_mutex);
        }
    }

    // Read RH levels for transition to highrhday state from global jsonDocument
    if (settings_state_highrhnight_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highrhnight_mutex, (TickType_t)100) == pdTRUE)
        {
            rhhighlevel = settings_state_highrhnight["rh_high_state_highrhnight"];
            xSemaphoreGive(settings_state_highrhnight_mutex);
        }
    }

    message = "High CO2 detection level: " + String(co2highlevel) + ". High RH detection level: " + String(rhhighlevel);
    print_message(message);

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + " and fanspeed is " + temp_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(temp_fanspeed);
    select_sensors();

    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    { // Just in case that a reboot happened and old_time is not set
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    message = "High CO2 detection level: " + String(co2highlevel);
    print_message(message);

    message = "High RH detection level: " + String(rhhighlevel);
    print_message(message);

    if (valve_move_locked == 0)
    {
        valve_position_statemachine(statemachine_state);
    }
    else
    {
        message = "Valves are locked for moving, will try again later";
        print_message(message);
    }

    for (int i = 0; i < co2_sensor_counter; i++)
    {
        if (co2_sensors[i].co2_reading > co2highlevel)
        {
            message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has high CO2 reading. Transit to highco2night state";
            print_message(message);
            co2_sensors_high++;
        }
    }

    for (int i = 0; i < rh_sensor_counter; i++)
    {
        if (rh_sensors[i].rh_reading > rhhighlevel)
        {
            message = "Sensor" + String(i) + " which is located at " + String(rh_sensors[i].valve) + " has high RH reading. Transit to highrhnight state";
            print_message(message);
            rh_sensors_high++;
        }
    }

    message = "Number of sensors measure high CO2: " + String(co2_sensors_high) + ". Number of sensors measure high RH: " + String(rh_sensors_high);
    print_message(message);

    // Conditions to transit to other state
    if (co2_sensors_high > 0 && elapsed_time >= min_state_time)
    {
        new_state = "highco2night";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (rh_sensors_high > 0 && elapsed_time >= min_state_time)
    {
        new_state = "highrhnight";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (temp_hour >= 8 && temp_hour < 21 && temp_day_of_week != "Saturday" && temp_day_of_week != "Sunday")
    {
        message = "It is after 8, before 21 and a weekday. Transit to day.";
        print_message(message);
        new_state = "day";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (temp_hour >= 9 && temp_hour < 21 && (temp_day_of_week == "Saturday" || temp_day_of_week == "Sunday"))
    {
        message = "It is after 9, before 21 and weekend. Transit to day";
        print_message(message);
        new_state = "day";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (valve_cycle_times_night() == true)
    {
        message = "It's night and valve_cycle time. Transit to valve_cycle_night.";
        print_message(message);
        new_state = "cyclingnight";
    }
    // Manual high speed mode is ignored for now
    else
    {
        message = "Conditions have not changed, it's still night.";
        print_message(message);
        new_state = "night";
    }

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = new_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }
}

void high_co2_day_transitions(void)
{

    int temp_hour = 0;
    int temp_minute = 0;
    int co2_sensors_high = 0;
    int co2highlevel = 0;
    int co2lowlevel = 0;
    int min_state_time = 600; // minimum time in seconds the statemachine should stay in this state
    int reading = 0;
    long new_time = 0;
    bool valve_move_locked = 0;
    bool state_valve_pos_file_present = 0;

    String statemachine_state = "highco2day";
    String temp_fanspeed = "";
    String temp_day_of_week = "";
    String state_valve_pos_path = "";
    String state_valve_pos_str = "";
    String message = "";
    String valve = "";

    JsonDocument state_valve_pos_doc;

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = statemachine_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = hourStr.toInt();
            temp_minute = minuteStr.toInt();
            temp_day_of_week = dayOfWeek;
            xSemaphoreGive(date_time_mutex);
        }
    }

    if (settings_state_highco2day_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)100) == pdTRUE)
        {
            String state_fanspeed = settings_state_highco2day[String("state_highco2day_fanspeed")];
            temp_fanspeed = state_fanspeed;
            xSemaphoreGive(settings_state_highco2day_mutex);
        }
    }

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            fanspeed = temp_fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    if (lock_valve_move_mutex != NULL)
    {
        if (xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
        {
            valve_move_locked = lock_valve_move;
            xSemaphoreGive(lock_valve_move_mutex);
        }
    }

    // Read CO2 levels for transition to highCO2day state from global jsonDocument
    if (settings_state_highco2day_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)100) == pdTRUE)
        {
            co2highlevel = settings_state_highco2day["co2_high_state_highco2day"];
            co2lowlevel = settings_state_highco2day["co2_low_state_highco2day"];
            xSemaphoreGive(settings_state_highco2day_mutex);
        }
    }

    message = "High CO2 detection level: " + String(co2highlevel) + ". Low CO2 detection level: " + String(co2lowlevel);
    print_message(message);

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + " and fanspeed is " + temp_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(temp_fanspeed);
    select_sensors();

    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    { // Just in case that a reboot happened and old_time is not set
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    // Temp valve settings for individual valves starting with default settings for this state. Should read these from file and not hardcode them
    state_valve_pos_path = ("/json/settings_state_" + statemachine_state + ".json");
    state_valve_pos_file_present = check_file_exists(state_valve_pos_path.c_str());

    if (settings_state_highco2day_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)100) == pdTRUE)
        {
            if (state_valve_pos_file_present == 1)
            {
                File file = LittleFS.open(state_valve_pos_path, "r");
                while (file.available())
                {
                    state_valve_pos_str = file.readString();
                }
                file.close();
            }
            xSemaphoreGive(settings_state_highco2day_mutex);
        }
    }

    deserializeJson(state_valve_pos_doc, state_valve_pos_str);

    settings_state_temp["valve0_position_state_temp"] = state_valve_pos_doc["valve0_position_highco2day"].as<int>();
    settings_state_temp["valve1_position_state_temp"] = state_valve_pos_doc["valve1_position_highco2day"].as<int>();
    settings_state_temp["valve2_position_state_temp"] = state_valve_pos_doc["valve2_position_highco2day"].as<int>();
    settings_state_temp["valve3_position_state_temp"] = state_valve_pos_doc["valve3_position_highco2day"].as<int>();
    settings_state_temp["valve4_position_state_temp"] = state_valve_pos_doc["valve4_position_highco2day"].as<int>();
    settings_state_temp["valve5_position_state_temp"] = state_valve_pos_doc["valve5_position_highco2day"].as<int>();
    settings_state_temp["valve6_position_state_temp"] = state_valve_pos_doc["valve6_position_highco2day"].as<int>();
    settings_state_temp["valve7_position_state_temp"] = state_valve_pos_doc["valve7_position_highco2day"].as<int>();
    settings_state_temp["valve8_position_state_temp"] = state_valve_pos_doc["valve8_position_highco2day"].as<int>();
    settings_state_temp["valve9_position_state_temp"] = state_valve_pos_doc["valve9_position_highco2day"].as<int>();
    settings_state_temp["valve10_position_state_temp"] = state_valve_pos_doc["valve10_position_highco2day"].as<int>();
    settings_state_temp["valve11_position_state_temp"] = state_valve_pos_doc["valve11_position_highco2day"].as<int>();

    // Iterate through CO2 sensors to see which one has high CO2 reading to see if default settings apply when high reading is at fan inlet
    // or only one valve needs to open (when high reading is in a room)
    for (int i = 0; i < co2_sensor_counter; i++)
    {
        valve = co2_sensors[i].valve;
        reading = co2_sensors[i].co2_reading;

        if (valve != "Fan inlet")
        {
            if (reading <= co2lowlevel)
            {
                // Default valve position setting
                settings_state_temp[valve + "_position_state_temp"] = state_valve_pos_doc[valve + "_position_highco2day"].as<int>();
            }
            else if (reading > co2highlevel)
            {
                settings_state_temp[valve + "_position_state_temp"] = 20;
                message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has high CO2 reading.";
                print_message(message);
            }
            else
            {
                // The sensor value is between 900 and 1000 ppm so the valve position should remain at 20 until low co2 level is reached. The valve was set at 4 by default in above statements
                // This logic corrects the default setting of 4 to 20 and to make sure a deadband of the difference between highco2level and lowc02level is achieved
                settings_state_temp[valve + "_position_state_temp"] = 20;
                message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has CO2 reading between low and high CO2 level. Valve remain at 20 until CO2 reduces to the CO2 low level";
                print_message(message);
            }
        }

        if (reading > co2highlevel)
        {
            co2_sensors_high++;
        }
    }

    message = "Number of sensors measure high CO2: " + String(co2_sensors_high);
    print_message(message);

    if (valve_move_locked == 0)
    {
        valve_position_statemachine("state_temp");
    }
    else
    {
        message = "Valves are locked for moving, will try again later";
        print_message(message);
    }

    // Conditions for transition
    if (co2_sensors_high > 0 && temp_hour >= 21 && temp_day_of_week != "Saturday" && temp_day_of_week != "Sunday")
    {
        message = "It is before 8, after 21 and a weekday. Transit to high_co2_night.";
        print_message(message);
        new_state = "highco2night";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (co2_sensors_high > 0 && temp_hour >= 21 && (temp_day_of_week == "Saturday" || temp_day_of_week == "Sunday"))
    {
        message = "It is after 9, before 21 and weekend. Transit to high_co2_night.";
        print_message(message);
        new_state = "highco2night";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (co2_sensors_high == 0 && elapsed_time >= min_state_time)
    {
        message = "It is day, no high co2 levels. Transit to day.";
        print_message(message);
        new_state = "day";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        message = "It is day with high CO2 levels. Remain in highco2day state";
        print_message(message);
        new_state = "highco2day";
    }

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = new_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }
}

void high_co2_night_transitions(void)
{

    int temp_hour = 0;
    int temp_minute = 0;
    int co2_sensors_high = 0;
    int co2highlevel = 0;
    int co2lowlevel = 0;
    int min_state_time = 600; // minimum time in seconds the statemachine should stay in this state
    int reading = 0;
    long new_time = 0;
    bool valve_move_locked = 0;
    bool state_valve_pos_file_present = 0;

    String statemachine_state = "highco2night";
    String temp_fanspeed = "";
    String temp_day_of_week = "";
    String state_valve_pos_path = "";
    String state_valve_pos_str = "";
    String message = "";
    String valve = "";

    JsonDocument state_valve_pos_doc;

    // Actions for this state
    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = statemachine_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = hourStr.toInt();
            temp_minute = minuteStr.toInt();
            temp_day_of_week = dayOfWeek;
            xSemaphoreGive(date_time_mutex);
        }
    }

    if (settings_state_highco2night_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)100) == pdTRUE)
        {
            String state_fanspeed = settings_state_highco2night[String("state_highco2night_fanspeed")];
            temp_fanspeed = state_fanspeed;
            xSemaphoreGive(settings_state_highco2night_mutex);
        }
    }

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            fanspeed = temp_fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    // Disable valve moving when valves are already moving
    if (lock_valve_move_mutex != NULL)
    {
        if (xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
        {
            valve_move_locked = lock_valve_move;
            xSemaphoreGive(lock_valve_move_mutex);
        }
    }

    // Read CO2 levels for transition to highCO2day state from global jsonDocument
    if (settings_state_highco2night_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)100) == pdTRUE)
        {
            co2highlevel = settings_state_highco2night["co2_high_state_highco2night"].as<int>();
            co2lowlevel = settings_state_highco2night["co2_low_state_highco2night"].as<int>();
            xSemaphoreGive(settings_state_highco2night_mutex);
        }
    }

    message = "High CO2 detection level: " + String(co2highlevel) + ". Low CO2 detection level: " + String(co2lowlevel);
    print_message(message);

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + " and fanspeed is " + temp_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(temp_fanspeed);
    select_sensors();

    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    { // Just in case that a reboot happened and old_time is not set
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    // Temp valve settings for individual valves starting with default settings for this state. Should read these from file and not hardcode them
    state_valve_pos_path = ("/json/settings_state_" + statemachine_state + ".json");
    state_valve_pos_file_present = check_file_exists(state_valve_pos_path.c_str());

    if (settings_state_highco2night_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)100) == pdTRUE)
        {
            if (state_valve_pos_file_present == 1)
            {
                File file = LittleFS.open(state_valve_pos_path, "r");
                while (file.available())
                {
                    state_valve_pos_str = file.readString();
                }
                file.close();
            }
            xSemaphoreGive(settings_state_highco2night_mutex);
        }
    }

    deserializeJson(state_valve_pos_doc, state_valve_pos_str);

    settings_state_temp["valve0_position_state_temp"] = state_valve_pos_doc["valve0_position_highco2night"].as<int>();
    settings_state_temp["valve1_position_state_temp"] = state_valve_pos_doc["valve1_position_highco2night"].as<int>();
    settings_state_temp["valve2_position_state_temp"] = state_valve_pos_doc["valve2_position_highco2night"].as<int>();
    settings_state_temp["valve3_position_state_temp"] = state_valve_pos_doc["valve3_position_highco2night"].as<int>();
    settings_state_temp["valve4_position_state_temp"] = state_valve_pos_doc["valve4_position_highco2night"].as<int>();
    settings_state_temp["valve5_position_state_temp"] = state_valve_pos_doc["valve5_position_highco2night"].as<int>();
    settings_state_temp["valve6_position_state_temp"] = state_valve_pos_doc["valve6_position_highco2night"].as<int>();
    settings_state_temp["valve7_position_state_temp"] = state_valve_pos_doc["valve7_position_highco2night"].as<int>();
    settings_state_temp["valve8_position_state_temp"] = state_valve_pos_doc["valve8_position_highco2night"].as<int>();
    settings_state_temp["valve9_position_state_temp"] = state_valve_pos_doc["valve9_position_highco2night"].as<int>();
    settings_state_temp["valve10_position_state_temp"] = state_valve_pos_doc["valve10_position_highco2night"].as<int>();
    settings_state_temp["valve11_position_state_temp"] = state_valve_pos_doc["valve11_position_highco2night"].as<int>();

    // High CO2 has been detected to come into this state. Iterate through CO2 sensors to see which sensor detects high CO2. Valves with CO2 sensors are default
    // set to 24 for this state. Valves with a CO2 value lower than 900 ppm will be closed to 4 to direct airflow to the rooms with high CO2 reading.
    for (int i = 0; i < co2_sensor_counter; i++)
    {
        valve = co2_sensors[i].valve;
        reading = co2_sensors[i].co2_reading;

        if (valve != "Fan inlet")
        {
            if (reading <= co2lowlevel)
            {
                // Default valve positionsetting
                settings_state_temp[valve + "_position_state_temp"] = 4;
            }
            else if (reading > co2highlevel)
            {
                settings_state_temp[valve + "_position_state_temp"] = 20;
                message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has high CO2 reading.";
                print_message(message);
            }
            else
            {
                // The sensor value is between 900 and 1000 ppm so the valve position should remain at 4 but the valve was set at 20 by default in above statements
                // This logic corrects the default setting of 4 to 20 and to make sure a deadband of the difference between highco2level and lowc02level is achieved
                settings_state_temp[valve + "_position_state_temp"] = 4;
                message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has CO2 reading between low and high CO2 level. Valve remain at 4 until CO2 exceeds the CO2 high level";
                print_message(message);
            }
        }

        if (reading > co2highlevel)
        {
            co2_sensors_high++;
        }
    }

    message = "Number of sensors measure high CO2: " + String(co2_sensors_high);
    print_message(message);

    if (valve_move_locked == 0)
    {
        valve_position_statemachine("state_temp");
    }
    else
    {
        message = "Valves are locked for moving.";
        print_message(message);
    }

    // Conditions for transition
    if (co2_sensors_high == 0 && elapsed_time >= min_state_time)
    {
        message = "It is night, no high co2 levels. Transit to night.";
        print_message(message);
        new_state = "night";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (co2_sensors_high > 0 && temp_hour >= 8 && temp_hour < 21 && temp_day_of_week != "Saturday" && temp_day_of_week != "Sunday")
    {
        message = "It is after 8, before 21 and a weekday. Transit to high_co2_day.";
        print_message(message);
        new_state = "highco2day";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (co2_sensors_high > 0 && temp_hour >= 9 && temp_hour < 21 && (temp_day_of_week == "Saturday" || temp_day_of_week == "Sunday"))
    {
        message = "It is after 9, before 21 and weekend. Transit to high_co2_day.";
        print_message(message);
        new_state = "highco2day";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        message = "It is night with high CO2 levels. Remain in highco2night state";
        print_message(message);
        new_state = "highco2night";
    }

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = new_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }
}

void high_rh_day_transitions(void)
{

    int temp_hour = 0;
    int temp_minute = 0;
    int rh_sensors_high = 0;
    int rhlowlevel = 0;
    int min_state_time = 600;  // minimum time in seconds the statemachine should stay in this state
    int max_state_time = 1800; // maximum time in seconds the statemachine should stay in this state (high RH cannot always be solved with ventilation)
    long new_time = 0;
    bool valve_move_locked = 0;

    String statemachine_state = "highrhday";
    String temp_fanspeed = "";
    String temp_day_of_week = "";
    String message = "";

    // Actions for this sate
    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = statemachine_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = hourStr.toInt();
            temp_minute = minuteStr.toInt();
            temp_day_of_week = dayOfWeek;
            xSemaphoreGive(date_time_mutex);
        }
    }

    if (settings_state_highrhday_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highrhday_mutex, (TickType_t)100) == pdTRUE)
        {
            String state_fanspeed = settings_state_highrhday[String("state_highrhday_fanspeed")];
            temp_fanspeed = state_fanspeed;
            xSemaphoreGive(settings_state_highrhday_mutex);
        }
    }

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            fanspeed = temp_fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    if (lock_valve_move_mutex != NULL)
    {
        if (xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
        {
            valve_move_locked = lock_valve_move;
            xSemaphoreGive(lock_valve_move_mutex);
        }
    }

    // Read RH levels for transition to highrhday state from global jsonDocument
    if (settings_state_highrhday_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highrhday_mutex, (TickType_t)100) == pdTRUE)
        {
            rhlowlevel = settings_state_highrhday["rh_low_state_highrhday"];
            xSemaphoreGive(settings_state_highrhday_mutex);
        }
    }

    message = "Low RH detection level: " + String(rhlowlevel);
    print_message(message);

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + " and fanspeed is " + temp_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(temp_fanspeed);
    select_sensors();

    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    { // Just in case that a reboot happened and old_time is not set
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    if (valve_move_locked == 0)
    {
        valve_position_statemachine(statemachine_state);
    }
    else
    {
        Serial.print("\nValves are locked for moving, will try again later");
    }

    // High RH has been detected to come into this state. Iterate through RH sensors to see which sensor detects high RH. This state does not change valve positions
    for (int i = 0; i < rh_sensor_counter; i++)
    {
        if (rh_sensors[i].rh_reading > rhlowlevel)
        {
            rh_sensors_high++;
            message = "Sensor" + String(i) + " which is located at " + String(rh_sensors[i].valve) + " has high RH reading.";
            print_message(message);
        }
    }

    message = "Number of sensors measure high RH: " + String(rh_sensors_high);
    print_message(message);

    // Conditions for transition
    if ((rh_sensors_high == 0 && elapsed_time >= min_state_time) || elapsed_time >= max_state_time)
    {
        message = "It's day with no high RH or time expired. Transit to day";
        print_message(message);
        new_state = "day";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (temp_hour >= 21)
    {
        message = "It's night but RH levels are still high and time is not expired. Transit to high_rh_night";
        print_message(message);
        new_state = "highrhnight";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        message = "Conditions have not changed, RH is still high, so remain in high_rh_day state";
        print_message(message);
        new_state = "highrhday";
    }

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = new_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }
}

void high_rh_night_transitions(void)
{

    int temp_hour = 0;
    int temp_minute = 0;
    int rh_sensors_high = 0;
    int rhlowlevel = 0;
    int min_state_time = 600;  // minimum time in seconds the statemachine should stay in this state
    int max_state_time = 1800; // maximum time in seconds the statemachine should stay in this state (high RH cannot always be solved with ventilation)
    long new_time = 0;
    bool valve_move_locked = 0;

    String statemachine_state = "highrhnight";
    String temp_fanspeed = "";
    String temp_day_of_week = "";
    String message = "";

    // Actions for this state
    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = statemachine_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = hourStr.toInt();
            temp_minute = minuteStr.toInt();
            temp_day_of_week = dayOfWeek;
            xSemaphoreGive(date_time_mutex);
        }
    }

    if (settings_state_highrhnight_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highrhnight_mutex, (TickType_t)100) == pdTRUE)
        {
            String state_fanspeed = settings_state_highrhnight[String("state_highrhnight_fanspeed")];
            temp_fanspeed = state_fanspeed;
            xSemaphoreGive(settings_state_highrhnight_mutex);
        }
    }

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            fanspeed = temp_fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    if (lock_valve_move_mutex != NULL)
    {
        if (xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
        {
            valve_move_locked = lock_valve_move;
            xSemaphoreGive(lock_valve_move_mutex);
        }
    }

    // Read RH levels for transition to highrhday state from global jsonDocument
    if (settings_state_highrhnight_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highrhnight_mutex, (TickType_t)100) == pdTRUE)
        {
            rhlowlevel = settings_state_highrhnight["rh_low_state_highrhnight"];
            xSemaphoreGive(settings_state_highrhnight_mutex);
        }
    }

    message = "Low RH detection level: " + String(rhlowlevel);
    print_message(message);

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + " and fanspeed is " + temp_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(temp_fanspeed);
    select_sensors();

    // If the statemachine is till in this state after 30 mins then RH cannot be lowered with ventilation
    // No mutex is required as only statemachine uses this variable
    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    {
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    if (valve_move_locked == 0)
    {
        valve_position_statemachine(statemachine_state);
    }
    else
    {
        message = "Valves are locked for moving, will try again later";
        print_message(message);
    }

    // High RH has been detected to come into this state. Iterate through RH sensors to see which sensor detects high RH. This state does not change valve positions
    for (int i = 0; i < rh_sensor_counter; i++)
    {
        if (rh_sensors[i].rh_reading > rhlowlevel)
        {
            rh_sensors_high++;
            message = "Sensor" + String(i) + " which is located at " + String(rh_sensors[i].valve) + " has high RH reading.";
            print_message(message);
        }
    }

    message = "Number of sensors measure high RH: " + String(rh_sensors_high);
    print_message(message);

    // Conditions for transition
    if (rh_sensors_high == 0 && elapsed_time >= min_state_time || elapsed_time >= max_state_time)
    {
        message = "It's night and RH is low enough. Transit to night.";
        print_message(message);
        new_state = "night";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (temp_hour >= 8 && temp_hour < 21 && temp_day_of_week != "Saturday" && temp_day_of_week != "Sunday")
    {
        message = "It is after 8, before 21 and a weekday but RH is still high. Transit to high_rh_day.";
        print_message(message);
        new_state = "highrhday";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (temp_hour >= 9 && temp_hour < 21 && (temp_day_of_week == "Saturday" || temp_day_of_week == "Sunday"))
    {
        message = "It is after 9, before 21 and weekend but RH is still high. Transit to high_rh_day ";
        print_message(message);
        new_state = "highrhday";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        message = "Conditions have not changed, RH is still high, so remain in high_rh_night state";
        print_message(message);
        new_state = "highrhnight";
    }

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = new_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }
}

void cooking_transitions(void)
{

    int temp_hour;
    int temp_minute;
    bool valve_move_locked = 0;

    String statemachine_state = "cooking";
    String temp_fanspeed = "";
    String temp_day_of_week = "";
    String message = "";

    // Actions for this sate
    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = statemachine_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = hourStr.toInt();
            temp_minute = minuteStr.toInt();
            temp_day_of_week = dayOfWeek;
            xSemaphoreGive(date_time_mutex);
        }
    }

    // read fanspeed from config of this state
    if (settings_state_cooking_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_cooking_mutex, (TickType_t)100) == pdTRUE)
        {
            String state_fanspeed = settings_state_cooking[String("state_cooking_fanspeed")];
            temp_fanspeed = state_fanspeed;
            xSemaphoreGive(settings_state_cooking_mutex);
        }
    }

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            fanspeed = temp_fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    // Disable valve moving when valves are already moving
    if (lock_valve_move_mutex != NULL)
    {
        if (xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
        {
            valve_move_locked = lock_valve_move;
            xSemaphoreGive(lock_valve_move_mutex);
        }
    }

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + " and fanspeed is " + temp_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(temp_fanspeed);

    if (valve_move_locked == 0)
    {
        valve_position_statemachine(statemachine_state);
    }
    else
    {
        message = "Valves are locked for moving, will try again later";
    }

    // Conditions for transition
    if (cooking_times() == false)
    {
        message = "It's day and not cooking time. Transit to day";
        print_message(message);
        new_state = "day";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        message = "Conditions have not changed, cooking time is not over so remain in cooking state";
        print_message(message);
        new_state = "cooking";
    }

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = new_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }
}

void valve_cycle_day_transitions(void)
{
    int co2_sensors_high = 0;
    int rh_sensors_high = 0;
    int temp_hour = 0;
    int temp_minute = 0;
    int co2highlevel = 0;
    int rhhighlevel = 0;
    int min_state_time = 600; // minimum time in seconds the statemachine should stay in this state
    long new_time = 0;
    bool valve_move_locked = 0;

    String statemachine_state = "cyclingday";
    String temp_fanspeed = "";
    String temp_day_of_week = "";
    String message = "";

    // Actions for this state
    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = statemachine_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = hourStr.toInt();
            temp_minute = minuteStr.toInt();
            temp_day_of_week = dayOfWeek;
            xSemaphoreGive(date_time_mutex);
        }
    }

    // read fanspeed from config of this state
    if (settings_state_cyclingday_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_cyclingday_mutex, (TickType_t)100) == pdTRUE)
        {
            String state_fanspeed = settings_state_cyclingday[String("state_cyclingday_fanspeed")];
            temp_fanspeed = state_fanspeed;
            xSemaphoreGive(settings_state_cyclingday_mutex);
        }
    }

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            fanspeed = temp_fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    // Disable valve moving when valves are already moving
    if (lock_valve_move_mutex != NULL)
    {
        if (xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
        {
            valve_move_locked = lock_valve_move;
            xSemaphoreGive(lock_valve_move_mutex);
        }
    }

    // Read CO2 levels for transition to highCO2day state from global jsonDocument
    if (settings_state_highco2day_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)100) == pdTRUE)
        {
            co2highlevel = settings_state_highco2day["co2_high_state_highco2day"];
            xSemaphoreGive(settings_state_highco2day_mutex);
        }
    }

    // Read RH levels for transition to highrhday state from global jsonDocument
    if (settings_state_highrhday_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highrhday_mutex, (TickType_t)100) == pdTRUE)
        {
            rhhighlevel = settings_state_highrhday["rh_high_state_highrhday"];
            xSemaphoreGive(settings_state_highrhday_mutex);
        }
    }

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + " and fanspeed is " + temp_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(temp_fanspeed);
    select_sensors();

    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    {
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    if (valve_move_locked == 0)
    {
        valve_position_statemachine(statemachine_state);
    }
    else
    {
        Serial.print("\nValves are locked for moving, will try again later");
    }

    for (int i = 0; i < co2_sensor_counter; i++)
    {
        if (co2_sensors[i].co2_reading > co2highlevel)
        {
            message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has high CO2 reading. Transit to highco2day state";
            print_message(message);
            co2_sensors_high++;
        }
    }

    for (int i = 0; i < rh_sensor_counter; i++)
    {
        if (rh_sensors[i].rh_reading > rhhighlevel)
        {
            message = "Sensor" + String(i) + " which is located at " + String(rh_sensors[i].valve) + " has high RH reading. Transit to highrhday state";
            print_message(message);
            rh_sensors_high++;
        }
    }

    message = "Number of RH sensor with high reading: " + String(rh_sensors_high) + ". Number of CO2 sensor with high reading: " + String(co2_sensors_high);
    print_message(message);

    // Conditions for transition
    if (valve_cycle_times_day() == false)
    {
        message = "It's not valve cycle time. Transit to day";
        print_message(message);
        new_state = "day";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (rh_sensors_high > 0 && elapsed_time >= min_state_time)
    {
        message = "It's valve_cycle_day and high RH is measured. Transit to high_rh_day state.";
        print_message(message);
        new_state = "highrhday";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (co2_sensors_high > 0 && elapsed_time >= min_state_time)
    {
        message = "It is valve_cycle_day and high CO2 levels are measured. Transit to high_co2_day.";
        print_message(message);
        new_state = "highco2day";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        message = "Conditions have not changed, valve_cycle_day is still active.";
        print_message(message);
        new_state = "cyclingday";
    }

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = new_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }
}

void valve_cycle_night_transitions(void)
{

    int temp_hour = 0;
    int temp_minute = 0;
    int co2_sensors_high = 0;
    int rh_sensors_high = 0;
    int co2highlevel = 0;
    int rhhighlevel = 0;
    int min_state_time = 600; // minimum time in seconds the statemachine should stay in this state
    long new_time = 0;
    bool valve_move_locked = 0;

    String statemachine_state = "cyclingnight";
    String temp_fanspeed = "";
    String temp_day_of_week = "";
    String message = "";

    // Actions for this state
    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = statemachine_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    if (date_time_mutex != NULL)
    {
        if (xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
        {
            temp_hour = hourStr.toInt();
            temp_minute = minuteStr.toInt();
            temp_day_of_week = dayOfWeek;
            xSemaphoreGive(date_time_mutex);
        }
    }

    // read fanspeed from config of this state
    if (settings_state_cyclingnight_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_cyclingnight_mutex, (TickType_t)100) == pdTRUE)
        {
            String state_fanspeed = settings_state_cyclingnight[String("state_cyclingnight_fanspeed")];
            temp_fanspeed = state_fanspeed;
            xSemaphoreGive(settings_state_cyclingnight_mutex);
        }
    }

    if (fanspeed_mutex != NULL)
    {
        if (xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
        {
            fanspeed = temp_fanspeed;
            xSemaphoreGive(fanspeed_mutex);
        }
    }

    // Disable valve moving when valves are already moving
    if (lock_valve_move_mutex != NULL)
    {
        if (xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
        {
            valve_move_locked = lock_valve_move;
            xSemaphoreGive(lock_valve_move_mutex);
        }
    }

    // Read CO2 levels for transition to highCO2day state from global jsonDocument
    if (settings_state_highco2night_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)100) == pdTRUE)
        {
            co2highlevel = settings_state_highco2night["co2_high_state_highco2night"];
            xSemaphoreGive(settings_state_highco2night_mutex);
        }
    }

    // Read RH levels for transition to highrhday state from global jsonDocument
    if (settings_state_highrhnight_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highrhnight_mutex, (TickType_t)100) == pdTRUE)
        {
            rhhighlevel = settings_state_highrhnight["rh_high_state_highrhnight"];
            xSemaphoreGive(settings_state_highrhnight_mutex);
        }
    }

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + " and fanspeed is " + temp_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(temp_fanspeed);
    select_sensors();

    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    {
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    if (valve_move_locked == 0)
    {
        valve_position_statemachine(statemachine_state);
    }
    else
    {
        message = "Valves are locked for moving, will try again later";
        print_message(message);
    }

    for (int i = 0; i < co2_sensor_counter; i++)
    {
        if (co2_sensors[i].co2_reading > co2highlevel)
        {
            message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has high CO2 reading. Transit to highco2day state";
            print_message(message);
            co2_sensors_high++;
        }
    }

    for (int i = 0; i < rh_sensor_counter; i++)
    {
        if (rh_sensors[i].rh_reading > rhhighlevel)
        {
            message = "Sensor" + String(i) + " which is located at " + String(rh_sensors[i].valve) + " has high RH reading. Transit to highrhday state";
            print_message(message);
            rh_sensors_high++;
        }
    }

    message = "Number of RH sensor with high reading: " + String(rh_sensors_high) + ". Number of CO2 sensor with high reading: " + String(co2_sensors_high);
    print_message(message);

    // Conditions for transition
    if (valve_cycle_times_night() == false)
    {
        message = "It's not valve_cycle time. Transit to night";
        print_message(message);
        new_state = "night";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (rh_sensors_high > 0 && elapsed_time >= min_state_time)
    {
        message = "It's valve_cycle_night and high RH. Transit to high_rh_night state.";
        print_message(message);
        new_state = "highrhnight";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (co2_sensors_high > 0 && elapsed_time >= min_state_time)
    {
        message = "It is valve_cycle_night and CO2 level is high. Transit to high_co2_night";
        print_message(message);
        new_state = "highco2night";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        message = "Conditions have not changed, valve_cycle_day is still active.";
        print_message(message);
        new_state = "cyclingnight";
    }

    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = new_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }
}

// This state is for later
void manual_high_speed_transitions(void)
{

    String statemachine_state = "manual_high_speed";
    String fanspeed = "high";
    String message = "";

    bool valve_move_locked = 0;

    // Actions for this state
    if (statemachine_state_mutex != NULL)
    {
        if (xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
        {
            state = statemachine_state;
            xSemaphoreGive(statemachine_state_mutex);
        }
    }

    // Disable valve moving when valves are already moving
    if (lock_valve_move_mutex != NULL)
    {
        if (xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
        {
            valve_move_locked = lock_valve_move;
            xSemaphoreGive(lock_valve_move_mutex);
        }
    }

    set_fanspeed(fanspeed);

    if (valve_move_locked == 0)
    {
        valve_position_statemachine(statemachine_state);
    }
    else
    {
        message = "Valves are locked for moving, will try again later";
        print_message(message);
    }
}

void select_sensors(void)
{
    String sensor_config1_string = "";
    String sensor_config2_string = "";
    String co2_sensor_wire = "";
    String co2_sensor_wire1 = "";
    String rh_sensor_wire = "";
    String rh_sensor_wire1 = "";
    String valve = "";
    String message = "";

    bool sensor_config1_file_present = 0;
    bool sensor_config2_file_present = 0;

    co2_sensor_counter = 0;
    rh_sensor_counter = 0;

    int j = 0; // counter for struct
    int k = 0; // counter for struct

    float sensor_data[2][8][3];

    // Sensor config 1
    if (sensor_config_file_mutex != NULL)
    {
        if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
        {
            sensor_config1_file_present = check_file_exists(SENSOR_CONFIG1_PATH);
            if (sensor_config1_file_present == 1)
            {
                File file = LittleFS.open(SENSOR_CONFIG1_PATH, "r");
                while (file.available())
                {
                    sensor_config1_string = file.readString();
                }
                file.close();
            }
            xSemaphoreGive(sensor_config_file_mutex);
        }
    }
    if (sensor_config1_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return;
    }
    else
    {
        DeserializationError err1 = deserializeJson(wire_sensor_data, sensor_config1_string);
        if (err1)
        {
            message = message = "[ERROR] Failed to parse: " + String(SENSOR_CONFIG1_PATH) + " with error: " + String(err1.c_str());
            print_message(message);
            return;
        }
    }

    // Sensor config 2
    if (sensor_config_file_mutex != NULL)
    {
        if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
        {
            sensor_config2_file_present = check_file_exists(SENSOR_CONFIG2_PATH);
            if (sensor_config2_file_present == 1)
            {
                File file = LittleFS.open(SENSOR_CONFIG2_PATH, "r");
                while (file.available())
                {
                    sensor_config2_string = file.readString();
                }
                file.close();
            }
            xSemaphoreGive(sensor_config_file_mutex);
        }
    }
    if (sensor_config2_string == "")
    {
        message = "[ERROR] String is empty or failed to read file";
        print_message(message);
        return;
    }
    else
    {
        DeserializationError err2 = deserializeJson(wire1_sensor_data, sensor_config2_string);
        if (err2)
        {
            message = message = "[ERROR] Failed to parse: " + String(SENSOR_CONFIG2_PATH) + " with error: " + String(err2.c_str());
            print_message(message);
            return;
        }
    }

    // Copy sensor readings from global
    xQueuePeek(sensor_avg_queue, &sensor_data, 0);

    for (int i = 0; i < 16; i++)
    {
        if (sensor_config_file_mutex != NULL)
        {
            if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
            {
                String co2_sensor_wire_temp = wire_sensor_data["wire_sensor" + String(i)]["co2"];
                String co2_sensor_wire1_temp = wire1_sensor_data["wire1_sensor" + String(i)]["co2"];
                String rh_sensor_wire_temp = wire_sensor_data["wire_sensor" + String(i)]["rh"];
                String rh_sensor_wire1_temp = wire1_sensor_data["wire1_sensor" + String(i)]["rh"];
                co2_sensor_wire = co2_sensor_wire_temp;
                co2_sensor_wire1 = co2_sensor_wire1_temp;
                rh_sensor_wire = rh_sensor_wire_temp;
                rh_sensor_wire1 = rh_sensor_wire1_temp;
                xSemaphoreGive(sensor_config_file_mutex);
            }
        }
        if (co2_sensor_wire == "On")
        {
            if (sensor_config_file_mutex != NULL)
            {
                if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
                {
                    String valve_temp = wire_sensor_data["wire_sensor" + String(i)]["valve"];
                    valve = valve_temp;
                    xSemaphoreGive(sensor_config_file_mutex);
                }
            }

            co2_sensors[j].valve = valve;
            co2_sensors[j].co2_reading = sensor_data[0][i][2];

            message = "valve: " + String(co2_sensors[j].valve) + ", CO2 reading: " + String(co2_sensors[j].co2_reading);
            print_message(message);
            j++;
        }

        if (co2_sensor_wire1 == "On")
        {
            if (sensor_config_file_mutex != NULL)
            {
                if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
                {
                    String valve_temp = wire1_sensor_data["wire1_sensor" + String(i)]["valve"];
                    valve = valve_temp;
                    xSemaphoreGive(sensor_config_file_mutex);
                }
            }
            co2_sensors[j].valve = valve;
            co2_sensors[j].co2_reading = sensor_data[1][i][2];

            message = "valve: " + String(co2_sensors[j].valve) + ", CO2 reading: " + String(co2_sensors[j].co2_reading);
            print_message(message);
            j++;
        }

        if (rh_sensor_wire == "On")
        {
            if (sensor_config_file_mutex != NULL)
            {
                if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
                {
                    String valve_temp = wire_sensor_data["wire_sensor" + String(i)]["valve"];
                    valve = valve_temp;
                    xSemaphoreGive(sensor_config_file_mutex);
                }
            }
            rh_sensors[k].valve = valve;
            rh_sensors[k].rh_reading = sensor_data[0][i][1];

            message = "valve: " + String(rh_sensors[k].valve) + ", RH reading: " + String(rh_sensors[k].rh_reading);
            print_message(message);
            k++;
        }

        if (rh_sensor_wire1 == "On")
        {
            if (sensor_config_file_mutex != NULL)
            {
                if (xSemaphoreTake(sensor_config_file_mutex, (TickType_t)100) == pdTRUE)
                {
                    String valve_temp = wire1_sensor_data["wire1_sensor" + String(i)]["valve"];
                    valve = valve_temp;
                    xSemaphoreGive(sensor_config_file_mutex);
                }
            }
            rh_sensors[k].valve = valve;
            rh_sensors[k].rh_reading = sensor_data[1][i][1];

            message = "valve: " + String(rh_sensors[k].valve) + ", RH reading: " + String(rh_sensors[k].rh_reading);
            print_message(message);
            k++;
        }
    }

    co2_sensor_counter = j;
    rh_sensor_counter = k;
}
