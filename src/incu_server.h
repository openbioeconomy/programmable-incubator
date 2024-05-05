#ifndef INCU_SERVER_H_
#define INCU_SERVER_H_

#include "Arduino.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "incu__defs.h"

// Pointer for WebSocket data
static struct IncuData *incuServerRxData;
// Create AsyncWebServer object on port 80
static AsyncWebServer server(INCU_HTTP_PORT);
// Create WebSocket
static AsyncWebSocket ws("/ws");

void incuServerInitSpiffs();

void incuServerInitWebserver();

void incuServerInitWebSocket(IncuData*);

void incuServerOnRootRequest(AsyncWebServerRequest *request);

void incuServerNotifiyWebSocketClients(String message);

void incuServerWebSocketOnEvent(AsyncWebSocket *server,
                            AsyncWebSocketClient *client,
                            AwsEventType type,
                            void *arg,
                            uint8_t *data,
                            size_t len);

void incuServerOnRootRequest(AsyncWebServerRequest *request);

void incuServerHandleWebSocketMessage(void *arg, uint8_t *data, size_t len);

#endif /* ICU_SERVER_H_ */