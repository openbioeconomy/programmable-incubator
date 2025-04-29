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
#include "sensor.h"

CommandParser commandParser;
IncuControl incuControl;

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(100000);
    incuControl.begin(Wire, 0x44);
    commandParser.begin(incuControl);
}

// ----------------------------------------------------------------------------
// Main control loop
// ----------------------------------------------------------------------------

void loop() 
{

    if (Serial.available()) 
    {
        // Deserialise.
        DeserializationError err = deserializeJson(commandParser.jsonRx, Serial);
        // Parse.
        commandParser.parse();
    }

    if (!commandParser.jsonTx.isNull()) 
    {
        serializeJson(commandParser.jsonTx, Serial);
    }
}