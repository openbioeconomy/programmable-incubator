/**
 * ----------------------------------------------------------------------------
 * ESP32 Incubator Control with WebSocket
 * ----------------------------------------------------------------------------
 * © 2020 Richard Hopper
 * ----------------------------------------------------------------------------
 */

#include <Arduino.h>
#include "incubator.h"
#include "incu_server.h"
#include "incu_wifi.h"
#include "incu__defs.h"

Incubator incu;

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);
    incu.begin(10000);
    incuWifiInit();
    incuServerInitWebserver();
    incuServerInitWebSocket(&incu.write, &incu.config);
}

// ----------------------------------------------------------------------------
// Main control loop
// ----------------------------------------------------------------------------

void loop() {
    incu.run();
    incu.processCmds();
    incuServerUpdateWebSocketClients();
    delay(10);
}