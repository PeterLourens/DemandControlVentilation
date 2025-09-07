#include "task_system.h"

void start_task_system(void)
{

    xTaskCreate(task_system_code, "taskSystem", 15000, NULL, 1, &task_sys);
}

void task_system_code(void *pvParameters)
{
    int free_heap_size = 0;
    int minimum_ever_free_heap_size = 0;

    char msg[MSG_SIZE] = {};

    for (;;)
    {
        free_heap_size = get_free_heap_size();
        snprintf(msg, sizeof(msg), "[INFO] Free heap size: %d",free_heap_size);
        printmessage(msg);

        minimum_ever_free_heap_size = get_min_ever_heap_size();
        snprintf(msg, sizeof(msg), "[INFO] Minimum ever free heap size: %d",minimum_ever_free_heap_size);
        printmessage(msg);

        vTaskDelay(120000);
    }
}