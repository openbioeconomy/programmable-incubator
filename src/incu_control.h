#ifndef _INCU_CONTROL_h
#define _INCU_CONTROL_h

#include "Arduino.h"
#include <Wire.h>
#include "SHT31.h"
#include <PID_v1.h>

// Sensor address. 
#define INCU_SHT31_ADDRESS 0x44

// Peltier control pins.
#define PIN_HEAT 17 
#define PIN_COOL 16

// PID window limits.
#define PID_WINDOW_MAX 5000
#define PID_WINDOW_MIN -5000

// Temperature limits.
#define SETPOINT_MIN 10
#define SETPOINT_MAX 90

// Default PID constants.
#define PID_KP 2
#define PID_KI 5
#define PID_KD 1

class IncuControl 
{
    public:   
        IncuControl();

        // Variables ------------

        // Sensor readings.
        double temperature;
        double humidity;

        // Temperature setpoint.
        double setpoint;

        // Constructor. 
        
                
        // Functions ------------
        bool begin(TwoWire &wire, uint8_t address);
        
        // Enable temperature control.
        void enable();

        // Disable temperature control.
        void disable();

        // Disable temperature control.
        void setSetpoint(float);

        // Run the control algorithm. 
        void run();

    private:
        // Variables ------------
        unsigned long _windowStartTime;
        double _pidOutput;
        bool _control_enable;

        // I-squared-C Class.
        TwoWire *_wire = NULL;

        // PID Class.
        PID *_pid = NULL;

        // Sensor Class.
        SHT31 *_sht31 = NULL; 

        // Functions ------------
        void initPid();
        void peltierHeat();
        void peltierCool();
        void peltierOff();
        void readSensor();
        bool initSensor();
        void initPins();
        void pidOutput();
};

#endif