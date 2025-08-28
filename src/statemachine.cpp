/* Statemachine code */

#include "statemachine.h"

float statemachine_sensor_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];
float statemachine_avg_sensor_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];

int co2_sensor_counter = 0;
int rh_sensor_counter = 0;

long old_time = 0;
long elapsed_time = 0;

String new_state = "";
String temp_fanspeed = "Low";

struct CO2_Sensors
{
    char *valve;
    float co2_reading;
};
struct RH_Sensors
{
    char *valve;
    float rh_reading;
};

CO2_Sensors co2_sensors[MAX_SENSORS];
RH_Sensors rh_sensors[MAX_SENSORS];

void init_statemachine(void)
{
    String temp_fanspeed = "";

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state = "init";
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        fanspeed = temp_fanspeed;
        xSemaphoreGive(fanspeed_mutex);
    }

    set_fanspeed(temp_fanspeed);
}

void run_statemachine(void)
{
    String message = "";
    String temp_state = "";

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_state = state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    message = "Read sensor data from queue for statemachine.";
    print_message(message);

    if (xQueuePeek(sensor_queue, &statemachine_sensor_data, 0) != pdTRUE)
    {
        message = "Faild to read from sensor_queue in function: " + String(__FUNCTION__);
        print_message(message);
        return;
    }

    message = "Read average sensor data from queue for statemachine.";
    print_message(message);

    if (xQueuePeek(sensor_avg_queue, &statemachine_avg_sensor_data, 0) != pdTRUE)
    {
        message = "Faild to read from sensor_avg_queue in function: " + String(__FUNCTION__);
        print_message(message);
        return;
    }

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
    String state_fanspeed = "Low";
    String temp_day_of_week = "";
    String message = "";

    // Actions for this state

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state = statemachine_state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_hour = hourStr.toInt();
        temp_minute = minuteStr.toInt();
        temp_day_of_week = dayOfWeek;
        xSemaphoreGive(date_time_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        fanspeed = state_fanspeed;
        xSemaphoreGive(fanspeed_mutex);
    }

    message = "Statemachine in state " + statemachine_state + ". It is " + temp_hour + ":" + temp_minute + " and day of week is " + temp_day_of_week + ", fanspeed is " + state_fanspeed;
    print_message(message);

    set_fanspeed(state_fanspeed);

    if (is_day() == true)
    {
        message = "It is day. Transit to day.";
        print_message(message);
        new_state = "day";
    }
    else
    {
        message = "It is night. Transit to night.";
        print_message(message);
        new_state = "night";
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state = new_state;
        xSemaphoreGive(statemachine_state_mutex);
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
    long new_time = 0;
    bool valve_move_locked = 0;

    // Statemachine settings
    int minimum_state_time = 0;

    String state_fanspeed = "";
    String statemachine_state = "day";
    String temp_day_of_week = "";
    String message = "";

    // Actions for this state

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state = statemachine_state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_hour = hourStr.toInt();
        temp_minute = minuteStr.toInt();
        temp_day_of_week = dayOfWeek;
        xSemaphoreGive(date_time_mutex);
    }

    if (settings_state_day_mutex && xSemaphoreTake(settings_state_day_mutex, (TickType_t)10) == pdTRUE)
    {
        state_fanspeed = String(statedaysettings.state_day_fanspeed);
        co2highlevel = statedaysettings.state_day_highco2;
        rhhighlevel = statedaysettings.state_day_highrh;
        xSemaphoreGive(settings_state_day_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        fanspeed = state_fanspeed;
        xSemaphoreGive(fanspeed_mutex);
    }

    if (lock_valve_move_mutex && xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
    {
        valve_move_locked = lock_valve_move;
        xSemaphoreGive(lock_valve_move_mutex);
    }

    if (settings_statemachine_mutex && xSemaphoreTake(settings_statemachine_mutex, (TickType_t)10))
    {
        minimum_state_time = statemachinesettings.minimum_state_time;
        xSemaphoreGive(settings_statemachine_mutex);
    }

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + ", fanspeed is " + state_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(state_fanspeed);
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

    if (co2_sensors_high > 0 && elapsed_time >= minimum_state_time)
    {
        new_state = "highco2day";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (rh_sensors_high > 0 && elapsed_time >= minimum_state_time)
    {
        new_state = "highrhday";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (is_day() == false)
    {
        message = "It is night time. Transit to night.";
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

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state = new_state;
        xSemaphoreGive(statemachine_state_mutex);
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
    long new_time = 0;
    bool valve_move_locked = 0;

    // Statemachine settings
    int minimum_state_time = 0;

    String statemachine_state = "night";
    String state_fanspeed = "";
    String temp_day_of_week = "";
    String message = "";

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state = statemachine_state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_hour = hourStr.toInt();
        temp_minute = minuteStr.toInt();
        temp_day_of_week = dayOfWeek;
        xSemaphoreGive(date_time_mutex);
    }

    if (settings_state_night_mutex && xSemaphoreTake(settings_state_night_mutex, (TickType_t)10) == pdTRUE)
    {
        state_fanspeed = String(statenightsettings.state_night_fanspeed);
        co2highlevel = statenightsettings.state_night_highco2;
        rhhighlevel = statenightsettings.state_night_highrh;
        xSemaphoreGive(settings_state_night_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        fanspeed = state_fanspeed;
        xSemaphoreGive(fanspeed_mutex);
    }

    if (lock_valve_move_mutex && xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
    {
        valve_move_locked = lock_valve_move;
        xSemaphoreGive(lock_valve_move_mutex);
    }

    if (settings_statemachine_mutex && xSemaphoreTake(settings_statemachine_mutex, (TickType_t)10))
    {
        minimum_state_time = statemachinesettings.minimum_state_time;
        xSemaphoreGive(settings_statemachine_mutex);
    }

    message = "High CO2 detection level: " + String(co2highlevel) + ". High RH detection level: " + String(rhhighlevel);
    print_message(message);

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + " and fanspeed is " + state_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(state_fanspeed);
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
    if (co2_sensors_high > 0 && elapsed_time >= minimum_state_time)
    {
        new_state = "highco2night";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (rh_sensors_high > 0 && elapsed_time >= minimum_state_time)
    {
        new_state = "highrhnight";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (is_day() == true)
    {
        message = "It is day. Transit to day.";
        print_message(message);
        new_state = "day";
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

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state = new_state;
        xSemaphoreGive(statemachine_state_mutex);
    }
}

void high_co2_day_transitions(void)
{
    int temp_hour = 0;
    int temp_minute = 0;
    int co2_sensors_high = 0;
    int co2highlevel = 0;
    int co2lowlevel = 0;
    int reading = 0;
    int valve_nr = 0;
    long new_time = 0;
    bool valve_move_locked = 0;

    // Statemachine settings
    /*int weekday_day_hour_start = 0;
    int weekday_day_minute_start = 0;
    int weekday_night_hour_start = 0;
    int weekday_night_minute_start = 0;
    int weekend_day_hour_start = 0;
    int weekend_day_minute_start = 0;
    int weekend_night_hour_start = 0;
    int weekend_night_minute_start = 0;
    char *weekend_day_1 = NULL;
    char *weekend_day_2 = NULL;*/
    int minimum_state_time = 0;

    int state_valve_position[12] = {0}; // Array for valvepositions from statemachine settings

    String statemachine_state = "highco2day";
    String state_fanspeed = "";
    String temp_day_of_week = "";
    String state_valve_pos_path = "";
    String state_valve_pos_str = "";
    String message = "";
    String valve = "";

    // JsonDocument state_valve_pos_doc;

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state = statemachine_state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_hour = hourStr.toInt();
        temp_minute = minuteStr.toInt();
        temp_day_of_week = dayOfWeek;
        xSemaphoreGive(date_time_mutex);
    }

    if (settings_state_highco2day_mutex && xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)100) == pdTRUE)
    {
        state_fanspeed = String(statehighco2daysettings.state_highco2day_fanspeed);
        co2highlevel = statehighco2daysettings.co2_low_state_highco2day;
        co2lowlevel = statehighco2daysettings.co2_high_state_highco2day;
        xSemaphoreGive(settings_state_highco2day_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        fanspeed = state_fanspeed;
        xSemaphoreGive(fanspeed_mutex);
    }

    if (lock_valve_move_mutex && xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
    {
        valve_move_locked = lock_valve_move;
        xSemaphoreGive(lock_valve_move_mutex);
    }

    if (settings_statemachine_mutex && xSemaphoreTake(settings_statemachine_mutex, (TickType_t)10))
    {
        minimum_state_time = statemachinesettings.minimum_state_time;
        xSemaphoreGive(settings_statemachine_mutex);
    }

    message = "High CO2 detection level: " + String(co2highlevel) + ". Low CO2 detection level: " + String(co2lowlevel);
    print_message(message);

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + " and fanspeed is " + state_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(state_fanspeed);
    select_sensors();

    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    { // Just in case that a reboot happened and old_time is not set
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    // Temp valve settings for individual valves starting with default settings for this state. Should read these from file and not hardcode them
    if (settings_state_highco2day_mutex && xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)10) == pdTRUE)
    {
        state_valve_position[0] = statehighco2daysettings.valve0_position_highco2day;
        state_valve_position[1] = statehighco2daysettings.valve1_position_highco2day;
        state_valve_position[2] = statehighco2daysettings.valve2_position_highco2day;
        state_valve_position[3] = statehighco2daysettings.valve3_position_highco2day;
        state_valve_position[4] = statehighco2daysettings.valve4_position_highco2day;
        state_valve_position[5] = statehighco2daysettings.valve5_position_highco2day;
        state_valve_position[6] = statehighco2daysettings.valve6_position_highco2day;
        state_valve_position[7] = statehighco2daysettings.valve7_position_highco2day;
        state_valve_position[8] = statehighco2daysettings.valve8_position_highco2day;
        state_valve_position[9] = statehighco2daysettings.valve9_position_highco2day;
        state_valve_position[10] = statehighco2daysettings.valve10_position_highco2day;
        state_valve_position[11] = statehighco2daysettings.valve11_position_highco2day;
        xSemaphoreGive(settings_state_highco2day_mutex);
    }

    // Iterate through CO2 sensors to see which one has high CO2 reading to see if default settings apply when high reading is at fan inlet
    // or only one valve needs to open (when high reading is in a room)
    for (int i = 0; i < co2_sensor_counter; i++)
    {
        valve = co2_sensors[i].valve;
        valve.replace("valve", "");
        valve_nr = valve.toInt();
        reading = co2_sensors[i].co2_reading;

        if (valve != "Fan inlet")
        {
            if (reading >= co2highlevel)
            {
                // settings_state_temp[valve + "_position_state_temp"] = 20;
                state_valve_position[valve_nr] = 20;
                message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has a CO2 reading higher than highco2level.";
                print_message(message);
            }
            else if (reading >= co2lowlevel && reading < co2highlevel)
            {
                // The sensor value is between 900 and 1000 ppm so the valve position should remain at 20 until low co2 level is reached. The valve was set at 4 by default in above statements
                // This logic corrects the default setting of 4 to 20 and to make sure a deadband of the difference between highco2level and lowc02level is achieved
                // settings_state_temp[valve + "_position_state_temp"] = 20;
                state_valve_position[valve_nr] = 20;
                message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has CO2 reading between low and high CO2 level. Valve remain at 20 until CO2 reduces to the CO2 low level";
                print_message(message);
            }
            else
            {
                // Do nothing because valve is already back to default position according to sate
                message = "No sensor has high CO2 reading. Valve set to default state position";
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

    // Copy values to temp settings
    if (settings_state_temp_mutex && xSemaphoreTake(settings_state_temp_mutex, (TickType_t)10) == pdTRUE)
    {
        statetempsettings.valve0_position_temp = state_valve_position[0];
        statetempsettings.valve1_position_temp = state_valve_position[1];
        statetempsettings.valve2_position_temp = state_valve_position[2];
        statetempsettings.valve3_position_temp = state_valve_position[3];
        statetempsettings.valve4_position_temp = state_valve_position[4];
        statetempsettings.valve5_position_temp = state_valve_position[5];
        statetempsettings.valve6_position_temp = state_valve_position[6];
        statetempsettings.valve7_position_temp = state_valve_position[7];
        statetempsettings.valve8_position_temp = state_valve_position[8];
        statetempsettings.valve9_position_temp = state_valve_position[9];
        statetempsettings.valve10_position_temp = state_valve_position[10];
        statetempsettings.valve11_position_temp = state_valve_position[11];
        xSemaphoreGive(settings_state_temp_mutex);
    }

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
    /*if (co2_sensors_high > 0 && temp_hour >= weekday_night_hour_start && temp_minute >= weekday_night_minute_start && temp_day_of_week != "Saturday" && temp_day_of_week != "Sunday")
    {
        message = "It is before 8, after 21 and a weekday. Transit to high_co2_night.";
        print_message(message);
        new_state = "highco2night";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (co2_sensors_high > 0 && temp_hour >= weekend_night_hour_start && weekend_night_minute_start && (temp_day_of_week == String(weekend_day_1) || temp_day_of_week == String(weekend_day_2)))
    {
        message = "It is after 9, before 21 and weekend. Transit to high_co2_night.";
        print_message(message);
        new_state = "highco2night";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }*/

    if (is_day() == true && co2_sensors_high == 0 && elapsed_time >= minimum_state_time)
    {
        message = "It is day, no high co2 levels. Transit to day.";
        print_message(message);
        new_state = "day";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (is_day() == false && co2_sensors_high > 0 && elapsed_time >= minimum_state_time)
    {
        message = "It is night with high co2 levels. Transit to highco2night.";
        print_message(message);
        new_state = "highco2night";
        elapsed_time = 0;
        old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        message = "It is still day with high CO2 levels. Remain in highco2day state";
        print_message(message);
        new_state = "highco2day";
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state = new_state;
        xSemaphoreGive(statemachine_state_mutex);
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
    int minimum_state_time = 0;
    int valve_nr = 0;

    int state_valve_position[12] = {0}; // Array for valvepositions from statemachine settings
    int valve_position_temp[12] = {0};  // Array for temporary valvepositions

    String statemachine_state = "highco2night";
    String state_fanspeed = "";
    String temp_day_of_week = "";
    String state_valve_pos_path = "";
    String state_valve_pos_str = "";
    String message = "";
    String valve = "";

    JsonDocument state_valve_pos_doc;

    // Actions for this state
    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state = statemachine_state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (date_time_mutex && xSemaphoreTake(date_time_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_hour = hourStr.toInt();
        temp_minute = minuteStr.toInt();
        temp_day_of_week = dayOfWeek;
        xSemaphoreGive(date_time_mutex);
    }

    if (settings_state_highco2night_mutex && xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)10) == pdTRUE)
    {
        state_fanspeed = String(statehighco2nightsettings.state_highco2night_fanspeed);
        co2highlevel = statehighco2nightsettings.co2_low_state_highco2night;
        co2lowlevel = statehighco2nightsettings.co2_high_state_highco2night;
        xSemaphoreGive(settings_state_highco2night_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        fanspeed = temp_fanspeed;
        xSemaphoreGive(fanspeed_mutex);
    }

    // Disable valve moving when valves are already moving
    if (lock_valve_move_mutex && xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
    {
        valve_move_locked = lock_valve_move;
        xSemaphoreGive(lock_valve_move_mutex);
    }

    if (settings_statemachine_mutex && xSemaphoreTake(settings_statemachine_mutex, (TickType_t)10))
    {
        minimum_state_time = statemachinesettings.minimum_state_time;
        xSemaphoreGive(settings_statemachine_mutex);
    }

    message = "High CO2 detection level: " + String(co2highlevel) + ". Low CO2 detection level: " + String(co2lowlevel);
    print_message(message);

    message = "Statemachine in state " + statemachine_state + ", it is " + temp_day_of_week + " " + temp_hour + ":" + temp_minute + " and fanspeed is " + state_fanspeed + ", elapsed time: " + String(elapsed_time);
    print_message(message);

    set_fanspeed(temp_fanspeed);
    select_sensors();

    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    { // Just in case that a reboot happened and old_time is not set
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    if (settings_state_highco2night_mutex && xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)10) == pdTRUE)
    {
        state_valve_position[0] = statehighco2nightsettings.valve0_position_highco2night;
        state_valve_position[1] = statehighco2nightsettings.valve1_position_highco2night;
        state_valve_position[2] = statehighco2nightsettings.valve2_position_highco2night;
        state_valve_position[3] = statehighco2nightsettings.valve3_position_highco2night;
        state_valve_position[4] = statehighco2nightsettings.valve4_position_highco2night;
        state_valve_position[5] = statehighco2nightsettings.valve5_position_highco2night;
        state_valve_position[6] = statehighco2nightsettings.valve6_position_highco2night;
        state_valve_position[7] = statehighco2nightsettings.valve7_position_highco2night;
        state_valve_position[8] = statehighco2nightsettings.valve8_position_highco2night;
        state_valve_position[9] = statehighco2nightsettings.valve9_position_highco2night;
        state_valve_position[10] = statehighco2nightsettings.valve10_position_highco2night;
        state_valve_position[11] = statehighco2nightsettings.valve11_position_highco2night;
        xSemaphoreGive(settings_state_highco2night_mutex);
    }

    // High CO2 has been detected to come into this state. Iterate through CO2 sensors to see which sensor detects high CO2. Valves with CO2 sensors are default
    // set to 20 for this state. Valves with a CO2 value lower than 900 ppm will be closed to 4 to direct airflow to the rooms with high CO2 reading.
    for (int i = 0; i < co2_sensor_counter; i++)
    {
        valve = co2_sensors[i].valve;
        valve.replace("valve", "");
        valve_nr = valve.toInt();
        reading = co2_sensors[i].co2_reading;

        if (valve != "Fan inlet")
        {
            if (reading <= co2lowlevel)
            {
                // CO2 is low so close the valve re-route the air to valves with high CO2
                state_valve_position[valve_nr] = 4;
                message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has CO2 reading low than CO2lowlevel. Valve remain at 4 until CO2 exceeds the CO2lowlevel";
            }
            else if (reading > co2lowlevel && reading < co2highlevel)
            {
                // The sensor value is between 900 and 1000 ppm so the valve position should remain at 4 until low co2 level is reached. The valve was set at 20 by default.
                // This logic corrects the default setting of 4 to 20 and to make sure a deadband of the difference between highco2level and lowc02level is achieved
                state_valve_position[valve_nr] = 4;
                message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has CO2 reading between low and high CO2 level. Valve remain at 20 until CO2 reduces to the CO2 low level";
                print_message(message);
            }
            else
            {
                // The sensor value is above co2highlevel so default valve setting applies
                message = "Sensor" + String(i) + " which is located at " + String(co2_sensors[i].valve) + " has CO2 reading higher than co2highlevel. Default valve setting applies";
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

    // Copy values to temp settings
    if (settings_state_temp_mutex && xSemaphoreTake(settings_state_temp_mutex, (TickType_t)10) == pdTRUE)
    {
        statetempsettings.valve0_position_temp = state_valve_position[0];
        statetempsettings.valve1_position_temp = state_valve_position[1];
        statetempsettings.valve2_position_temp = state_valve_position[2];
        statetempsettings.valve3_position_temp = state_valve_position[3];
        statetempsettings.valve4_position_temp = state_valve_position[4];
        statetempsettings.valve5_position_temp = state_valve_position[5];
        statetempsettings.valve6_position_temp = state_valve_position[6];
        statetempsettings.valve7_position_temp = state_valve_position[7];
        statetempsettings.valve8_position_temp = state_valve_position[8];
        statetempsettings.valve9_position_temp = state_valve_position[9];
        statetempsettings.valve10_position_temp = state_valve_position[10];
        statetempsettings.valve11_position_temp = state_valve_position[11];
        xSemaphoreGive(settings_state_temp_mutex);
    }

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
    /*if (settings_state_highco2day_mutex != NULL)
    {
        if (xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)100) == pdTRUE)
        {
            co2highlevel = settings_state_highco2day["co2_high_state_highco2day"];
            xSemaphoreGive(settings_state_highco2day_mutex);
        }
    }*/

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

    // Example:
    // char msg[128];
    // snprintf(msg, sizeof(msg), "valve: %s, CO2 reading: %.2f", valve_1, co2_sensors[co2_index].co2_reading);
    // print_message(msg);

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
    char *co2_sensor_wire = NULL;
    char *co2_sensor_wire1 = NULL;
    char *rh_sensor_wire = NULL;
    char *rh_sensor_wire1 = NULL;
    char *valve_wire = NULL;
    char *valve_wire1 = NULL;

    String message = "";

    co2_sensor_counter = 0;
    rh_sensor_counter = 0;

    int j = 0; // counter for struct
    int k = 0; // counter for struct

    float sensor_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];

    // Copy sensor readings from global
    if (xQueuePeek(sensor_avg_queue, &sensor_data, 0) != pdTRUE)
    {
        message = "Faild to read from sensor_avg_queue in function: " + String(__FUNCTION__);
        return;
    }

    for (int i = 0; i < SENSOR_COUNT; i++)
    {

        if (settings_sensor1_mutex && xSemaphoreTake(settings_sensor1_mutex, (TickType_t)10))
        {
            co2_sensor_wire = sensor1settings[i].wire_sensor_co2;
            rh_sensor_wire = sensor1settings[i].wire_sensor_rh;
            valve_wire = sensor1settings[i].wire_sensor_valve;
            xSemaphoreGive(settings_sensor1_mutex);
        }

        if (settings_sensor2_mutex && xSemaphoreTake(settings_sensor2_mutex, (TickType_t)10))
        {
            co2_sensor_wire1 = sensor2settings[i].wire1_sensor_co2;
            rh_sensor_wire1 = sensor2settings[i].wire1_sensor_rh;
            valve_wire1 = sensor2settings[i].wire1_sensor_valve;
            xSemaphoreGive(settings_sensor2_mutex);
        }

        if (co2_sensor_wire && strcmp(co2_sensor_wire, "On") == 0)
        {
            co2_sensors[j].valve = valve_wire;
            co2_sensors[j].co2_reading = sensor_data[0][i][2];

            message = "valve: " + String(co2_sensors[j].valve) + ", CO2 reading: " + String(co2_sensors[j].co2_reading);
            print_message(message);
            j++;
        }

        if (co2_sensor_wire1 && strcmp(co2_sensor_wire1, "On") == 0)
        {
            co2_sensors[j].valve = valve_wire1;
            co2_sensors[j].co2_reading = sensor_data[1][i][2];

            message = "valve: " + String(co2_sensors[j].valve) + ", CO2 reading: " + String(co2_sensors[j].co2_reading);
            print_message(message);
            j++;
        }

        if (rh_sensor_wire && strcmp(rh_sensor_wire, "On") == 0)
        {
            rh_sensors[k].valve = valve_wire;
            rh_sensors[k].rh_reading = sensor_data[0][i][1];

            message = "valve: " + String(rh_sensors[k].valve) + ", RH reading: " + String(rh_sensors[k].rh_reading);
            print_message(message);
            k++;
        }

        if (rh_sensor_wire1 && strcmp(rh_sensor_wire1, "On") == 0)
        {
            rh_sensors[k].valve = valve_wire1;
            rh_sensors[k].rh_reading = sensor_data[1][i][1];

            message = "valve: " + String(rh_sensors[k].valve) + ", RH reading: " + String(rh_sensors[k].rh_reading);
            print_message(message);
            k++;
        }
    }

    co2_sensor_counter = j;
    rh_sensor_counter = k;
}
