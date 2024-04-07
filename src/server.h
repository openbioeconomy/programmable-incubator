#ifndef SERVER_H_
#define SERVER_H_

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "Arduino.h"
#include <SPIFFS.h>

#define HTTP_PORT 80

class IncubatorServer {
    public:
        IncubatorServer();
        void begin();
        const char *wifiSSID = "Makespace";
        const char *wifiPass = "getexc1tedandmaketh1ngs";
        
    private:
        void initWiFi();
        void initWebServer();
        void initWebSocket();
        bool initSensor();
        //void onRootRequest(AsyncWebServerRequest *request);
        void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
        void notifyClients(String message); 
        void onEvent(AsyncWebSocket *server,
                     AsyncWebSocketClient *client,
                     AwsEventType type,
                     void *arg,
                     uint8_t *data,
                     size_t len);
        void onRootRequest(AsyncWebServerRequest *request);
        AsyncWebServer *_server = NULL;
        AsyncWebSocket *_ws = NULL;
};

#endif /* SERVER_H_ */