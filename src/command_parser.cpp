#include "command_parser.h"

CommandParser::CommandParser() 
{

}

void CommandParser::begin(IncuControl &incuControl, HardwareSerial &serial)
{
    _incuControl = &incuControl;
    _serial = &serial;
}

void CommandParser::run() 
{
    if (_serial->available()) 
    {
        // Deserialise.
        DeserializationError err = deserializeJson(jsonRx, *_serial);
        if (!err)
        {
            if(jsonRx["set"]) 
            {
                parseSet();
            }
            if(jsonRx["get"]) 
            {
                parseGet();
            }
        } 
    }
}

void CommandParser::parseSet() 
{
    if(jsonRx["set"]["enable"] == true)
    {
        _incuControl->enable();
        _serial->println("Enable");
    }

    if(jsonRx["set"]["enable"] == false)
    {
        _incuControl->disable();
        _serial->println("Disable");
    }

    if (jsonRx["set"]["setpoint"]) {
        double sp = jsonRx["set"]["setpoint"];
        _incuControl->setSetpoint(sp);
        _serial->print("Setpoint: ");
        _serial->println(sp);
    }

    if (jsonRx["set"]["sched"]["list"]) {

        /* Get the size of the JSON Array */
        int arraySize = jsonRx["incu"]["set"]["sched"].size();   

        /* Clear the old schedule */
        _incuControl->sched.clear();

        /* Append to the schedule */
        for (uint8_t i = 0; i < arraySize; i++) { 
            uint8_t temperature = jsonRx["incu"]["set"]["sched"][i]["list"]["temp"];  
            uint32_t period = jsonRx["incu"]["set"]["sched"][i]["list"]["period"];
            _incuControl->sched.append(temperature, period);
        }
    }

    if (jsonRx["set"]["sched"]["play"] == true) {
        /* Play the schedule */
        _incuControl->sched.play();
    }

    if (jsonRx["set"]["sched"]["play"] == false) {
        /* Disable the schedule */
        _incuControl->sched.stop();
    }

}

void CommandParser::parseGet() 
{
    if (jsonRx["get"] == "state") {
        /* Read the sensor */
        jsonTx["state"]["temperature"] = _incuControl->temperature;
        jsonTx["state"]["setpoint"] = _incuControl->setpoint;
        jsonTx["state"]["temperature"] = _incuControl->temperature;
        serializeJson(jsonTx, Serial);
    }
}