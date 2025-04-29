#ifndef _INCU_CONTROL_h
#define _INCU_CONTROL_h

#include "Arduino.h"
#include <PID_v1.h>
#include "Wire.h"
#include "SHT31.h" 

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
#define PID_KP 1000
#define PID_KI 5
#define PID_KD 1

class IncuControl 
{
    public:   
        // Constructor. 
        IncuControl(void);

        // Variables ------------

        // Temperature setpoint.
        double setpoint;

        // Temperature reading.
        double temperature;
        double humidity;

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

        // PID Class.
        PID *_pid = NULL;

        TwoWire *_wire = NULL;

        SHT31 *_sht31 = NULL;

        // Functions ------------
        void initPid();
        void peltierHeat();
        void peltierCool();
        void peltierOff();
        void initPins();
        void pidOutput();
        void readSensor();
};

#endif