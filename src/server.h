#ifndef SERVER_H_
#define SERVER_H_

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "Arduino.h"
#include <SPIFFS.h>

#define HTTP_PORT 80

// WiFi network credentials
const char* WIFI_SSID = "TALKTALK42E7AF";
const char* WIFI_PASSWORD = ;

void serverInitWiFi();

void serverInitWebServer();

void serverInitWebSocket();

void serverOnRootRequest(AsyncWebServerRequest *request);

void serverNotifyWebsocketClients(String message);

void serverWebsocketOnEvent(AsyncWebSocket *server,
                            AsyncWebSocketClient *client,
                            AwsEventType type,
                            void *arg,
                            uint8_t *data,
                            size_t len);

void serverHandleWebSocketMessage(void *arg, uint8_t *data, size_t len);

#endif /* SERVER_H_ */