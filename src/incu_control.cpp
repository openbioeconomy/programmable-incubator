#include "incu_control.h"

IncuControl::IncuControl() {

}

bool IncuControl::begin(TwoWire &wire, uint8_t address)
{
    _wire = &wire;

    // Start the i2c. 
    _wire->begin();   

    // Create the sensor object.
    _sht31 = new SHT31(address);
    
    // Disable temperature control.
    _control_enable = false;

    initPins();
    initSensor();
    initPid();

    _windowStartTime = millis();
  
    return true;
}

void IncuControl::initPins() {
    pinMode(PIN_HEAT, OUTPUT);
    pinMode(PIN_COOL, OUTPUT);
    digitalWrite(PIN_HEAT, LOW);
    digitalWrite(PIN_COOL, LOW);
}

bool IncuControl::initSensor() 
{
    if (_sht31->begin()) {
      Serial.println("Sensor err!"); // Needs fixing
      return false;
    }

    return true;
}

void IncuControl::initPid() {
    // Specify the links and initial tuning parameters. 
    _pid = new PID(&temperature, &_pidOutput, 
        &setpoint, PID_KP, PID_KI, PID_KD, DIRECT);
    //Tell the PID to range between the window size
    _pid->SetOutputLimits(PID_WINDOW_MIN, PID_WINDOW_MAX);
    //Turn the PID on
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
        readSensor(); // Read sensor
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

// Read the sensor.
void IncuControl::readSensor() {
    _sht31->read();
    temperature = (double)_sht31->getTemperature();
    humidity = (double)_sht31->getHumidity();
    Serial.println(temperature);
}

void IncuControl::disable() {
    peltierOff();
    _control_enable = false;
    Serial.println("Enabled system");
}


void IncuControl::enable() {
    _control_enable = true;
    Serial.println("Enabled system");
}

void IncuControl::setSetpoint(float setpointVal){
    //Check the temperature value is within range. 
    if (setpointVal > SETPOINT_MIN && setpointVal < SETPOINT_MAX) {
        setpoint = setpointVal; //Assign to system. 
        char buffer[64];  // buffer must be big enough to hold all the message
        sprintf(buffer, "Setpoint = %d degC", setpoint);
        Serial.println(buffer);
    }
}








