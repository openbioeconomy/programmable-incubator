#include "webserver.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(HTTP_PORT);
// Create websocket
AsyncWebSocket ws("/ws");

void webSvrOnRootRequest(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
}

void webSvrWebsocketOnEvent(AsyncWebSocket       *server,
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
            webSvrHandleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void webSvrInitWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("Trying to connect [%s] ", WiFi.macAddress().c_str());
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.printf(" %s\n", WiFi.localIP().toString().c_str());
}

void webSvrInitWebServer() {
    server.on("/", HTTP_GET, webSvrOnRootRequest);
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
}

void webSvrInitWebSocket() {
    ws.onEvent(webSvrWebsocketOnEvent);
    server.addHandler(&ws); 
}

void webSvrNotifyWebsocketClients(String message) {
    ws.textAll(message);
}

void webSvrHandleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        //DeserializationError err = deserializeJson(webSvrBufRx, data);
        //if (err) {
        //    Serial.print(F("deserializeJson() failed with code "));
        //    Serial.println(err.c_str());
        //    return;
        //}
    }
}

