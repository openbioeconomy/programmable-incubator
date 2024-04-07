#include "incubator.h"

Incubator::Incubator()
{
   
}
/// @brief 

bool Incubator::begin(const uint32_t i2cSpeed){

    Wire.begin();             // Start I2C as master
    Wire.setClock(i2cSpeed);  // and set bus speed
    _I2CSpeed = i2cSpeed;

    //initWiFi();
    initSensor();
    initPid();
   
    _windowStartTime = millis();

    return true;
};

bool Incubator::initSensor() {
    _sht31 = new SHT31(SHT31_ADDRESS);
    
    if (_sht31->begin()) {
      Serial.println("Sensor err!"); // Needs fixing
      return false;
    }

    return true;
}

void Incubator::initPid() {
    //Specify the links and initial tuning parameters
    pidSetpoint = 30;
    _aggKp=400, _aggKi=20, _aggKd=100;
    _consKp=1, _consKi=0.05, _consKd=0.25;
    _pid = new PID(&sensorTemperature, &_pidOutput, &pidSetpoint, _aggKp, _aggKi, _aggKd, DIRECT);
    //tell the PID to range between 0 and the full window size
    _pid->SetOutputLimits(PID_WINDOW_SIZE_MIN, PID_WINDOW_SIZE_MAX);
    //turn the PID on
    _pid->SetMode(AUTOMATIC);
}

// Function definitions:
void Incubator::peltierHeat() {
    digitalWrite(PIN_COOL, LOW);
    digitalWrite(PIN_HEAT, HIGH);
    Serial.println("Peltier Heat");
}

void Incubator::peltierCool() {
  digitalWrite(PIN_HEAT, LOW);
  digitalWrite(PIN_COOL, HIGH);
  Serial.println("Peltier Cool");
}

void Incubator::peltierOff() {
    digitalWrite(PIN_HEAT, LOW);
    digitalWrite(PIN_COOL, LOW);
    Serial.println("Peltier Off");
}

void Incubator::readSensor() {
    _sht31->read();
    sensorTemperature = (double)_sht31->getTemperature();
    sensorHumidity = (double)_sht31->getHumidity();
}

void Incubator::run() {

    readSensor();
    Serial.println(sensorTemperature);

    Serial.println(_pidOutput);
    _pid->Compute();
    
    /************************************************
    * turn the output pin on/off based on pid output
    ************************************************/
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - _windowStartTime;
    Serial.println(elapsedTime);
    if (elapsedTime > PID_WINDOW_SIZE_MAX)
    { //time to shift the Relay Window
        _windowStartTime = currentTime;//     +PID_WINDOW_SIZE_MAX;
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

