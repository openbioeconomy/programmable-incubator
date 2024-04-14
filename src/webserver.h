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
const char* WIFI_PASSWORD = "44ER8RTH";

void webSvrInitWiFi();

void webSvrInitWebServer();

void webSvrInitWebSocket();

void webSvrOnRootRequest(AsyncWebServerRequest *request);

void webSvrNotifyWebsocketClients(String message);

void webSvrWebsocketOnEvent(AsyncWebSocket *server,
                            AsyncWebSocketClient *client,
                            AwsEventType type,
                            void *arg,
                            uint8_t *data,
                            size_t len);


void webSvrOnRootRequest(AsyncWebServerRequest *request);

void webSvrHandleWebSocketMessage(void *arg, uint8_t *data, size_t len);

#endif /* SERVER_H_ */