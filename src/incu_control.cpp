#include "incu_control.h"

IncuControl::IncuControl(void) 
{
    sched = Sched();
}

bool IncuControl::begin(TwoWire &wire, uint8_t address)
{
    // Create the sensor object & start
    _wire = &wire;

    // Start the i2c. 
    _wire->begin();   

    // Create the sensor object.
    _sht31 = new SHT31(address);
        
    // Enable the sensor
    if (_sht31->begin()) {
        return false;
    }
    
    // Disable the temperature control
    _control_enable = false;

    // Initialise pins
    initPins();

    // Initialise the PID
    initPid();

    sched.begin(setpoint);

    return(true);
}

void IncuControl::initPins() {
    pinMode(PIN_HEAT, OUTPUT);
    pinMode(PIN_COOL, OUTPUT);
    digitalWrite(PIN_HEAT, LOW);
    digitalWrite(PIN_COOL, LOW);
}

void IncuControl::initPid() {
    // Specify the links and initial tuning parameters. 
    _pid = new PID(&temperature, &_pidOutput, 
        &setpoint, PID_KP, PID_KI, PID_KD, DIRECT);
    // Tell the PID to range between the window size
    _pid->SetOutputLimits(PID_WINDOW_MIN, PID_WINDOW_MAX);
    // Turn the PID on
    _pid->SetMode(AUTOMATIC);
}

// Runs the PID control. 
void IncuControl::run() {
    if(_control_enable == true) {
        pidOutput();
    }
}

// Controls the system based on the PID output.
void IncuControl::pidOutput() {
    
    unsigned long currentTime = millis(); 
    unsigned long elapsedTime = currentTime - _windowStartTime;
    
    if (elapsedTime > PID_WINDOW_MAX)
    { 
        _windowStartTime = currentTime; //Shift window
        readSensor(); // Read the sensor
        _pid->Compute(); // Compute PID output
    }

    if (_pidOutput > 0) {
        if (_pidOutput < elapsedTime) {
            peltierOff();
        }
        else {
            peltierHeat();
        }
    }

    if (_pidOutput < 0) {
        if (abs(_pidOutput) < elapsedTime) {
            peltierOff();
        }
        else {
            peltierCool();
        }
    }
}

// Heat the Peltier.
void IncuControl::peltierHeat() {
    digitalWrite(PIN_COOL, LOW);
    digitalWrite(PIN_HEAT, HIGH);
}

// Cool the Peltier.
void IncuControl::peltierCool() {
    digitalWrite(PIN_HEAT, LOW);
    digitalWrite(PIN_COOL, HIGH);
}

// Disable the Peltier.
void IncuControl::peltierOff() {
    digitalWrite(PIN_HEAT, LOW);
    digitalWrite(PIN_COOL, LOW);
}

// Disable control.
void IncuControl::disable() {
    peltierOff();
    _control_enable = false;
    Serial.println("Enabled system");
}

// Enable control.
void IncuControl::enable() {
    _control_enable = true;
    // Set the start time
    _windowStartTime = millis();
}

// Set the setpoint.
void IncuControl::setSetpoint(float setpointVal){
    //Check the temperature value is within range. 
    if (setpointVal > SETPOINT_MIN && setpointVal < SETPOINT_MAX) {
        setpoint = setpointVal; //Assign to system. 
        char buffer[64];  // buffer must be big enough to hold all the message
    }
}

// Read the sensor.
void IncuControl::readSensor() {
    _sht31->read();
    temperature = (double)_sht31->getTemperature();
    humidity = (double)_sht31->getHumidity();
}








