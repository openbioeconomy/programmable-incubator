#include "incu_server.h"

void incuServerInitSpiffs() {
  if (!SPIFFS.begin()) {
    Serial.println("Cannot mount SPIFFS volume...");
  }
}

void incuServerOnRootRequest(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/control.html", "text/html");
}

void incuServerWebSocketOnEvent(AsyncWebSocket       *server,
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
            incuServerHandleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void incuServerInitWebserver() {
    incuServerInitSpiffs();
    server.on("/", HTTP_GET, incuServerOnRootRequest);
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
}

void incuServerInitWebSocket(IncuData* rxData) {
    incuServerRxData = rxData;
    ws.onEvent(incuServerWebSocketOnEvent);
    server.addHandler(&ws); 
}

void incuServerNotifiyWebSocketClients(String message) {
    ws.textAll(message);
}

void incuServerHandleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {

        JsonDocument jsonRx;
        DeserializationError err = deserializeJson(jsonRx, data);
        if (err) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(err.c_str());
            return;
        }
        
        if (jsonRx["setpoint"]) {
            incuServerRxData->setpoint = (float)jsonRx["setpoint"];
            incuServerRxData->cmd = INCU_SET_SETPOINT;
        }

        if (jsonRx["enable"]) {
            incuServerRxData->cmd = INCU_ENABLE;
        }

        if (jsonRx["disable"]) {
            incuServerRxData->cmd = INCU_DISABLE;
        }

    }
}







