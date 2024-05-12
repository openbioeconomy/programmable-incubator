#ifndef INCUBATOR_h
#define INCUBATOR_h

#include "Arduino.h"
#include <Wire.h>
#include "SHT31.h"
#include <PID_v1.h>
#include <incu__defs.h>
#include <ArduinoJson.h>
#include "incu_spiffs.h"

class Incubator {
    public:
        Incubator();

        bool begin(const uint32_t i2cSpeed);
        void run();
        void disable();
        void enableConstTemp();
        void setTempSetpoint(float);
        void processCmds();

        IncuWrite write;
        IncuConfig config;

    private:
        unsigned long _windowStartTime;
        uint8_t _address;
        uint32_t _I2CSpeed;
        double _pidOutput;
        void initPid();
        void peltierHeat();
        void peltierCool();
        void peltierOff();
        void readSensor();
        bool initSensor();
        void initPins();
        void pidOutput();
        TwoWire* _wire;
        PID* _pid;
        SHT31 *_sht31 = NULL; ///< Pointer to I2C sensor 
};

#endif