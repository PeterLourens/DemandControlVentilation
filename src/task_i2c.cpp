#include "task_i2c.h"

void start_task_i2c(void) {

    xTaskCreate(task_i2c_code, "taski2c", 30000, NULL, 6, &task_i2c);
}

void task_i2c_code(void * pvParameters)
{  
    int read_sensors_multiplier = 0;
    int current_time_multiplier = 0;
    int display_time_multiplier = 0;
    int rtc_time_multiplier = 0;
    int sync_time_multiplier = 0;
    const TickType_t timedelay = 10;                     // main time delay im ms

    pinMode(pushButton_pin, INPUT);
    attachInterrupt(pushButton_pin, lcd_baclight_pb_isr, RISING);

    //start with display clear and no backlight
    init_display_off();

    for(;;) {
        read_sensors_multiplier++;
        current_time_multiplier++;
        rtc_time_multiplier++;
        sync_time_multiplier++;
     
        if (read_sensors_multiplier == 500) {           //read every 5 seconds
            read_sensors();
            sensor_data_average();
            read_sensors_multiplier = 0;
        }

        if (current_time_multiplier == 500) {           //read every 5 seconds
            current_time();
            current_time_multiplier = 0;
        }
        
        if (rtc_time_multiplier == 2000) {             //Every 20 seconds
            Serial.print("\nUpdate time....");
            Serial.print("\nLocal time is: ");
            String temp_time = current_time();
            Serial.print(temp_time);
            Serial.print("\nSystem uptime: ");
            Serial.print(esp_timer_get_time()/1000000/60);
            Serial.print(" min");
            rtc_time_multiplier = 0;
        }

        if (sync_time_multiplier == 360000) {         //Every hour
            Serial.print("\nSync RTC with NTP server...");
            sync_rtc_ntp();
            sync_time_multiplier = 0;
        }

        //When pushbutton is pushed, toggle will be true and function to display status is started
        if (pb_toggle == true) {
            Serial.print("\nStart task display....");
            pb_start_display();
        }

        vTaskDelay(timedelay);
    } 
}



