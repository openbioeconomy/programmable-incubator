#include "server.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(HTTP_PORT);
// Create websocket
AsyncWebSocket ws("/ws");

void serverInitWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("Trying to connect [%s] ", WiFi.macAddress().c_str());
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.printf(" %s\n", WiFi.localIP().toString().c_str());
}

void serverInitWebServer() {
    server.on("/", HTTP_GET, serverOnRootRequest);
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
}

void serverInitWebSocket() {
    ws.onEvent(serverWebsocketOnEvent);
    server.addHandler(&ws); 
}

void serverOnRootRequest(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
}

void serverNotifyWebsocketClients(String message) {
    ws.textAll(message);
}

void serverWebsocketOnEvent(AsyncWebSocket       *server,
             AsyncWebSocketClient *client,
             AwsEventType          type,
             void                 *arg,
             uint8_t              *data,
             size_t                len) {

    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            serverHandleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void serverHandleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        Serial.println("test");
    }
}

