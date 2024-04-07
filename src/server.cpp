#include "server.h"

IncubatorServer::IncubatorServer()
{
   
}
/// @brief 

void IncubatorServer::begin(){
}

void IncubatorServer::initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSSID, wifiPass);
    Serial.printf("Trying to connect [%s] ", WiFi.macAddress().c_str());
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.printf(" %s\n", WiFi.localIP().toString().c_str());
}

void IncubatorServer::initWebServer() {
    _server = new AsyncWebServer(HTTP_PORT);
    _server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
        request->send(SPIFFS, "/index.html", "text/html");
    });
    _server->serveStatic("/", SPIFFS, "/");
    _server->begin();
}

void IncubatorServer::initWebSocket() {
    _ws = new AsyncWebSocket("/ws");
    _ws->onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        onEvent(server, client, type, arg, data, len);
    });
    _server->addHandler(_ws); 
}

void IncubatorServer::onEvent(AsyncWebSocket       *server,
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
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

//void Incubator::onRootRequest(AsyncWebServerRequest *request) {
  //request->send(SPIFFS, "/index.html", "text/html");
//}

void IncubatorServer::handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        Serial.println("test");
    }
}

void IncubatorServer::notifyClients(String message) {
    _ws->textAll(message);
}


