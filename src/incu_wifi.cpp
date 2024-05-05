#include "incu_wifi.h"

void incuWifiInit() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(INCU_WIFI_SSID, INCU_WIFI_PASSWORD);
    Serial.printf("Trying to connect [%s] ", WiFi.macAddress().c_str());
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.printf(" %s\n", WiFi.localIP().toString().c_str());
}