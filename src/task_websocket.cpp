#include "task_websocket.h"

AsyncWebServer ws_server(8080);
WebSerial webSerial;
AsyncWebSocket ws("/ws");

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

    // webSerial.onMessage([](const std::string &msg){ Serial.println(msg.c_str()); });
    webSerial.begin(&ws_server);
    webSerial.setBuffer(500);
    ws.onEvent(onEvent);
    ws_server.addHandler(&ws);
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

String create_json()
{
    String json_test = "{\"valve0\":0, \"valve1\":0, \"valve2\":0, \"valve3\":0, \"valve4\":0, \"valve5\":0, \"valve6\":0, \"valve7\":0, \"valve8\":0, \"valve9\":0, \"valve10\":0, \"valve11\":0}";
    return json_test;
}

void notifyClients(String json)
{
    ws.textAll(json);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        String json = create_json();
        Serial.print(json);
        notifyClients(json);
    }
}

void onEvent(AsyncWebSocket *ws_server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

