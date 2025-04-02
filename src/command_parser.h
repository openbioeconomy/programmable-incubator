#ifndef _COMMAND_PARSER_h
#define _COMMAND_PARSER_h

#include "ArduinoJson.h"
#include "incu_control.h"
#include "sched.h"

class CommandParser {
    public:
        CommandParser();
        void begin(Sched &sched, IncuControl &incuControl);
        void parse(JsonDocument& json);    
    private:
        Sched *_sched = NULL;
        IncuControl *_incuControl = NULL;
};

#endif