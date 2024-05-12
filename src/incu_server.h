#ifndef INCU_SERVER_H_
#define INCU_SERVER_H_

#include "Arduino.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "incu__defs.h"

// Pointer for WebSocket rx data from client 
static struct IncuWrite *incuWrite;
// Pointer for the system state 
static struct IncuConfig *incuConfig;
// Create AsyncWebServer object on port 80
static AsyncWebServer server(INCU_HTTP_PORT);
// Create WebSocket
static AsyncWebSocket ws("/ws");

void incuServerInitSpiffs();

void incuServerInitWebserver();

void incuServerInitWebSocket(IncuWrite*, IncuConfig*);

void incuServerOnRootRequest(AsyncWebServerRequest *request);

void incuServerWebSocketOnEvent(AsyncWebSocket *server,
                            AsyncWebSocketClient *client,
                            AwsEventType type,
                            void *arg,
                            uint8_t *data,
                            size_t len);

void incuServerOnRootRequest(AsyncWebServerRequest *request);

void incuServerHandleWebSocketMessage(void *arg, uint8_t *data, size_t len);

void incuServerUpdateWebSocketClients();

#endif /* ICU_SERVER_H_ */