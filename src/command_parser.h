#ifndef _COMMAND_PARSER_h
#define _COMMAND_PARSER_h

#include "ArduinoJson.h"
#include "incu_control.h"

class CommandParser {
    public:
        CommandParser();
        JsonDocument jsonRx;
        JsonDocument jsonTx;
        void begin(IncuControl &incuControl);
        void parse();
        
    private:
        IncuControl *_incuControl = NULL;
        HardwareSerial *_serial = NULL;
};

#endif