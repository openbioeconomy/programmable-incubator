#ifndef _SCHED_h
#define _SCHED_h

#include "incu_control.h"
#include "esp_timer.h"
#include "Arduino.h"
#include "esp_mac.h"

// Maximum schedule steps
#define STEP_LIMIT 16

// Structure for storing the schedule
struct ActivationList {
    uint8_t temperature;
    uint8_t period;
};

class Sched 
{
    public:
        // Current step
        uint8_t step;

        // Schedule step counter 
        uint8_t step_counter;

        // Time remaining
        uint32_t countdownTimer;

        // Constructor
        Sched();

        // Functions ------------
        void begin(IncuControl &incuControl);
        void play();  
        void stop();
        void clear();
        void append(uint8_t setpoint, uint32_t period);
        void timerInterruptHandler();
        
    private:
        esp_timer_handle_t _timer_handler = NULL;
        ActivationList _activationList[STEP_LIMIT];
        double *_setpoint = NULL;
        IncuControl *_incuControl = NULL;
};

#endif