/**
 * ----------------------------------------------------------------------------
 * ESP32 Incubator Control 
 * ----------------------------------------------------------------------------
 * © 2020 Richard Hopper
 * ----------------------------------------------------------------------------
 */

#include <Arduino.h>
#include "incu_control.h"
#include "sched.h"
#include "command_parser.h"

IncuControl incuControl;
CommandParser commandParser;
Sched sched;

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(100000);
    incuControl.begin(Wire, 0x44);
    commandParser.begin(sched, incuControl);
    sched.begin(incuControl);
}

// ----------------------------------------------------------------------------
// Main control loop
// ----------------------------------------------------------------------------

void loop() 
{
    if (Serial.available()) 
    {
        // Allocate the JSON document.
        JsonDocument doc;
        // Deserialise.
        DeserializationError err = deserializeJson(doc, Serial);
        // Send to parser.
        commandParser.parse(doc);
    }
}