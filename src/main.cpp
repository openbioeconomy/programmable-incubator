/**
 * ----------------------------------------------------------------------------
 * ESP32 Incubator Control with WebSocket
 * ----------------------------------------------------------------------------
 * © 2020 Richard Hopper
 * ----------------------------------------------------------------------------
 */

#include <Arduino.h>
#include "incubator.h"

Incubator incubator;

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

void setup() {
    // Blink Code
    pinMode(LED_BUILTIN, OUTPUT);
    
    Serial.begin(115200);
    incubator.begin(10000);
}

// ----------------------------------------------------------------------------
// Main control loop
// ----------------------------------------------------------------------------

void loop() {
    // Blink Code Below
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
    
    incubator.run();
    delay(100);
}