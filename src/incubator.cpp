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
    initPins();
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
    state.temperature = 25;
    state.pidKp = 240;
    state.pidKd = 60,
    state.pidKi = 12;

    _pid = new PID(&state.temperature, &_pidOutput, 
        &state.setpoint, state.pidKp, state.pidKi, state.pidKd, DIRECT);
    //tell the PID to range between the window size
    _pid->SetOutputLimits(INCU_PID_WINDOW_SIZE_MIN, INCU_PID_WINDOW_SIZE_MAX);
    //turn the PID on
    _pid->SetMode(AUTOMATIC);
}

/* @brief This API calls the APIs to read the sensor and generate the peltier output.
*/
void Incubator::run() {
    readSensor();

    if(state.enable) {
        pidOutput();
    }
}

void Incubator::pidOutput() {
    _pid->Compute();
    /************************************************
    * turn the output pin on/off based on pid output
    ************************************************/
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - _windowStartTime;
    
    if (elapsedTime > INCU_PID_WINDOW_SIZE_MAX)
    { //time to shift the Relay Window
        _windowStartTime = currentTime;
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
    Serial.println("Peltier Heat");
}

void Incubator::peltierCool() {
  digitalWrite(INCU_PIN_HEAT, LOW);
  digitalWrite(INCU_PIN_COOL, HIGH);
  Serial.println("Peltier Cool");
}

void Incubator::peltierOff() {
    digitalWrite(INCU_PIN_HEAT, LOW);
    digitalWrite(INCU_PIN_COOL, LOW);
    Serial.println("Peltier Off");
}

void Incubator::readSensor() {
    _sht31->read();
    state.temperature = (double)_sht31->getTemperature();
    state.humidity = (double)_sht31->getHumidity();
}

void Incubator::disable() {
    peltierOff();
    state.enable = false;
}

void Incubator::enable() {
    state.enable = true;
}

 void Incubator::readConfig(JsonDocument* jsonDoc) {
    Serial.print("Reading Config:");
    File configFile = SPIFFS.open("/config.txt", "r"); // Open file for reading. 

    if (!configFile) {
        Serial.println("- failed to open config file for writing");
        return;
    }

    deserializeJson(*jsonDoc, configFile); // Save contents of config file to json.
    configFile.close();
    Serial.println("Read OK");
 }

 void Incubator::saveConfig() {        
    Serial.println("Save Config: ");

    JsonDocument jsonDoc;
    readConfig(&jsonDoc);

    serializeJson(jsonDoc, Serial); // Print contents. 
        
    jsonDoc["test"] = "test"; // Create a json element. 
    
    File configFile = SPIFFS.open("/config.txt", "w"); // Open file to write. 
    serializeJson(jsonDoc, configFile); // Save json object. 
    serializeJson(jsonDoc, Serial); // Print contents. 
    configFile.close(); // Close file. 
    Serial.println(""); 
    Serial.println(" - config.json saved - OK.");
}

void Incubator::runCmds() {
    switch (rxData.cmd) {
        case INCU_ENABLE:
            Serial.println("Enable");
            enable();
            break;
        case INCU_DISABLE:
            Serial.println("Disable");
            disable();
            break;
        case INCU_SET_SETPOINT:
            state.setpoint = rxData.setpoint;
            Serial.print("Setpoint = ");
            Serial.println(state.setpoint);
        default:
            // statements
            break;
    }
    
    rxData.cmd = INCU_NULL_CMD;
}






