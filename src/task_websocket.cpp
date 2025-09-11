#include "task_websocket.h"

AsyncWebServer ws_server(8080);
WebSerial webSerial;
AsyncWebSocket ws("/ws");

void start_task_websocket(void)
{
    xTaskCreate(task_websocket_code, "taskwebsocket", 20000, NULL, 1, &task_websocket);
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
            Serial.print(msg);
            vTaskDelay(20);
            webSerial.print(msg);
            vTaskDelay(20);
        }
        // Serial.print("fAlarm ");
        // Serial.print(uxTaskGetStackHighWaterMark(NULL));
        // Serial.println();
        // Serial.flush();
        vTaskDelay(500);
    }
}

void notifyClients(String json)
{
    ws.textAll(json);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    char page_name[MEDIUM_CONFIG_ITEM] = {}; // Adjust size as needed
    char msg[MSG_SIZE] = {};

    AwsFrameInfo *info = (AwsFrameInfo *)arg;

    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        // Build page_name from incoming data
        size_t copy_len = (len < sizeof(page_name) - 1) ? len : sizeof(page_name) - 1;
        memcpy(page_name, data, copy_len);
        page_name[copy_len] = '\0';

        // Generate JSON based on page_name
        if (strcmp(page_name, "index") == 0)
        {
            temp_settings_char[0] = '\0';
            create_index_json();
            ws.textAll(temp_settings_char);
        }
        else if (strcmp(page_name, "settings") == 0)
        {
            temp_settings_char[0] = '\0';
            create_settings_json();
            ws.textAll(temp_settings_char);
        }
        else if (strcmp(page_name, "sensor_config") == 0)
        {
            temp_settings_char[0] = '\0';
            create_sensors_json();
            ws.textAll(temp_settings_char);
        }
        else if (strcmp(page_name, "statemachine") == 0)
        {
            temp_settings_char[0] = '\0';
            create_statemachine_json();
            ws.textAll(temp_settings_char);
        }
        else if (strcmp(page_name, "valvecontrol") == 0)
        {
            temp_settings_char[0] = '\0';
            create_valvecontrol_json();
            ws.textAll(temp_settings_char);
        }
        else
        {
            strncpy(page_name, "Page without form data requested, nothing to transmit over websocket", sizeof(page_name) - 1);
            page_name[sizeof(page_name) - 1] = '\0';
        }

        snprintf(msg, sizeof(msg), "Request json for page: %s", page_name);
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
