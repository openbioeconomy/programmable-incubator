#ifndef INCUBATOR_h
#define INCUBATOR_h

#include "Arduino.h"
#include <Wire.h>
#include "SHT31.h"
#include <ArduinoJson.h>
#include <PID_v1.h>

#define SHT31_ADDRESS   0x44
#define HTTP_PORT 80
#define PIN_HEAT 17 
#define PIN_COOL 16
#define PID_WINDOW_SIZE_MAX 5000
#define PID_WINDOW_SIZE_MIN -5000

class Incubator {
    public:
        Incubator();

        bool begin(const uint32_t i2cSpeed);
        void run();
        const char *wifiSSID = "Makespace";
        const char *wifiPass = "getexc1tedandmaketh1ngs";
        double pidSetpoint;
        
    private:
        unsigned long _windowStartTime;
        uint8_t _address;
        uint32_t _I2CSpeed;
        double _aggKp, _aggKi, _aggKd;
        double _consKp, _consKi, _consKd;
        double _pidOutput;
        void initPid();
        void peltierHeat();
        void peltierCool();
        void peltierOff();
        void readSensor();
        bool initSensor();
        void initPins();
        TwoWire* _wire;
        PID* _pid;
        SHT31 *_sht31 = NULL; ///< Pointer to I2C sensor 
        double sensorTemperature;
        double sensorHumidity;
        double temperatureSetpoint;
};

#endif