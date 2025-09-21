#include "task_i2c.h"



void start_task_i2c(void)
{
    xTaskCreate(task_i2c_code, "taski2c", 10000, NULL, 6, &task_i2c);
}

void task_i2c_code(void *pvParameters)
{
    int read_sensors_multiplier = 0;
    int current_time_multiplier = 0;
    int rtc_time_multiplier = 0;
    int sync_time_multiplier = 0;
    int lcd_i2c_address = 0;
    char date_time_buffer[50] = {};
    char uptime_str[64] = {};
    char msg[MSG_SIZE] = {};
    const TickType_t timedelay = 10; // main time delay im ms

    pinMode(pushButton_pin, INPUT);
    attachInterrupt(pushButton_pin, lcd_baclight_pb_isr, RISING);

    // start with display clear and no backlight
    init_display_off();

    // Sync time
    sync_rtc_ntp();

    if (settings_i2c_mutex && xSemaphoreTake(settings_i2c_mutex, (TickType_t)10) == pdTRUE)
    {
        lcd_i2c_address = i2csettings.display_i2c_address;
        xSemaphoreGive(settings_i2c_mutex);
    }

    for (;;)
    {
        read_sensors_multiplier++;
        current_time_multiplier++;
        rtc_time_multiplier++;
        sync_time_multiplier++;

        if (read_sensors_multiplier == 500)
        { // read every 5 seconds
            read_sensors();
            sensor_data_average();
            read_sensors_multiplier = 0;
        }

        if (current_time_multiplier == 500)
        { // read every 5 seconds
            current_time();
            current_time_multiplier = 0;
        }

        if (rtc_time_multiplier == 6000)
        { // Every 60 seconds

            // formatted_daydatetime(date_time_buffer, sizeof(date_time_buffer));
            // message = "Local time is: " + String(date_time_buffer);
            // print_message(message);

            formatted_uptime(uptime_str, sizeof(uptime_str));
            //message = "System uptime: " + String(uptime_str);
            
            snprintf(msg, sizeof(msg), "System uptime: %s.", uptime_str);
            printmessage(LOG_INFO, msg);

            rtc_time_multiplier = 0;
        }

        if (sync_time_multiplier == 30000)
        { // Every 30 mins
            snprintf(msg, sizeof(msg), "Sync RTC with NTP server.");
            printmessage(LOG_INFO, msg);
            sync_rtc_ntp();
            sync_time_multiplier = 0;
        }

        // When pushbutton is pushed, toggle will be true and function to display status is started
        if (pb_toggle == true)
        {
            snprintf(msg, sizeof(msg), "Pushbutton pressed. Start task display.");
            printmessage(LOG_INFO, msg);
            pb_start_display();
        }

        vTaskDelay(timedelay);
    }
}
