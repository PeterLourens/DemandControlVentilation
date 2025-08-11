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

void notifyClients(String json)
{
    ws.textAll(json);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    String page_name = "";
    String message = "";

    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        for (size_t i = 0; i < len; i++)
        {
            page_name += (char)data[i];
        }
        if (page_name == "index")
        {
            String json = create_index_json();
            notifyClients(json);
        }
        else
        {
            page_name= "unknown page";
        }
        message = "Request json for page: " + page_name;
        print_message(message);
    }
}

void onEvent(AsyncWebSocket *ws_server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    //String page_name = "index";

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
