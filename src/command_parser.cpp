#include "command_parser.h"

CommandParser::CommandParser() 
{

}

void CommandParser::begin(IncuControl &incuControl)
{
    _incuControl = &incuControl;
}

void CommandParser::parse() 
{
    if(jsonRx["incu"]["set"]["enable"] == true)
    {
        _incuControl->enable();
    }

    if(jsonRx["incu"]["set"]["enable"] == false)
    {
        _incuControl->disable();
    }

    if (jsonRx["incu"]["set"]["setpoint"]) { 
        _incuControl->sched.stop(); // Stop any schedule
        _incuControl->setSetpoint(jsonRx["incu"]["set"]["setpoint"]);
    }

    if (jsonRx["incu"]["set"]["sched"]["list"]) {

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

    if (jsonRx["incu"]["set"]["sched"]["play"] == true) {
        /* Play the schedule */
        _incuControl->sched.play();
    }

    if (jsonRx["incu"]["set"]["sched"]["play"] == false) {
        /* Disable the schedule */
        _incuControl->sched.stop();
    }

    if (jsonRx["incu"]["read"] == "state") {
        /* Read the sensor */
        jsonTx["incu"]["state"]["temperature"] = _incuControl->temperature;
        jsonTx["incu"]["state"]["sched"]["enable"] = _incuControl->temperature;
    }
}