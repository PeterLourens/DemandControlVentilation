#include "task_websocket.h"

AsyncWebServer ws_server(8080);
WebSerial webSerial;
AsyncWebSocket ws("/ws");

typedef String (*PageHandler)();

typedef struct
{
    const char *page_name;
    PageHandler handler;
} PageRoute;

PageRoute pageRoutes[] = {
    {"index", create_index_json},
    {"settings", create_settings_json},
    {"valvecontrol", create_valvecontrol_json},
    {"sensors", create_sensors_json},
    {"statemachine", create_statemachine_json}};

const int pageRoutesCount = sizeof(pageRoutes) / sizeof(PageRoute);

// Lookup function
PageHandler getPageHandler(const char *name)
{
    for (int i = 0; i < pageRoutesCount; ++i)
    {
        if (strcmp(pageRoutes[i].page_name, name) == 0)
        {
            return pageRoutes[i].handler;
        }
    }
    return NULL;
}

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

    webSerial.begin(&ws_server);
    webSerial.setBuffer(500);
    ws.onEvent(onEvent);
    ws_server.addHandler(&ws);
    ws_server.begin();

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

    AwsFrameInfo *info = (AwsFrameInfo *)arg;

    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        for (size_t i = 0; i < len; i++)
        {
            page_name += (char)data[i];
        }

        if (page_name == "web_serial")
        {
            return; // Skip this page
        }

        PageHandler handler = getPageHandler(page_name.c_str());
        if (handler != NULL)
        {
            String json = handler();
            print_message("JSON sent over websocket to client:");
            print_message(json);
            notifyClients(json);
        }
        else
        {
            Serial.println("Unknown page: " + page_name);
        }
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
