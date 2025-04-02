#include "command_parser.h"

CommandParser::CommandParser() 
{

}

void CommandParser::begin(Sched &sched, IncuControl &incuControl)
{
    _sched = &sched;
    _incuControl = &incuControl;
}

void CommandParser::parse(JsonDocument &json) 
{
    if(json["incu"]["enable"] == true)
    {
        _incuControl->enable();
    }

    if(json["incu"]["enable"] == false)
    {
        _incuControl->disable();
    }

    if (json["incu"]["setpoint"]) {
        _incuControl->setSetpoint(json["setpoint"]);
    }

    if (json["incu"]["sched"]["list"]) {

        /* Get the size of the JSON Array */
        int arraySize = json["sched"].size();   

        /* Clear the old schedule */
        _sched->clear(); 

        /* Append to the schedule */
        for (uint8_t i = 0; i < arraySize; i++) { 
            uint8_t temperature = json["sched"][i]["list"]["temp"];  
            uint32_t period = json["sched"][i]["list"]["period"];
            _sched->append(temperature, period);
        }
    }

    if (json["incu"]["sched"]["play"] == true) {
        /* Enable the schedule */
        _sched->play();
    }

    if (json["incu"]["sched"]["play"] == false) {
        /* Disable the schedule */
        _sched->stop();
    }
}