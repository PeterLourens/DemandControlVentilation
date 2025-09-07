#include "valvecontrol.h"

// Control valves from web interface and from statemachine
void move_valve(void)
{
    // Temporary variables used after selection of I/O
    int clockPin = 0;
    int latchPin = 0;
    int dataPin = 0;
    int valve_number = 0;
    int direction = 0;
    int valve_pos = 0;
    int valve_position_change = 0;
    int new_valve_position_change = 0;
    int new_valve_position = 0;
    int store_valve_position = 0;
    int check_valve_position = 0;
    int write_failed_counter = 0;
    bool write_valve_position_file = false;
    char new_valve_positions[512] = {};
    char buffer[512] = {};
    char msg[MSG_SIZE] = {};
    String temp_state = "";
    JsonDocument doc;

    if (valve_control_data_mutex && xSemaphoreTake(valve_control_data_mutex, (TickType_t)10) == pdTRUE)
    {
        store_valve_position = valvecontroldata.write_new_position;
        check_valve_position = valvecontroldata.check_position;
        xSemaphoreGive(valve_control_data_mutex);
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        temp_state = state;
        xSemaphoreGive(statemachine_state_mutex);
    }

    // Read actual valve positions
    if (read_settings(VALVE_POSITIONS_PATH, buffer, sizeof(buffer), valve_position_file_mutex))
    {
        DeserializationError error = deserializeJson(doc, buffer);
        if (error)
        {
            snprintf(msg, sizeof(msg), "[ERROR] Failed to parse %s with error: %s.", VALVE_POSITIONS_PATH, error);
            printmessage(msg);
        }
    }

    // Debug
    snprintf(msg, sizeof(msg), "[INFO] Store new valve Position: %d, Check valve position: ", store_valve_position, check_valve_position);
    printmessage(msg);

    for (int i = 0; i < MAX_VALVES; i++)
    {
        if (valve_control_data_mutex && xSemaphoreTake(valve_control_data_mutex, (TickType_t)100) == pdTRUE)
        {
            valve_number = valvecontroldata.valve_number[i];
            valve_position_change = valvecontroldata.position_change[i];
            direction = valvecontroldata.direction[i];
            xSemaphoreGive(valve_control_data_mutex);
        }
        valve_pos = doc["valve" + String(i)];

        // Assign the correct IO based on valve number
        if (valve_number < 6)
        {
            latchPin = LATCHPIN1;
            clockPin = CLOCKPIN1;
            dataPin = DATAPIN1;
        }
        else
        {
            latchPin = LATCHPIN2;
            clockPin = CLOCKPIN2;
            dataPin = DATAPIN2;
        }

        if (check_valve_position == 1)
        {
            if (direction == 0)
            { // Close
                new_valve_position_change = min(valve_position_change, valve_pos);
                new_valve_position = valve_pos - new_valve_position_change;
            }
            else
            { // Open
                new_valve_position_change = min(valve_position_change, 24 - valve_pos);
                new_valve_position = valve_pos + new_valve_position_change;
            }
            snprintf(msg, sizeof(msg), "Valve: %d, request: %.2f, current: %.2f, move: %.2f, direction: %s", i, valve_position_change, valve_pos, new_valve_position_change, (direction == 0 ? "close" : "open"));
            printmessage(msg);
            valvecontrol(direction, new_valve_position_change, valve_number, dataPin, clockPin, latchPin);
        }
        else if (check_valve_position == 0 && (valve_pos + valve_position_change) <= 24 && (valve_pos - valve_position_change) >= 0)
        {
            valvecontrol(direction, valve_position_change, valve_number, dataPin, clockPin, latchPin);
        }
        else
        {
            snprintf(msg, sizeof(msg), "[ERROR] Error in valvecontrol. Position change outside valve range of 0 and 24");
            printmessage(msg);
        }

        // Storing new valve positions only makes sense in combination with new calculated positions
        if (store_valve_position == 1 && check_valve_position == 1)
        {
            doc["valve" + String(i)] = new_valve_position;
        }
    }

    // Convert from JsonDocument to char for writing
    serializeJson(doc, new_valve_positions);

    // Write status file only if required
    if (store_valve_position == 1)
    {
        if (valve_position_file_mutex != NULL)
        {
            while (write_valve_position_file == false && write_failed_counter <= 5)
            {
                write_valve_position_file = write_settings(VALVE_POSITIONS_PATH, new_valve_positions, valve_control_data_mutex);
                write_failed_counter++;
            }

            if (write_valve_position_file == false)
            {
                snprintf(msg, sizeof(msg), "[ERROR] Failed to write valve positions to file: %s", VALVE_POSITIONS_PATH);
                printmessage(msg);
                temp_state == "stopped";
            }
            else
            {
                // Update actual valve position struct with new valve positions after succesful write of valve positions to file
                if (valve_control_data_mutex && xSemaphoreTake(valve_control_data_mutex, (TickType_t)10) == pdTRUE)
                {
                    for (int i = 0; i < MAX_VALVES; i++)
                    {
                        valvecontroldata.actual_valve_position[i] = doc["valve" + String(i)];
                    }
                    xSemaphoreGive(valve_control_data_mutex);
                }
                snprintf(msg, sizeof(msg), "[INFO] Valve positions stored successfully in: %s", VALVE_POSITIONS_PATH);
                printmessage(msg);
            }
        }
    }

    if (statemachine_state_mutex && xSemaphoreTake(statemachine_state_mutex, (TickType_t)10) == pdTRUE)
    {
        state = temp_state;
        xSemaphoreGive(statemachine_state_mutex);
    }
}

void valvecontrol(int direction, int position_change, int valve_number, int dataPin, int clockPin, int latchPin)
{
    // This function moves one valve based on 3 inputs. If multiple valves need to move, this function is called multiple
    // times. This function does not check if the valves are controlled outside their operating window so all requests for
    // valve movement should be checked by another function before calling this function. The reason why not to put the
    // operating window check in this functions is that the user must be able to calibrated the valves, i.e. line up up
    // the valves with the registered positions. The requested move is relative to the current position and not the new absolute
    // position of the valve.

    // Local variables
    int output[3][4] = {0}; // Output array with switching pattern, initialise at 0;
    int i;                  // Counter for creating output array
    int j;                  // Counter for
    int k;                  // Counter to iterate through output array

    // Variables which are application settings
    int cycles = 24; // the number if cycles to complete one rotation of the shaft

    // switching pattern for steppermotor in 8 bits.
    int pattern1[4] = {0b00000101, 0b00001001, 0b00001010, 0b00000110};
    int pattern2[4] = {0b01010000, 0b10010000, 0b10100000, 0b01100000};

    char msg[MSG_SIZE] = {};

    // Disable valve moving when valves are already moving
    if (lock_valve_move_mutex && xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
    {
        lock_valve_move = 1;
        xSemaphoreGive(lock_valve_move_mutex);
    }

    // Valve 0 - 5 has the same output as valves 6-11
    if (valve_number >= 6)
    {
        valve_number = valve_number - 6;
    }

    switch (valve_number)
    {
    case 0:
        for (i = 0; i < 4; i++)
        {
            output[0][i] = pattern1[i];
            output[1][i] = 0;
            output[2][i] = 0;
        }
        break;
    case 1:
        for (i = 0; i < 4; i++)
        {
            output[0][i] = pattern2[i];
            output[1][i] = 0;
            output[2][i] = 0;
        }
        break;
    case 2:
        for (i = 0; i < 4; i++)
        {
            output[0][i] = 0;
            output[1][i] = pattern1[i];
            output[2][i] = 0;
        }
        break;
    case 3:
        for (i = 0; i < 4; i++)
        {
            output[0][i] = 0;
            output[1][i] = pattern2[i];
            output[2][i] = 0;
        }
        break;
    case 4:
        for (i = 0; i < 4; i++)
        {
            output[0][i] = 0;
            output[1][i] = 0;
            output[2][i] = pattern1[i];
        }
        break;
    case 5:
        for (i = 0; i < 4; i++)
        {
            output[0][i] = 0;
            output[1][i] = 0;
            output[2][i] = pattern2[i];
        }
        break;
    }

    // Direction 1 is open valve
    if (direction == 1 && position_change <= 24)
    {
        // Loop to run the number of cycles to make one turn * the number of turns to make requested_position_change
        for (j = 0; j < (cycles * position_change); j++)
        {
            // Loop to make one cycle of the four coils in the motor
            for (k = 0; k < 4; k++)
            {
                digitalWrite(latchPin, 0); // ground latchPin and hold low for as long as you are transmitting
                shiftOut(dataPin, clockPin, MSBFIRST, output[2][k]);
                shiftOut(dataPin, clockPin, MSBFIRST, output[1][k]);
                shiftOut(dataPin, clockPin, MSBFIRST, output[0][k]);
                digitalWrite(latchPin, HIGH);
                vTaskDelay(10);
            }
        }
        // after running all outputs should be off
        all_outputs_off(dataPin, clockPin, latchPin);
    }

    // Direction 0 is close valve
    else if (direction == 0 && position_change <= 24)
    {
        // Loop to run the number of cycles to make one turn * the number of turns to make requested_position_change
        for (j = 0; j < (cycles * position_change); j++)
        {
            // Loop to make one cycle of the four coils in the motor
            for (k = 3; k > -1; k--)
            {
                digitalWrite(latchPin, 0);
                shiftOut(dataPin, clockPin, MSBFIRST, output[2][k]);
                shiftOut(dataPin, clockPin, MSBFIRST, output[1][k]);
                shiftOut(dataPin, clockPin, MSBFIRST, output[0][k]);
                digitalWrite(latchPin, HIGH);
                vTaskDelay(10);
            }
        }
        all_outputs_off(dataPin, clockPin, latchPin);
    }
    else
    {
        snprintf(msg, sizeof(msg), "[ERROR] Error in valvecontrol. Requested position change is: %d. Position change should be between 0 and 24", position_change);
        printmessage(msg);
    }

    // Enable valve moving
    if (lock_valve_move_mutex && xSemaphoreTake(lock_valve_move_mutex, (TickType_t)10) == pdTRUE)
    {
        lock_valve_move = 0;
        xSemaphoreGive(lock_valve_move_mutex);
    }
}

void all_outputs_off(int dataPin, int clockPin, int latchPin)
{
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
}

void valve_position_statemachine(String statemachine_state)
{
    /*
    The move_valve function works with requested position move value and is not with absolute positions of the valves.
    1. Select valve position file based on global variable state (with Mutex!)
    2. Read file with valve settings for state (e.g. day or high_co2_) of statemachine.
    3. Read file with existing positions (with mutex!)
    4. Calculate how much the valve should move and calculate direction.
    5. Set valve check and update position file to active
    6. Update valve_control_data global variable with data from step 4 (with Mutex!
    7. Call the move valve function.

    Data structure for each JSON valve_control_data Structure
    {
        "valve0_data": [valve_vumber,valve move,valvemove_direction],
        "valve1_data": [valve_vumber,valve move,valvemove_direction],
        ......
        ......
        "valve11_data": [valve_vumber,valve move,valvemove_direction]
    }
    */

    // String actual_valve_pos_json = "";
    int move = 0;
    int direction = 0;
    int valve_number = 0;
    int actual_valve_pos = 0;
    int state_valve_pos = 0;
    int sum_move = 0; // Variable for decision on writing config file (sum>0) or not (sum=0)

    char buffer[512];
    char msg[MSG_SIZE] = {};

    JsonDocument actual_valve_pos_doc;

    for (int i = 0; i < MAX_VALVES; i++)
    {
        // actual_valve_pos = actual_valve_pos_doc["valve" + String(i)];
        if (valve_control_data_mutex && xSemaphoreTake(valve_control_data_mutex, (TickType_t)10) == pdTRUE)
        {
            actual_valve_pos = valvecontroldata.actual_valve_position[i];
            xSemaphoreGive(valve_control_data_mutex);
        }

        if (statemachine_state == "day")
        {
            if (settings_state_day_mutex && xSemaphoreTake(settings_state_day_mutex, (TickType_t)10) == pdTRUE)
            {
                state_valve_pos = statedaysettings.valve_position_day[i];
                xSemaphoreGive(settings_state_day_mutex);
            }
        }
        else if (statemachine_state == "night")
        {
            if (settings_state_night_mutex && xSemaphoreTake(settings_state_night_mutex, (TickType_t)10) == pdTRUE)
            {
                state_valve_pos = statenightsettings.valve_position_night[i];
                xSemaphoreGive(settings_state_night_mutex);
            }
        }
        else if (statemachine_state == "highco2day")
        {
            if (settings_state_highco2day_mutex && xSemaphoreTake(settings_state_highco2day_mutex, (TickType_t)10) == pdTRUE)
            {
                state_valve_pos = statehighco2daysettings.valve_position_highco2day[i];
                xSemaphoreGive(settings_state_highco2day_mutex);
            }
        }
        else if (statemachine_state == "highco2night")
        {
            if (settings_state_highco2night_mutex && xSemaphoreTake(settings_state_highco2night_mutex, (TickType_t)10) == pdTRUE)
            {
                state_valve_pos = statehighco2nightsettings.valve_position_highco2night[i];
                xSemaphoreGive(settings_state_highco2night_mutex);
            }
        }
        else if (statemachine_state == "highrhday")
        {
            if (settings_state_highrhday_mutex && xSemaphoreTake(settings_state_highrhday_mutex, (TickType_t)10) == pdTRUE)
            {
                state_valve_pos = statehighrhdaysettings.valve_position_highrhday[i];
                xSemaphoreGive(settings_state_highrhday_mutex);
            }
        }
        else if (statemachine_state == "highrhnight")
        {
            if (settings_state_highrhnight_mutex && xSemaphoreTake(settings_state_highrhnight_mutex, (TickType_t)10) == pdTRUE)
            {
                state_valve_pos = statehighrhnightsettings.valve_position_highrhnight[i];
                xSemaphoreGive(settings_state_highrhnight_mutex);
            }
        }
        else if (statemachine_state == "cooking")
        {
            if (settings_state_cooking_mutex && xSemaphoreTake(settings_state_cooking_mutex, (TickType_t)10) == pdTRUE)
            {
                state_valve_pos = statecookingsettings.valve_position_cooking[i];
                xSemaphoreGive(settings_state_cooking_mutex);
            }
        }
        else if (statemachine_state == "cyclingday")
        {
            if (settings_state_cyclingday_mutex && xSemaphoreTake(settings_state_cyclingday_mutex, (TickType_t)10) == pdTRUE)
            {
                state_valve_pos = statecyclingdaysettings.valve_position_cyclingday[i];
                xSemaphoreGive(settings_state_cyclingday_mutex);
            }
        }
        else if (statemachine_state == "cyclingnight")
        {
            if (settings_state_cyclingnight_mutex && xSemaphoreTake(settings_state_cyclingnight_mutex, (TickType_t)10) == pdTRUE)
            {
                state_valve_pos = statecyclingnightsettings.valve_position_cyclingnight[i];
                xSemaphoreGive(settings_state_cyclingnight_mutex);
            }
        }
        else if (statemachine_state == "state_temp")
        {
            if (settings_state_temp_mutex && xSemaphoreTake(settings_state_temp_mutex, (TickType_t)10) == pdTRUE)
            {
                state_valve_pos = statetempsettings.valve_position_temp[i];
                xSemaphoreGive(settings_state_temp_mutex);
            }
        }
        else
        {
            snprintf(msg, sizeof(msg), "[ERROR] Statemachine state not defined. Can't assign state valve positions.");
            printmessage(msg);
        }

        if (actual_valve_pos > state_valve_pos)
        {
            // valve needs to close with difference. Check if within movements limits is done in move_valve function
            move = actual_valve_pos - state_valve_pos;
            sum_move = sum_move + move;
            direction = 0;
        }
        else
        {
            // valve needs to open with difference. Check if within movement limits is done in move_valve function
            move = state_valve_pos - actual_valve_pos;
            sum_move = sum_move + move;
            direction = 1;
        }

        if (valve_control_data_mutex && xSemaphoreTake(valve_control_data_mutex, (TickType_t)10) == pdTRUE)
        {
            valvecontroldata.valve_number[i] = valve_number;
            valvecontroldata.position_change[i] = move;
            valvecontroldata.direction[i] = direction;
            xSemaphoreGive(valve_control_data_mutex);
        }
    }

    if (valve_control_data_mutex && xSemaphoreTake(valve_control_data_mutex, (TickType_t)10) == pdTRUE)
    {
        if (sum_move == 0)
        {
            valvecontroldata.write_new_position = 0;
            valvecontroldata.check_position = 1;
        }
        else
        {
            valvecontroldata.write_new_position = 1;
            valvecontroldata.check_position = 1;
        }
        xSemaphoreGive(valve_control_data_mutex);
    }

    // finally the valves can be moved but function move_valve() should only be called if sum_move > 0
    if (sum_move > 0)
    {
        snprintf(msg, sizeof(msg), "[INFO] Valve move sum is > 0 (%d). Call function to move valves.", sum_move);
        printmessage(msg);
        move_valve();
    }
    else if (sum_move < 0)
    {
        snprintf(msg, sizeof(msg), "[ERROR] Valve move sum is < 0 (%d). This should not happen, check code.", sum_move);
        printmessage(msg);
    }
    else
    {
        snprintf(msg, sizeof(msg), "[ERROR] Valve move sum is %d. No valve movement required..", sum_move);
        printmessage(msg);
    }
}
