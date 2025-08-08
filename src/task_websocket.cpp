#include "task_websocket.h"

AsyncWebServer ws_server(8080);
WebSerial webSerial;

void start_task_websocket(void)
{
    xTaskCreate(task_websocket_code, "taskwebsocket", 10000, NULL, 1, &task_websocket);
}

void task_websocket_code(void *pvParameters)
{

    char rxBuffer[400];
    String datetime = "";
    String rxString = "";
    String message = "";

    webSerial.onMessage([](const std::string &msg)
                        { Serial.println(msg.c_str()); });
    webSerial.begin(&ws_server);
    webSerial.setBuffer(500);
    ws_server.begin();

    // Loop code for the task
    for (;;)
    {
        if (xQueueReceive(webserial_queue, rxBuffer, 50) == pdPASS)
        {
            rxString = String(rxBuffer);
            datetime = formatted_datetime();
            message = datetime + " - " + rxString;
            Serial.print("\n" + message);
            webSerial.print("\n" + message);
        }
        vTaskDelay(500);
    }
}


