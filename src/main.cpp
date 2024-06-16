/**
 * ----------------------------------------------------------------------------
 * ESP32 Incubator Control with WebSocket
 * ----------------------------------------------------------------------------
 * © 2020 Richard Hopper
 * ----------------------------------------------------------------------------
 */

#include <Arduino.h>
#include "incubator.h"
#include <list>

Incubator incubator;

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

void setup() {
    // Blink Code 14 Apirl 2024
    pinMode(LED_BUILTIN, OUTPUT);
    
    Serial.begin(115200);
    incubator.begin(10000);

    std::list<double> outputList;
}

// ----------------------------------------------------------------------------
// Main control loop
// ----------------------------------------------------------------------------

void loop() {
    // Blink Code Below
    // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    // delay(2000);              // wait for a second
    // digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    // delay(2000);              // wait for a second
    
    incubator.run();
    delay(100);
}

