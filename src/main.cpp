/**
 * ----------------------------------------------------------------------------
 * ESP32 Remote Control with WebSocket
 * ----------------------------------------------------------------------------
 * © 2020 Stéphane Calderoni
 * ----------------------------------------------------------------------------
 */

#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>

// ----------------------------------------------------------------------------
// Definition of macros
// ----------------------------------------------------------------------------

#define HTTP_PORT 80

// ----------------------------------------------------------------------------
// Definition of global constants
// ----------------------------------------------------------------------------


// WiFi credentials
const char *WIFI_SSID = "Makespace";
const char *WIFI_PASS = "getexc1tedandmaketh1ngs";

// ----------------------------------------------------------------------------
// Definition of global variables
// ----------------------------------------------------------------------------

unsigned long previousMillis = 0;  // will store last updated
const long INTERVAL = 1000;  // interval to update
JsonDocument readings; // Json Variable to Hold Sensor Readings
char buffer[64]; // Buffer for ws messages
// Allocate the JSON document
JsonDocument jsonRx;

AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws("/ws");

// ----------------------------------------------------------------------------
// SPIFFS initialization
// ----------------------------------------------------------------------------

void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("Cannot mount SPIFFS volume...");
  }
}

// ----------------------------------------------------------------------------
// Connecting to the WiFi network
// ----------------------------------------------------------------------------

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.printf("Trying to connect [%s] ", WiFi.macAddress().c_str());
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
  }
  Serial.printf(" %s\n", WiFi.localIP().toString().c_str());
}

// ----------------------------------------------------------------------------
// Web server initialization
// ----------------------------------------------------------------------------

void onRootRequest(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/index.html", "text/html");
}

void initWebServer() {
    server.on("/", onRootRequest);
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
}

// ----------------------------------------------------------------------------
// WebSocket initialization
// ----------------------------------------------------------------------------

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {

        // const uint8_t size = JSON_OBJECT_SIZE(1);
        //char testData[] =
          //  "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
        JsonDocument jsonDoc;
        DeserializationError err = deserializeJson(jsonDoc, (char*)data);
        
        if (err) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(err.c_str());
            return;
        }
        
        // Fetch values.

        //const char* sensor = jsonDoc["sensor"];
        //long time = jsonDoc["time"];
        int val = jsonDoc["testData"][0];
        //double longitude = jsonDoc["data"][1];

        // Print values.
        Serial.println(val);
        //Serial.println(time);
        //Serial.println(latitude, 6);
        //Serial.println(longitude, 6);
    }
}

void onEvent(AsyncWebSocket       *server,
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

void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

void notifyClients(String message) {
  ws.textAll(message);
}

// ----------------------------------------------------------------------------
// Sensor
// ----------------------------------------------------------------------------
String getSensorReadings(){
    readings["temperature"] = String(random(100));
    readings["humidity"] = String(random(100));
    serializeJson(readings, buffer, 64);
    return buffer;
}

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

void setup() {
    
    Serial.begin(115200); delay(500);

    initSPIFFS();
    initWiFi();
    initWebSocket();
    initWebServer();
}

// ----------------------------------------------------------------------------
// Main control loop
// ----------------------------------------------------------------------------

void loop() {
    ws.cleanupClients();
        
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= INTERVAL) {
        
        String sensorReadings = getSensorReadings();
        notifyClients(sensorReadings);

        previousMillis = currentMillis;
    }
    
}