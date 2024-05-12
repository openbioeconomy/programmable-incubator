#include "incubator.h"


Incubator::Incubator()
{
   
}
/// @brief 

bool Incubator::begin(const uint32_t i2cSpeed){

    Wire.begin();             // Start I2C as master
    Wire.setClock(i2cSpeed);  // and set bus speed
    _I2CSpeed = i2cSpeed;

    incuSpiffsInit();
    initPins();
    incuSpiffsReadConfig(&config); //Read the config from file
    initSensor();
    initPid();
   
    _windowStartTime = millis();

    return true;
};

void Incubator::initPins() {
    pinMode(INCU_PIN_HEAT, OUTPUT);
    pinMode(INCU_PIN_COOL, OUTPUT);
    digitalWrite(INCU_PIN_HEAT, LOW);
    digitalWrite(INCU_PIN_COOL, LOW);
}

bool Incubator::initSensor() {
    _sht31 = new SHT31(INCU_SHT31_ADDRESS);
    
    if (_sht31->begin()) {
      Serial.println("Sensor err!"); // Needs fixing
      return false;
    }

    return true;
}

void Incubator::initPid() {
    //Specify the links and initial tuning parameters
    _pid = new PID(&config.temperature, &_pidOutput, 
        &config.setpoint, config.pidKp, config.pidKi, config.pidKd, DIRECT);
    //Tell the PID to range between the window size
    _pid->SetOutputLimits(INCU_PID_WINDOW_SIZE_MIN, INCU_PID_WINDOW_SIZE_MAX);
    //Turn the PID on
    _pid->SetMode(AUTOMATIC);
}

/* @brief This API calls the APIs to read the sensor and generate the peltier output.
*/
void Incubator::run() {
    if(config.mode == INCU_CONST_TEMP) {
        pidOutput();
    }
}

void Incubator::pidOutput() {
    
    unsigned long currentTime = millis(); 
    unsigned long elapsedTime = currentTime - _windowStartTime;
    
    if (elapsedTime > INCU_PID_WINDOW_SIZE_MAX)
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

// Function definitions:
void Incubator::peltierHeat() {
    digitalWrite(INCU_PIN_COOL, LOW);
    digitalWrite(INCU_PIN_HEAT, HIGH);
}

void Incubator::peltierCool() {
    digitalWrite(INCU_PIN_HEAT, LOW);
    digitalWrite(INCU_PIN_COOL, HIGH);
}

void Incubator::peltierOff() {
    digitalWrite(INCU_PIN_HEAT, LOW);
    digitalWrite(INCU_PIN_COOL, LOW);
}

void Incubator::readSensor() {
    _sht31->read();
    config.temperature = (double)_sht31->getTemperature();
    config.humidity = (double)_sht31->getHumidity();
    config.newData = true;
    Serial.println(config.temperature);
}

void Incubator::disable() {
    peltierOff();
    config.mode  = INCU_OFF; //Set the operating mode
    Serial.println("Disabled system");
}

void Incubator::enableConstTemp() {
    config.mode  = INCU_CONST_TEMP; //Set the operating mode
    Serial.println("Enabled constant temp");
}

void Incubator::setTempSetpoint(float setpointVal){
    //Check the temperature value is within range. 
    if (setpointVal > INCU_TEMP_MIN && setpointVal < INCU_TEMP_MAX) {
        config.setpoint = setpointVal; //Assign to system. 
        Serial.print("Setpoint = ");
        Serial.println(config.setpoint);
    }
}

void Incubator::processCmds() {
    switch (write.cmd) {
        case INCU_ENABLE_CONST_TEMP:
            enableConstTemp(); // Enable the system
            incuSpiffsSaveConfig(&config); //Save state
            break;
        case INCU_DISABLE:
            disable(); //Disable the system
            incuSpiffsSaveConfig(&config); //Save state
            break;
        case INCU_SET_SETPOINT:
            setTempSetpoint(write.setpoint); //Set setpoint
            incuSpiffsSaveConfig(&config); //Save state
            break;
        case INCU_SAVE_CONFIG:
            incuSpiffsSaveConfig(&config); //Save config
        default:
            // statements
            break;
    }
    write.cmd = INCU_NULL_CMD;
}






