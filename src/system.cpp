#include "system.h"

/*void task_list(void)
{

    char ptrTaskList[250];

    vTaskList(ptrTaskList);

    Serial.println(F("**********************************"));
    Serial.println(F("Task  State   Prio    Stack    Num"));
    Serial.println(F("**********************************"));
    Serial.print(ptrTaskList);
    Serial.println(F("**********************************"));


    char buffer1[2048] = {0};
    vTaskList(buffer1);
    Serial.println("\nTasklist: ");
    Serial.println(buffer1);
    Serial.println("\n");
    vTaskDelay(10000);

    //available_heap_space_bytes = xAvailableHeapSpaceInBytes();
    //Serial.print("\nAvailble heap space (bytes): ");
    //Serial.print(available_heap_space_bytes);

    //HeapStats_t xHeapStats;
    //vPortGetHeapStats(&xHeapStats);

    //printf("Available heap space: %u\n", xHeapStats.xAvailableHeapSpaceInBytes);
    //printf("Number of allocations: %u\n", xHeapStats.xNumberOfSuccessfulAllocations);
    //printf("Number of frees: %u\n", xHeapStats.xNumberOfSuccessfulFrees);
}*/