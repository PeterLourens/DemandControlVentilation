/* Statemachine code */
#include "statemachine.h"

float statemachine_sensor_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];
float statemachine_avg_sensor_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];

int co2_sensor_counter = 0;
int rh_sensor_counter = 0;
int co2_sensors_high = 0;
int rh_sensors_high = 0;
int old_time = 0;
int elapsed_time = 0;

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
    char temp_fanspeed[MEDIUM_CONFIG_ITEM] = "Low";

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(state, "init", sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(fanspeed, "Low", sizeof(fanspeed));
        fanspeed[sizeof(fanspeed) - 1] = '\0';
        xSemaphoreGive(fanspeed_mutex);
    }

    set_fanspeed(temp_fanspeed);
}

void run_statemachine(void)
{
    char msg[MSG_SIZE] = {};

    char *temp_state = NULL;

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_state = state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    snprintf(msg, sizeof(msg), "Read sensor data from queue for statemachine.");
    printmessage(LOG_INFO, msg);

    if (xQueuePeek(sensor_queue, &statemachine_sensor_data, 0) != pdTRUE)
    {
        snprintf(msg, sizeof(msg), "Failed to read from sensor_queue.");
        printmessage(LOG_ERROR, msg);
        return;
    }

    snprintf(msg, sizeof(msg), "Read average sensor data from queue for statemachine.");
    printmessage(LOG_INFO, msg);

    if (xQueuePeek(sensor_avg_queue, &statemachine_avg_sensor_data, 0) != pdTRUE)
    {
        snprintf(msg, sizeof(msg), "Failed to read from sensor_avg_queue.");
        printmessage(LOG_ERROR, msg);
        return;
    }

    if (strcmp(temp_state, "init") == 0)
    {
        init_transitions();
    }
    else if (strcmp(temp_state, "stopped") == 0)
    {
        stopped_transitions();
    }
    else if (strcmp(temp_state, "day") == 0)
    {
        day_transitions();
    }
    else if (strcmp(temp_state, "night") == 0)
    {
        night_transitions();
    }
    else if (strcmp(temp_state, "highco2day") == 0)
    {
        high_co2_day_transitions();
    }
    else if (strcmp(temp_state, "highco2night") == 0)
    {
        high_co2_night_transitions();
    }
    else if (strcmp(temp_state, "highrhday") == 0)
    {
        high_rh_day_transitions();
    }
    else if (strcmp(temp_state, "highrhnight") == 0)
    {
        high_rh_night_transitions();
    }
    else if (strcmp(temp_state, "cooking") == 0)
    {
        cooking_transitions();
    }
    else if (strcmp(temp_state, "cyclingday") == 0)
    {
        valve_cycle_day_transitions();
    }
    else if (strcmp(temp_state, "cyclingnight") == 0)
    {
        valve_cycle_night_transitions();
    }
    else if (strcmp(temp_state, "fanhighspeed") == 0)
    {
        manual_high_speed_transitions();
    }
    else
    {
        // This state should normally never be entered. Back to init of statemachine to keep it running
        snprintf(msg, sizeof(msg), "Error in state number, back to init state.");
        printmessage(LOG_ERROR, msg);
        init_transitions();
    }
}

void stopped_transitions(void)
{
    char msg[MSG_SIZE] = {};

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(state, "stopped", sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    // No further logic required because when start statemachine button is pushed the statemachine will go back to init state
    snprintf(msg, sizeof(msg), "State is stopped.");
    printmessage(LOG_INFO, msg);
    sniprintf(msg, sizeof(msg), "Push start statemachine button on the Valve Control web page to continue.");
    printmessage(LOG_INFO, msg);
}

void init_transitions(void)
{
    char msg[MSG_SIZE] = {};
    char state_fanspeed[MEDIUM_CONFIG_ITEM] = "Low";
    char new_state[MEDIUM_CONFIG_ITEM] = {};

    // Actions for this state
    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(state, "init", sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(fanspeed, state_fanspeed, sizeof(fanspeed));
        fanspeed[sizeof(fanspeed) - 1] = '\0';
        xSemaphoreGive(fanspeed_mutex);
    }

    snprintf(msg, sizeof(msg), "State: init, fanspeed: %s.", state_fanspeed);
    printmessage(LOG_INFO, msg);

    set_fanspeed(state_fanspeed);

    if (is_day() == true)
    {
        snprintf(msg, sizeof(msg), "Transit to day.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "day", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }
    else
    {
        snprintf(msg, sizeof(msg), "Transit to night.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "night", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(state, new_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }
}

void day_transitions(void)
{
    int rhhighlevel = 0;
    int co2highlevel = 0;
    int minimum_state_time = 0;
    int new_time = 0;
    bool valve_move_locked = 0;

    char msg[MSG_SIZE] = {};
    char new_state[MEDIUM_CONFIG_ITEM] = {};

    char *state_fanspeed = NULL;
    char *statemachine_state = NULL;

    co2_sensors_high = 0;
    rh_sensors_high = 0;

    // Actions for this state
    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        statemachine_state = state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (settings_state_day_mutex && xSemaphoreTake(settings_state_day_mutex, (TickType_t)10) == pdTRUE)
    {
        state_fanspeed = statedaysettings.state_day_fanspeed;
        co2highlevel = statedaysettings.state_day_highco2;
        rhhighlevel = statedaysettings.state_day_highrh;
        xSemaphoreGive(settings_state_day_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(fanspeed, state_fanspeed, sizeof(fanspeed));
        fanspeed[sizeof(fanspeed) - 1] = '\0';
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

    snprintf(msg, sizeof(msg), "State: %s, fanspeed: %s, state_time: %d.", statemachine_state, state_fanspeed, elapsed_time);
    printmessage(LOG_INFO, msg);

    set_fanspeed(state_fanspeed);
    select_sensors();

    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    {
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    snprintf(msg, sizeof(msg), "High CO2 level: %d", co2highlevel);
    printmessage(LOG_INFO, msg);

    snprintf(msg, sizeof(msg), "High RH level: %d", rhhighlevel);
    printmessage(LOG_INFO, msg);

    if (valve_move_locked == 0)
    {
        valve_position_statemachine(statemachine_state);
    }
    else
    {
        snprintf(msg, sizeof(msg), "Valves are locked for moving.");
        printmessage(LOG_INFO, msg);
    }

    for (int i = 0; i < co2_sensor_counter; i++)
    {
        if (co2_sensors[i].co2_reading > co2highlevel)
        {
            snprintf(msg, sizeof(msg), "Sensor located at %s has high CO2.", co2_sensors[i].valve);
            printmessage(LOG_INFO, msg);
            snprintf(msg, sizeof(msg), "Transit to highco2day if minimum state time has expired.");
            printmessage(LOG_INFO, msg);
            co2_sensors_high++;
        }
    }

    for (int i = 0; i < rh_sensor_counter; i++)
    {
        if (rh_sensors[i].rh_reading > rhhighlevel)
        {
            snprintf(msg, sizeof(msg), "Sensor located at %s has high RH.", rh_sensors[i].valve);
            printmessage(LOG_INFO, msg);
            snprintf(msg, sizeof(msg), "Transit to highrhday if minimum state time has expired.");
            printmessage(LOG_INFO, msg);
            rh_sensors_high++;
        }
    }

    snprintf(msg, sizeof(msg), "Number of sensors have high CO2: %d.", co2_sensors_high);
    printmessage(LOG_INFO, msg);

    snprintf(msg, sizeof(msg), "Number of sensors have high RH: %d.", rh_sensors_high);
    printmessage(LOG_INFO, msg);

    if (co2_sensors_high > 0 && elapsed_time >= minimum_state_time)
    {
        strncpy(new_state, "highco2day", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (rh_sensors_high > 0 && elapsed_time >= minimum_state_time)
    {
        strncpy(new_state, "highrhday", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (is_day() == false)
    {
        snprintf(msg, sizeof(msg), "Transit to night.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "night", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (cooking_times() == true)
    {
        snprintf(msg, sizeof(msg), "Transit to cooking.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "cooking", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }
    else if (valve_cycle_times_day() == true)
    {
        snprintf(msg, sizeof(msg), "Transit to valve cycling day.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "cyclingday", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }
    // Manual high speed mode is ignored for now
    else
    {
        snprintf(msg, sizeof(msg), "Stay in day state.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "day", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        if (strcmp(state, new_state) != 0)
        {
            elapsed_time = 0;
            old_time = (esp_timer_get_time()) / 1000000;
        }
        strncpy(state, new_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }
}

void night_transitions(void)
{
    int co2highlevel = 0;
    int rhhighlevel = 0;
    int minimum_state_time = 0;
    int new_time = 0;
    bool valve_move_locked = 0;

    char msg[MSG_SIZE] = {};
    char new_state[MEDIUM_CONFIG_ITEM] = {};

    char *statemachine_state = NULL;
    char *state_fanspeed = NULL;

    co2_sensors_high = 0;
    rh_sensors_high = 0;

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        statemachine_state = state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (settings_state_night_mutex && xSemaphoreTake(settings_state_night_mutex, (TickType_t)10) == pdTRUE)
    {
        state_fanspeed = statenightsettings.state_night_fanspeed;
        co2highlevel = statenightsettings.state_night_highco2;
        rhhighlevel = statenightsettings.state_night_highrh;
        xSemaphoreGive(settings_state_night_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(fanspeed, state_fanspeed, sizeof(fanspeed));
        fanspeed[sizeof(fanspeed) - 1] = '\0';
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

    snprintf(msg, sizeof(msg), "High CO2 level: %d", co2highlevel);
    printmessage(LOG_INFO, msg);

    snprintf(msg, sizeof(msg), "High RH level: %d", rhhighlevel);
    printmessage(LOG_INFO, msg);

    snprintf(msg, sizeof(msg), "State: %s, fanspeed: %s, state_time: %d", statemachine_state, state_fanspeed, elapsed_time);
    printmessage(LOG_INFO, msg);

    set_fanspeed(state_fanspeed);
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
        snprintf(msg, sizeof(msg), "Valves are locked for moving.");
        printmessage(LOG_INFO, msg);
    }

    for (int i = 0; i < co2_sensor_counter; i++)
    {
        if (co2_sensors[i].co2_reading > co2highlevel)
        {
            snprintf(msg, sizeof(msg), "Sensor located at %s has high CO2.", co2_sensors[i].valve);
            printmessage(LOG_INFO, msg);
            snprintf(msg, sizeof(msg), "Transit to highco2day if minimum state time has expired.");
            printmessage(LOG_INFO, msg);
            co2_sensors_high++;
        }
    }

    for (int i = 0; i < rh_sensor_counter; i++)
    {
        if (rh_sensors[i].rh_reading > rhhighlevel)
        {
            snprintf(msg, sizeof(msg), "Sensor located at %s has high RH.", rh_sensors[i].valve);
            printmessage(LOG_INFO, msg);
            snprintf(msg, sizeof(msg), "Transit to highrhday if minimum state time has expired.");
            printmessage(LOG_INFO, msg);
            rh_sensors_high++;
        }
    }

    snprintf(msg, sizeof(msg), "Number of sensors have high CO2: %d.", co2_sensors_high);
    printmessage(LOG_INFO, msg);

    snprintf(msg, sizeof(msg), "Number of sensors have high RH: %d.", rh_sensors_high);
    printmessage(LOG_INFO, msg);

    // Conditions to transit to other state
    if (co2_sensors_high > 0 && elapsed_time >= minimum_state_time)
    {
        snprintf(msg, sizeof(msg), "Transit to highco2night.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "highco2night", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (rh_sensors_high > 0 && elapsed_time >= minimum_state_time)
    {
        snprintf(msg, sizeof(msg), "Transit to highrhnight.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "highrhnight", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (is_day() == true)
    {
        snprintf(msg, sizeof(msg), "Transit to day.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "day", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }
    else if (valve_cycle_times_night() == true)
    {
        snprintf(msg, sizeof(msg), "Transit to valve cycling night.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "cyclingnight", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }
    // Manual high speed mode is ignored for now
    else
    {
        snprintf(msg, sizeof(msg), "Conditions have not changed, it's still night.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "night", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        if (strcmp(state, new_state) != 0)
        {
            elapsed_time = 0;
            old_time = (esp_timer_get_time()) / 1000000;
        }
        strncpy(state, new_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }
}

void high_co2_day_transitions(void)
{
    int co2highlevel = 0;
    int co2lowlevel = 0;
    int reading = 0;
    int valve_nr = 0;
    int new_time = 0;
    int minimum_state_time = 0;
    int state_valve_position[12] = {0}; // Array for valvepositions from statemachine settings
    bool valve_move_locked = 0;

    char msg[MSG_SIZE] = {};
    char statemachine_state[MEDIUM_CONFIG_ITEM] = "highco2day";
    char new_state[MEDIUM_CONFIG_ITEM] = {};
    char valve[MEDIUM_CONFIG_ITEM] = {};

    char *state_fanspeed = NULL;

    co2_sensors_high = 0;

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(state, statemachine_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (settings_state_highco2day_mutex && xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)10) == pdTRUE)
    {
        state_fanspeed = statehighco2daysettings.state_highco2day_fanspeed;
        co2highlevel = statehighco2daysettings.co2_high_state_highco2day;
        co2lowlevel = statehighco2daysettings.co2_low_state_highco2day;
        xSemaphoreGive(settings_state_highco2day_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(fanspeed, state_fanspeed, sizeof(fanspeed));
        fanspeed[sizeof(fanspeed) - 1] = '\0';
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

    snprintf(msg, sizeof(msg), "High CO2 level: %d", co2highlevel);
    printmessage(LOG_INFO, msg);

    snprintf(msg, sizeof(msg), "Low CO2 level: %d", co2lowlevel);
    printmessage(LOG_INFO, msg);

    snprintf(msg, sizeof(msg), "State: %s, fanspeed: %s, state_time: %d", statemachine_state, state_fanspeed, elapsed_time);
    printmessage(LOG_INFO, msg);

    set_fanspeed(state_fanspeed);
    select_sensors();

    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    {
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    // Temp valve settings for individual valves starting with default settings for this state. Should read these from file and not hardcode them
    if (settings_state_highco2day_mutex && xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)10) == pdTRUE)
    {
        for (int i = 0; i < 12; i++)
        {
            state_valve_position[i] = statehighco2daysettings.valve_position_highco2day[i];
        }
        xSemaphoreGive(settings_state_highco2day_mutex);
    }

    // Iterate through CO2 sensors to see which one has high CO2 reading to see if default settings apply when high reading is at fan inlet
    // or only one valve needs to open (when high reading is in a room)
    for (int i = 0; i < co2_sensor_counter; i++)
    {
        strncpy(valve, co2_sensors[i].valve, sizeof(valve) - 1);
        valve[sizeof(valve) - 1] = '\0';
        char *substr = strstr(valve, "valve");
        if (substr)
        {
            memmove(substr, substr + 5, strlen(substr + 5) + 1); // 5 is length of "valve"
        }
        valve_nr = atoi(valve);
        reading = co2_sensors[i].co2_reading;

        if (strcmp(valve, "Fan inlet") != 0)
        {
            if (reading >= co2highlevel)
            {
                state_valve_position[valve_nr] = 20;
                snprintf(msg, sizeof(msg), "Sensor located at %s has high CO2.", co2_sensors[i].valve);
                printmessage(LOG_INFO, msg);
                snprintf(msg, sizeof(msg), "Transit to highco2day if minimum state time has expired.");
                printmessage(LOG_INFO, msg);
            }
            else if (reading >= co2lowlevel && reading < co2highlevel)
            {
                // The sensor value is between 900 and 1000 ppm so the valve position should remain at 20 until low co2 level is reached. The valve was set at 4 by default in above statements
                // This logic corrects the default setting of 4 to 20 and to make sure a deadband of the difference between highco2level and lowc02level is achieved
                // settings_state_temp[valve + "_position_state_temp"] = 20;
                state_valve_position[valve_nr] = 20;
                snprintf(msg, sizeof(msg), "Sensor located at %s has reading between low and high CO2.", co2_sensors[i].valve);
                printmessage(LOG_INFO, msg);
                snprintf(msg, sizeof(msg), "Valve remain at 20 until CO2 reduces to the CO2 low level.");
                printmessage(LOG_INFO, msg);
            }
            else
            {
                // Do nothing because valve is already back to default position according to sate
                snprintf(msg, sizeof(msg), "No sensor has high CO2. Valve set to default state pos.");
                printmessage(LOG_INFO, msg);
            }
        }

        if (reading > co2highlevel)
        {
            co2_sensors_high++;
        }
    }

    snprintf(msg, sizeof(msg), "Number of sensors have high CO2: %d.", co2_sensors_high);
    printmessage(LOG_INFO, msg);

    // Copy values to temp settings
    if (settings_state_temp_mutex && xSemaphoreTake(settings_state_temp_mutex, (TickType_t)10) == pdTRUE)
    {
        for (int i = 0; i < MAX_VALVES; i++)
        {
            statetempsettings.valve_position_temp[i] = state_valve_position[i];
        }
        xSemaphoreGive(settings_state_temp_mutex);
    }

    if (valve_move_locked == 0)
    {
        valve_position_statemachine("state_temp");
    }
    else
    {
        snprintf(msg, sizeof(msg), "Valves are locked for moving.");
        printmessage(LOG_INFO, msg);
    }

    // Conditions for transition.
    // This state always transits back to day first or to highco2night if night time.
    if (is_day() == true && co2_sensors_high == 0 && elapsed_time >= minimum_state_time) // Day with no high CO2 readings
    {
        snprintf(msg, sizeof(msg), "Transit to day.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "day", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (is_day() == false && co2_sensors_high > 0) // Night with high CO2 readings
    {
        snprintf(msg, sizeof(msg), "Transit to highco2night.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "highco2night", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        snprintf(msg, sizeof(msg), "Remain in highco2day state.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "highco2day", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        if (strcmp(state, new_state) != 0)
        {
            elapsed_time = 0;
            old_time = (esp_timer_get_time()) / 1000000;
        }
        strncpy(state, new_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }
}

void high_co2_night_transitions(void)
{
    int co2highlevel = 0;
    int co2lowlevel = 0;
    int reading = 0;
    int minimum_state_time = 0;
    int valve_nr = 0;
    int new_time = 0;

    int state_valve_position[12] = {0}; // Array for valvepositions from statemachine settings

    bool valve_move_locked = 0;
    bool state_valve_pos_file_present = 0;

    char msg[MSG_SIZE] = {};
    char statemachine_state[MEDIUM_CONFIG_ITEM] = "highco2night";
    char new_state[MEDIUM_CONFIG_ITEM] = {};
    char valve[MEDIUM_CONFIG_ITEM] = {};

    char *state_fanspeed = NULL;

    co2_sensors_high = 0;

    // Actions for this state
    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(state, statemachine_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (settings_state_highco2night_mutex && xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)10) == pdTRUE)
    {
        state_fanspeed = statehighco2nightsettings.state_highco2night_fanspeed;
        co2highlevel = statehighco2nightsettings.co2_high_state_highco2night;
        co2lowlevel = statehighco2nightsettings.co2_low_state_highco2night;
        xSemaphoreGive(settings_state_highco2night_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(fanspeed, state_fanspeed, sizeof(fanspeed));
        fanspeed[sizeof(fanspeed) - 1] = '\0';
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

    snprintf(msg, sizeof(msg), "High CO2 level: %d", co2highlevel);
    printmessage(LOG_INFO, msg);

    snprintf(msg, sizeof(msg), "Low CO2 level: %d", co2lowlevel);
    printmessage(LOG_INFO, msg);

    snprintf(msg, sizeof(msg), "State: %s, fanspeed: %s, state_time: %d", statemachine_state, state_fanspeed, elapsed_time);
    printmessage(LOG_INFO, msg);

    set_fanspeed(state_fanspeed);
    select_sensors();

    new_time = (esp_timer_get_time()) / 1000000;
    if (new_time > old_time)
    {
        elapsed_time += new_time - old_time;
        old_time = new_time;
    }

    if (settings_state_highco2night_mutex && xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)10) == pdTRUE)
    {
        for (int i = 0; i < 12; i++)
        {
            state_valve_position[i] = statehighco2nightsettings.valve_position_highco2night[i];
        }
        xSemaphoreGive(settings_state_highco2night_mutex);
    }

    // High CO2 has been detected to come into this state. Iterate through CO2 sensors to see which sensor detects high CO2. Valves with CO2 sensors are default
    // set to 20 for this state. Valves with a CO2 value lower than 900 ppm will be closed to 4 to direct airflow to the rooms with high CO2 reading.
    for (int i = 0; i < co2_sensor_counter; i++)
    {
        strncpy(valve, co2_sensors[i].valve, sizeof(valve) - 1);
        valve[sizeof(valve) - 1] = '\0';
        char *substr = strstr(valve, "valve");
        if (substr)
        {
            memmove(substr, substr + 5, strlen(substr + 5) + 1); // 5 is length of "valve"
        }
        valve_nr = atoi(valve);
        reading = co2_sensors[i].co2_reading;

        if (valve != "Fan inlet")
        {
            if (reading <= co2lowlevel)
            {
                // CO2 is low so close the valve re-route the air to valves with high CO2
                state_valve_position[valve_nr] = 4;
                snprintf(msg, sizeof(msg), "Sensor at %s has low CO2.", co2_sensors[i].valve);
                printmessage(LOG_INFO, msg);
                snprintf(msg, sizeof(msg), "Valve reamains at 4 until CO2 exceeds the CO2 low level.");
                printmessage(LOG_INFO, msg);
            }
            else if (reading > co2lowlevel && reading < co2highlevel)
            {
                // The sensor value is between 900 and 1000 ppm so the valve position should remain at 4 until low co2 level is reached. The valve was set at 20 by default.
                // This logic corrects the default setting of 4 to 20 and to make sure a deadband of the difference between highco2level and lowc02level is achieved
                state_valve_position[valve_nr] = 4;
                snprintf(msg, sizeof(msg), "Sensor at %s has CO2 reading between low and high CO2 level.");
                printmessage(LOG_INFO, msg);
                snprintf(msg, sizeof(msg), "Valve remain at 20 until CO2 reduces to the CO2 low level", co2_sensors[i].valve);
                printmessage(LOG_INFO, msg);
            }
            else
            {
                // The sensor value is above co2highlevel so default valve setting applies
                snprintf(msg, sizeof(msg), "Sensor at %s has CO2 reading higher than co2highlevel.", co2_sensors[i].valve);
                printmessage(LOG_INFO, msg);
                snprintf(msg, sizeof(msg), "Default valve setting applies.");
                printmessage(LOG_INFO, msg);
            }
        }

        if (reading > co2highlevel)
        {
            co2_sensors_high++;
        }
    }

    snprintf(msg, sizeof(msg), "Number of sensors have high CO2: %d.", co2_sensors_high);
    printmessage(LOG_INFO, msg);

    // Copy values to temp settings
    if (settings_state_temp_mutex && xSemaphoreTake(settings_state_temp_mutex, (TickType_t)10) == pdTRUE)
    {
        for (int i = 0; i < MAX_VALVES; i++)
        {
            statetempsettings.valve_position_temp[i] = state_valve_position[i];
        }
        xSemaphoreGive(settings_state_temp_mutex);
    }

    if (valve_move_locked == 0)
    {
        valve_position_statemachine("state_temp");
    }
    else
    {
        snprintf(msg, sizeof(msg), "Valves are locked for moving.");
        printmessage(LOG_INFO, msg);
    }

    // Conditions for transition
    // This state always transits back to night first or to highco2day if day time.
    if (co2_sensors_high == 0 && elapsed_time >= minimum_state_time) // Night with no high CO2 readings
    {
        snprintf(msg, sizeof(msg), "Transit to night.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "night", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (is_day() == true && co2_sensors_high > 0) // Day with high CO2 readings, no need to wait for minimum_state_time
    {
        snprintf(msg, sizeof(msg), "Transit to highco2day.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "highco2day", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        snprintf(msg, sizeof(msg), "Remain in highco2night state.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "highco2night", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        if (strcmp(state, new_state) != 0)
        {
            elapsed_time = 0;
            old_time = (esp_timer_get_time()) / 1000000;
        }
        strncpy(state, new_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }
}

void high_rh_day_transitions(void)
{
    int rhlowlevel = 0;
    int minimum_state_time = 0;
    int maximum_state_time = 0;
    int new_time = 0;
    bool valve_move_locked = 0;

    char msg[MSG_SIZE] = {};
    char statemachine_state[MEDIUM_CONFIG_ITEM] = "highrhday";
    char new_state[MEDIUM_CONFIG_ITEM] = {};

    char *state_fanspeed = NULL;

    rh_sensors_high = 0;

    // Actions for this sate
    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(state, statemachine_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (settings_state_highrhday_mutex && xSemaphoreTake(settings_state_highrhday_mutex, (TickType_t)10) == pdTRUE)
    {
        state_fanspeed = statehighrhdaysettings.state_highrhday_fanspeed;
        rhlowlevel = statehighrhdaysettings.rh_low_state_highrhday;
        maximum_state_time = statehighrhdaysettings.maximum_state_time_highrhday;
        xSemaphoreGive(settings_state_highrhday_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(fanspeed, state_fanspeed, sizeof(fanspeed));
        fanspeed[sizeof(fanspeed) - 1] = '\0';
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

    snprintf(msg, sizeof(msg), "Low RH level: %d", rhlowlevel);
    printmessage(LOG_INFO, msg);

    snprintf(msg, sizeof(msg), "State: %s, fanspeed: %s, state_time: %d", statemachine_state, state_fanspeed, elapsed_time);
    printmessage(LOG_INFO, msg);

    set_fanspeed(state_fanspeed);
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
        snprintf(msg, sizeof(msg), "Valves are locked for moving.");
        printmessage(LOG_INFO, msg);
    }

    // High RH has been detected to come into this state. Iterate through RH sensors to see which sensor detects high RH. This state does not change valve positions
    for (int i = 0; i < rh_sensor_counter; i++)
    {
        if (rh_sensors[i].rh_reading > rhlowlevel)
        {
            rh_sensors_high++;
            snprintf(msg, sizeof(msg), "Sensor at %s has high RH.", co2_sensors[i].valve);
            printmessage(LOG_INFO, msg);
        }
    }

    // message = "Number of sensors measure high RH: " + String(rh_sensors_high);
    snprintf(msg, sizeof(msg), "Number of sensors have high RH: %d.", rh_sensors_high);
    printmessage(LOG_INFO, msg);

    // Conditions for transition
    if ((rh_sensors_high == 0 && elapsed_time >= minimum_state_time) || elapsed_time >= maximum_state_time)
    {
        snprintf(msg, sizeof(msg), "Transit to day.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "day", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (is_day() == false)
    {
        snprintf(msg, sizeof(msg), "Transit to high_rh_night.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "highrhnight", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        snprintf(msg, sizeof(msg), "Remain in high_rh_day state");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "highrhday", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        if (strcmp(state, new_state) != 0)
        {
            elapsed_time = 0;
            old_time = (esp_timer_get_time()) / 1000000;
        }
        strncpy(state, new_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }
}

void high_rh_night_transitions(void)
{
    int rhlowlevel = 0;
    int minimum_state_time = 0;
    int maximum_state_time = 0;
    int new_time = 0;
    bool valve_move_locked = 0;

    char msg[MSG_SIZE] = {};
    char statemachine_state[MEDIUM_CONFIG_ITEM] = "highrhnight";
    char new_state[MEDIUM_CONFIG_ITEM] = {};

    char *state_fanspeed = NULL;

    rh_sensors_high = 0;

    // Actions for this state
    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(state, statemachine_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    if (settings_state_highrhnight_mutex && xSemaphoreTake(settings_state_highrhnight_mutex, (TickType_t)10) == pdTRUE)
    {
        state_fanspeed = statehighrhnightsettings.state_highrhnight_fanspeed;
        rhlowlevel = statehighrhnightsettings.rh_low_state_highrhnight;
        maximum_state_time = statehighrhnightsettings.maximum_state_time_highrhnight;
        xSemaphoreGive(settings_state_highrhnight_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(fanspeed, state_fanspeed, sizeof(fanspeed));
        fanspeed[sizeof(fanspeed) - 1] = '\0';
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

    snprintf(msg, sizeof(msg), "Low RH level: %d", rhlowlevel);
    printmessage(LOG_INFO, msg);

    snprintf(msg, sizeof(msg), "State: %s, fanspeed: %s, state_time: %d", statemachine_state, state_fanspeed, elapsed_time);
    printmessage(LOG_INFO, msg);

    set_fanspeed(state_fanspeed);
    select_sensors();

    // If the statemachine is till in this state after 30 mins then RH cannot be lowered with ventilation
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
        snprintf(msg, sizeof(msg), "Valves are locked for moving.");
        printmessage(LOG_INFO, msg);
    }

    // High RH has been detected to come into this state. Iterate through RH sensors to see which sensor detects high RH. This state does not change valve positions
    for (int i = 0; i < rh_sensor_counter; i++)
    {
        if (rh_sensors[i].rh_reading > rhlowlevel)
        {
            snprintf(msg, sizeof(msg), "Sensor at %s has high RH.", rh_sensors[i].valve);
            printmessage(LOG_INFO, msg);
            rh_sensors_high++;
        }
    }

    snprintf(msg, sizeof(msg), "Number of sensors have high RH: %d.", rh_sensors_high);
    printmessage(LOG_INFO, msg);

    // Conditions for transition
    if (rh_sensors_high == 0 && elapsed_time >= minimum_state_time || elapsed_time >= maximum_state_time)
    {
        snprintf(msg, sizeof(msg), "Transit to night.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "night", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (is_day() == true)
    {
        snprintf(msg, sizeof(msg), "Transit to highrhday.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "highrhday", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        snprintf(msg, sizeof(msg), "Remain in high_rh_night state.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "highrhnight", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        if (strcmp(state, new_state) != 0)
        {
            elapsed_time = 0;
            old_time = (esp_timer_get_time()) / 1000000;
        }
        strncpy(state, new_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }
}

void cooking_transitions(void)
{
    int new_time = 0;
    bool valve_move_locked = 0;

    char msg[MSG_SIZE] = {};
    char statemachine_state[MEDIUM_CONFIG_ITEM] = "cooking";
    char new_state[MEDIUM_CONFIG_ITEM] = {};

    char *state_fanspeed = NULL;

    // Actions for this sate
    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(state, statemachine_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    // read fanspeed from config of this state
    if (settings_state_cooking_mutex && xSemaphoreTake(settings_state_cooking_mutex, (TickType_t)10) == pdTRUE)
    {
        state_fanspeed = statecookingsettings.state_cooking_fanspeed;
        xSemaphoreGive(settings_state_cooking_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(fanspeed, state_fanspeed, sizeof(fanspeed));
        fanspeed[sizeof(fanspeed) - 1] = '\0';
        xSemaphoreGive(fanspeed_mutex);
    }

    // Disable valve moving when valves are already moving
    if (lock_valve_move_mutex && xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
    {
        valve_move_locked = lock_valve_move;
        xSemaphoreGive(lock_valve_move_mutex);
    }

    snprintf(msg, sizeof(msg), "State: %s, fanspeed: %s, state_time: %d", statemachine_state, state_fanspeed, elapsed_time);
    printmessage(LOG_INFO, msg);

    set_fanspeed(state_fanspeed);

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
        snprintf(msg, sizeof(msg), "Valves are locked for moving.");
        printmessage(LOG_INFO, msg);
    }

    // Conditions for transition
    if (cooking_times() == false && is_day() == true)
    {
        snprintf(msg, sizeof(msg), "Transit to day.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "day", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else if (cooking_times() == false && is_day() == false)
    {
        snprintf(msg, sizeof(msg), "Transit to night.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "night", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        snprintf(msg, sizeof(msg), "Remain in cooking state.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "cooking", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        if (strcmp(state, new_state) != 0)
        {
            elapsed_time = 0;
            old_time = (esp_timer_get_time()) / 1000000;
        }
        strncpy(state, new_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }
}

void valve_cycle_day_transitions(void)
{
    int co2_sensors_high = 0;
    int rh_sensors_high = 0;
    int temp_hour = 0;
    int temp_minute = 0;
    int new_time = 0;
    bool valve_move_locked = 0;

    char msg[MSG_SIZE] = {};
    char statemachine_state[MEDIUM_CONFIG_ITEM] = "cyclingday";
    char new_state[MEDIUM_CONFIG_ITEM] = {};

    char *state_fanspeed = NULL;

    // Actions for this state
    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(state, statemachine_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    // read fanspeed from config of this state
    if (settings_state_cyclingday_mutex && xSemaphoreTake(settings_state_cyclingday_mutex, (TickType_t)10) == pdTRUE)
    {
        state_fanspeed = statecyclingdaysettings.state_cyclingday_fanspeed;
        xSemaphoreGive(settings_state_cyclingday_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(fanspeed, state_fanspeed, sizeof(fanspeed));
        fanspeed[sizeof(fanspeed) - 1] = '\0';
        xSemaphoreGive(fanspeed_mutex);
    }

    // Disable valve moving when valves are already moving
    if (lock_valve_move_mutex && xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
    {
        valve_move_locked = lock_valve_move;
        xSemaphoreGive(lock_valve_move_mutex);
    }

    snprintf(msg, sizeof(msg), "State: %s, fanspeed: %s, state_time: %d", statemachine_state, state_fanspeed, elapsed_time);
    printmessage(LOG_INFO, msg);

    set_fanspeed(state_fanspeed);

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
        snprintf(msg, sizeof(msg), "Valves are locked for moving.");
        printmessage(LOG_INFO, msg);
    }

    // Conditions for transition
    if (valve_cycle_times_day() == false)
    {
        snprintf(msg, sizeof(msg), "Transit to day.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "day", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        snprintf(msg, sizeof(msg), "Remain in cycling day.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "cyclingday", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        if (strcmp(state, new_state) != 0)
        {
            elapsed_time = 0;
            old_time = (esp_timer_get_time()) / 1000000;
        }
        strncpy(state, new_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }
}

void valve_cycle_night_transitions(void)
{
    int new_time = 0;
    bool valve_move_locked = 0;

    char msg[MSG_SIZE] = {};
    char statemachine_state[MEDIUM_CONFIG_ITEM] = "cyclingnight";
    char new_state[MEDIUM_CONFIG_ITEM] = {};

    char *state_fanspeed = NULL;

    // Actions for this state
    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(state, statemachine_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    // read fanspeed from config of this state
    if (settings_state_cyclingnight_mutex && xSemaphoreTake(settings_state_cyclingnight_mutex, (TickType_t)10) == pdTRUE)
    {
        state_fanspeed = statecyclingnightsettings.state_cyclingnight_fanspeed;
        xSemaphoreGive(settings_state_cyclingnight_mutex);
    }

    if (fanspeed_mutex && xSemaphoreTake(fanspeed_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(fanspeed, state_fanspeed, sizeof(fanspeed));
        fanspeed[sizeof(fanspeed) - 1] = '\0';
        xSemaphoreGive(fanspeed_mutex);
    }

    // Disable valve moving when valves are already moving
    if (lock_valve_move_mutex && xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
    {
        valve_move_locked = lock_valve_move;
        xSemaphoreGive(lock_valve_move_mutex);
    }

    snprintf(msg, sizeof(msg), "State: %s, fanspeed: %s, state_time: %d", statemachine_state, state_fanspeed, elapsed_time);
    printmessage(LOG_INFO, msg);

    set_fanspeed(state_fanspeed);

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
        snprintf(msg, sizeof(msg), "Valves are locked for moving.");
        printmessage(LOG_INFO, msg);
    }

    // Conditions for transition
    if (valve_cycle_times_night() == false)
    {
        snprintf(msg, sizeof(msg), "Transit to night.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "night", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
        // elapsed_time = 0;
        // old_time = (esp_timer_get_time()) / 1000000;
    }
    else
    {
        snprintf(msg, sizeof(msg), "Remain in cycling night.");
        printmessage(LOG_INFO, msg);
        strncpy(new_state, "cyclingnight", sizeof(new_state));
        new_state[sizeof(new_state) - 1] = '\0';
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        if (strcmp(state, new_state) != 0)
        {
            elapsed_time = 0;
            old_time = (esp_timer_get_time()) / 1000000;
        }
        strncpy(state, new_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }
}

// This state is for later
void manual_high_speed_transitions(void)
{
    bool valve_move_locked = 0;

    char msg[MSG_SIZE] = {};
    char statemachine_state[LARGE_CONFIG_ITEM] = "manual_high_speed";
    char state_fanspeed[MEDIUM_CONFIG_ITEM] = "High";
    char new_state[MEDIUM_CONFIG_ITEM] = {};

    // Actions for this state
    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        strncpy(state, statemachine_state, sizeof(state));
        state[sizeof(state) - 1] = '\0';
        xSemaphoreGive(statemachine_state_mutex);
    }

    // Disable valve moving when valves are already moving
    if (lock_valve_move_mutex && xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
    {
        valve_move_locked = lock_valve_move;
        xSemaphoreGive(lock_valve_move_mutex);
    }

    snprintf(msg, sizeof(msg), "State: %s, fanspeed: %s, state_time: %d", statemachine_state, fanspeed, elapsed_time);
    printmessage(LOG_INFO, msg);

    set_fanspeed(fanspeed);

    if (valve_move_locked == 0)
    {
        valve_position_statemachine(statemachine_state);
    }
    else
    {
        snprintf(msg, sizeof(msg), "Valves are locked for moving.");
        printmessage(LOG_INFO, msg);
    }
}

void select_sensors(void)
{
    int j = 0; // counter for struct
    int k = 0; // counter for struct

    char *co2_sensor_wire = NULL;
    char *co2_sensor_wire1 = NULL;
    char *rh_sensor_wire = NULL;
    char *rh_sensor_wire1 = NULL;
    char *valve_wire = NULL;
    char *valve_wire1 = NULL;
    char msg[MSG_SIZE] = {};

    co2_sensor_counter = 0;
    rh_sensor_counter = 0;

    float sensor_data[SENSOR_I2C_BUSSES][SENSOR_COUNT][SENSOR_DATA_FIELDS];

    // Copy sensor readings from global
    if (xQueuePeek(sensor_avg_queue, &sensor_data, 0) != pdTRUE)
    {
        snprintf(msg, sizeof(msg), "Failed to read from sensor_avg_queue in function");
        printmessage(LOG_INFO, msg);
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
            snprintf(msg, sizeof(msg), "%s, CO2: %.2f", co2_sensors[j].valve, co2_sensors[j].co2_reading);
            printmessage(LOG_INFO, msg);
            j++;
        }

        if (co2_sensor_wire1 && strcmp(co2_sensor_wire1, "On") == 0)
        {
            co2_sensors[j].valve = valve_wire1;
            co2_sensors[j].co2_reading = sensor_data[1][i][2];
            snprintf(msg, sizeof(msg), "%s, CO2: %.2f", co2_sensors[j].valve, co2_sensors[j].co2_reading);
            printmessage(LOG_INFO, msg);
            j++;
        }

        if (rh_sensor_wire && strcmp(rh_sensor_wire, "On") == 0)
        {
            rh_sensors[k].valve = valve_wire;
            rh_sensors[k].rh_reading = sensor_data[0][i][1];
            snprintf(msg, sizeof(msg), "%s, RH: %.2f", rh_sensors[k].valve, rh_sensors[k].rh_reading);
            printmessage(LOG_INFO, msg);
            k++;
        }

        if (rh_sensor_wire1 && strcmp(rh_sensor_wire1, "On") == 0)
        {
            rh_sensors[k].valve = valve_wire1;
            rh_sensors[k].rh_reading = sensor_data[1][i][1];
            snprintf(msg, sizeof(msg), "%s, RH: %.2f", rh_sensors[k].valve, rh_sensors[k].rh_reading);
            printmessage(LOG_INFO, msg);
            k++;
        }
    }

    co2_sensor_counter = j;
    rh_sensor_counter = k;
}

int parse_co2_sensors_high(void)
{
    return co2_sensors_high;
}

int parse_rh_sensors_high(void)
{
    return rh_sensors_high;
}