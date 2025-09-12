#include "task_neopixel.h"

void start_task_neopixel(void) {

    xTaskCreate(task_neopixel_code, "tasknp", 10000, NULL, 6, &task_np);

}

void task_neopixel_code(void * pvParameters) {

    int pixel = 0;
    int brightness = 64;
    bool temp_valve_move_lock = 0;
    
    String temp_fanspeed = "";
    String temp_state = "";

    for(;;) {       
        if (statemachine_state_mutex != NULL) {
            if(xSemaphoreTake(statemachine_state_mutex, ( TickType_t ) 10 ) == pdTRUE) {
                temp_state = state;
                xSemaphoreGive(statemachine_state_mutex);
            }
        }

        if (lock_valve_move_mutex != NULL) {
            if(xSemaphoreTake(lock_valve_move_mutex, ( TickType_t ) 10 ) == pdTRUE) { 
                temp_valve_move_lock = lock_valve_move;
                xSemaphoreGive(lock_valve_move_mutex);
            }
        }

        if (fanspeed_mutex != NULL) {
            if(xSemaphoreTake(fanspeed_mutex, ( TickType_t ) 10 ) == pdTRUE) { 
                temp_fanspeed = fanspeed;
                xSemaphoreGive(fanspeed_mutex);
            }
        }

        //1st LED for status of statemachine
        if (temp_state == "init") {
            pixel = 0;
            led_red_blink(pixel, brightness);
        }
        else if (temp_state == "day" && temp_valve_move_lock == 0) {
            pixel = 0;
            led_green_on(pixel, brightness);
        }
        else if (temp_state == "day" && temp_valve_move_lock == 1) {
            pixel = 0;
            led_green_blink(pixel, brightness);
        }
        else if (temp_state == "night" && temp_valve_move_lock == 0) {
            pixel = 0;
            led_blue_on(pixel, brightness);
        }
        else if (temp_state == "night" && temp_valve_move_lock == 1) {
            pixel = 0;
            led_blue_blink(pixel, brightness);
        }
        else if (temp_state == "cyclingday" && temp_valve_move_lock == 0) {
            pixel = 0;
            led_yellow_on(pixel, brightness);
        }
        else if (temp_state == "cyclingday" && temp_valve_move_lock == 1) {
            pixel = 0;
            led_yellow_blink(pixel, brightness);
        }
        else if (temp_state == "cyclingnight" && temp_valve_move_lock == 0) {
            pixel = 0;
            led_purple_on(pixel, brightness);
        }
        else if (temp_state == "cyclingnight" && temp_valve_move_lock == 1) {
            pixel = 0;
            led_purple_blink(pixel, brightness);
        }
        else if (temp_state == "cooking" && temp_valve_move_lock == 0) {
            pixel = 0;
            led_white_on(pixel, brightness);
        }
        else if (temp_state == "cooking" && temp_valve_move_lock == 1) {
            pixel = 0;
            led_white_blink(pixel, brightness);
        }
        else if (temp_state == "stopped") {
            pixel = 0;
            led_purple_blink(pixel, brightness);
        }
        else {
            pixel = 0;
            led_red_on(pixel, brightness);

        }

        //2nd LED for fanspeed
        if (temp_fanspeed == "Low") {
            pixel = 1;
            led_green_on(pixel, brightness);
        }
        else if (temp_fanspeed == "Medium") {
            pixel = 1;
            led_yellow_on(pixel, brightness);
        }
        else if (temp_fanspeed == "High") {
            pixel = 1;
            led_red_on(pixel, brightness);
        }
        else {
            pixel = 1;
            led_red_blink(pixel, brightness);
        }
    }

}  