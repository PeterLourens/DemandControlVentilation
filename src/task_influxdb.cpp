#include "task_influxdb.h"

void start_task_influxdb(void)
{

    xTaskCreate(task_influxdb_code, "task_influxdb", 10000, NULL, 3, &task_influxdb);
}

void task_influxdb_code(void *pvParameters)
{
    bool ap_active_temp = 0;
    char enable_influxdb[SMALL_CONFIG_ITEM] = {};
    String message = "";

    for (;;)
    {
        vTaskDelay(10000);

        if (ap_active_mutex && xSemaphoreTake(ap_active_mutex, (TickType_t)10) == pdTRUE)
        {
            ap_active_temp = ap_active;
            xSemaphoreGive(ap_active_mutex);
        }

        if (settings_influxdb_mutex && xSemaphoreTake(settings_influxdb_mutex, (TickType_t)10) == pdTRUE)
        {
            strncpy(enable_influxdb, influxdbsettings.enable_influxdb, sizeof(enable_influxdb) - 1);
            enable_influxdb[sizeof(enable_influxdb) - 1] = '\0';
            xSemaphoreGive(settings_influxdb_mutex);
        }

        if (WiFi.waitForConnectResult() == WL_CONNECTED && ap_active_temp == 0 && strcmp(enable_influxdb,"On") == 0)
        {
            message = "Writing to InfluxDB...";
            print_message(message);

            write_sensor_data();
            write_avg_sensor_data();
            write_valve_position_data();
            write_system_uptime();
            write_state_info();
            write_fanspeed();
            write_heap_info();
        }
    }
}