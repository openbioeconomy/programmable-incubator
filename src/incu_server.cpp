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

void incuServerInitWebSocket(IncuWrite* write, IncuConfig* config) {
    incuConfig = config;
    incuWrite = write;
    ws.onEvent(incuServerWebSocketOnEvent);
    server.addHandler(&ws);
}

void incuServerUpdateWebSocketClients() {
    if(incuConfig->newData == true){
        JsonDocument stateJson;
        char dataCharArray[16];
        char jsonCharArray[256];
        snprintf(dataCharArray, sizeof(dataCharArray),"%0.1f", incuConfig->temperature);
        stateJson["state"]["temperature"] = dataCharArray;
        snprintf(dataCharArray, sizeof(dataCharArray),"%0.1f", incuConfig->humidity);
        stateJson["state"]["humidity"] = dataCharArray;
        snprintf(dataCharArray, sizeof(dataCharArray),"%0.1f", incuConfig->setpoint);
        stateJson["state"]["setpoint"] = dataCharArray;
        //stateJson["state"]["enable"] = String(incuConfig->enable);
        
        serializeJson(stateJson, jsonCharArray);
        Serial.println(jsonCharArray);
        ws.textAll(jsonCharArray);
        incuConfig->newData = false;
    }
}

void incuServerHandleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        Serial.println((char*)data);
        JsonDocument jsonRx;
        DeserializationError err = deserializeJson(jsonRx, data);
        if (err) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(err.c_str());
            return;
        }
        
        if (jsonRx["setpoint"]) {
            incuWrite->setpoint = (float)jsonRx["setpoint"];
            incuWrite->cmd = INCU_SET_SETPOINT;
        }

        if (jsonRx["enable"]) {
            incuWrite->cmd = INCU_ENABLE_CONST_TEMP;
        }

        if (jsonRx["disable"]) {
            incuWrite->cmd = INCU_DISABLE;
        }

        if (jsonRx["save"]) {
            incuWrite->cmd = INCU_SAVE_CONFIG;
            Serial.println("test");
        }

    }
}







