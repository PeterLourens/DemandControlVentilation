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
    char datetime_buffer[40];
    char msg[150] = {};

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
            formatted_datetime(datetime_buffer, sizeof(datetime_buffer));
            snprintf(msg, sizeof(msg), "\n%s %s", datetime_buffer, rxBuffer);
            // Serial.print("\n");
            Serial.print(msg);
            vTaskDelay(20);
            // webSerial.print("\n");
            webSerial.print(msg);
            vTaskDelay(20);
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
    String json = "";

    char msg[MSG_SIZE] = {};

    AwsFrameInfo *info = (AwsFrameInfo *)arg;

    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        // Compile page_name from js
        for (size_t i = 0; i < len; i++)
        {
            page_name += (char)data[i];
        }

        // generate json based on page_name
        if (page_name == "index")
        {
            String json = create_index_json();
            notifyClients(json);
        }
        else if (page_name == "settings")
        {
            json = create_settings_json();
            notifyClients(json);
        }
        else if (page_name == "sensor_config")
        {
            json = create_sensors_json();
            notifyClients(json);
        }
        else if (page_name == "statemachine")
        {
            json = create_statemachine_json();
            notifyClients(json);
        }
        else if (page_name == "valvecontrol")
        {
            json = create_valvecontrol_json();
            notifyClients(json);
        }
        else
        {
            page_name = "[INFO] Page without form data requested, nothing to transmit over websocket";
        }
        snprintf(msg, sizeof(msg), "[INFO] Request json for page: %d", page_name);
        printmessage(LOG_INFO, msg);
    }
}

void onEvent(AsyncWebSocket *ws_server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        // Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        // Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}
