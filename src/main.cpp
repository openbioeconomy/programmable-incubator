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
    commandParser.begin(incuControl, Serial);
}

// ----------------------------------------------------------------------------
// Main control loop
// ----------------------------------------------------------------------------

void loop() 
{
    commandParser.run();   
}