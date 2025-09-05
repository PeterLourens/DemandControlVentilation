#include "task_system.h"

void start_task_system(void)
{

    xTaskCreate(task_system_code, "taskSystem", 15000, NULL, 1, &task_sys);
}

void task_system_code(void *pvParameters)
{
    int free_heap_size = 0;
    int minimum_ever_free_heap_size = 0;

    String message = "";

    for (;;)
    {
        free_heap_size = get_free_heap_size();
        message = "Free heap size: " + String(free_heap_size);
        print_message(message);

        minimum_ever_free_heap_size = get_min_ever_heap_size();
        message = "Minimum ever free heap size: " + String(minimum_ever_free_heap_size);
        print_message(message);

        vTaskDelay(120000);
    }
}