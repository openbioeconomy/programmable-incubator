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
    pinMode(PIN_HEAT, OUTPUT);
    pinMode(PIN_COOL, OUTPUT);
    digitalWrite(PIN_HEAT, LOW);
    digitalWrite(PIN_COOL, LOW);
}

bool Incubator::initSensor() {
    _sht31 = new SHT31(SHT31_ADDRESS);
    
    if (_sht31->begin()) {
      Serial.println("Sensor error!"); // Needs fixing
      return false;
    }

    return true;
}

void Incubator::initPid() {
    //Specify the links and initial tuning parameters
    pidSetpoint = 30;
    _aggKp=4000, _aggKi=0, _aggKd=0; // originally was _aggKp=240, _aggKi=12, _aggKd=60;
    _consKp=4000, _consKi=0, _consKd=0; // originally was _consKp=10, _consKi=0.5, _consKd=2.5;
    _pid = new PID(&sensorTemperature, &_pidOutput, &pidSetpoint, _aggKp, _aggKi, _aggKd, DIRECT);
    //tell the PID to range between 0 and the full window size
    _pid->SetOutputLimits(PID_WINDOW_SIZE_MIN, PID_WINDOW_SIZE_MAX);
    //turn the PID on
    _pid->SetMode(AUTOMATIC);
}

void Incubator::run() {

    readSensor();
    //Serial.println(sensorTemperature);
    
    
    int action;

    _pid->Compute();
    
    /************************************************
    * turn the output pin on/off based on pid output
    ************************************************/
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - _windowStartTime;
    //Serial.println(elapsedTime);
    if (elapsedTime > PID_WINDOW_SIZE_MAX)
    { //time to shift the Relay Window
        _windowStartTime = currentTime;//     +PID_WINDOW_SIZE_MAX;
    }

    if (_pidOutput > 0) {
        if (_pidOutput < elapsedTime) {
            peltierOff();
            action = 0;
        }
        else {
            peltierHeat();
            action = 1;
        }
    }

    if (_pidOutput < 0) {
        if (abs(_pidOutput) < elapsedTime) {
            peltierOff();
            action = 0;
        }
        else {
            peltierCool();
            action = -1;
        }
    }
    // Serial.println("Pid output is");
    // Serial.println(_pidOutput);
    // Serial.println("Elapsed time is");
    // Serial.println(std::to_string(elapsedTime).c_str());
    // Serial.println("------------------------");
    
    //Serial.print(sensorTemperature);
    Serial.print("Time: $");
    Serial.print(millis());
    Serial.print("$, Temperature: $");
    Serial.print(sensorTemperature);
    Serial.print("$, Action: $");
    if (action == -1)
    {Serial.print(-1);}
    else if (action == 0)
    {Serial.print(0);}
    else if (action == 1)
    {Serial.print(1);}
    Serial.print("$, PID output is: $");
    Serial.println(_pidOutput);
}

// Function definitions:

void Incubator::peltierHeat() {
    digitalWrite(PIN_COOL, LOW);
    digitalWrite(PIN_HEAT, HIGH);
    //Serial.print("Heat");
    //Serial.println(sensorTemperature);
}

void Incubator::peltierCool() {
  digitalWrite(PIN_HEAT, LOW);
  digitalWrite(PIN_COOL, HIGH);
  //Serial.print("Cool");
  //Serial.println(sensorTemperature);

}

void Incubator::peltierOff() {
    digitalWrite(PIN_HEAT, LOW);
    digitalWrite(PIN_COOL, LOW);
    //Serial.print("Offf");
    //Serial.println(sensorTemperature);
}

void Incubator::readSensor() {
    _sht31->read();
    sensorTemperature = (double)_sht31->getTemperature();
    sensorHumidity = (double)_sht31->getHumidity();
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
    Serial.print("Save Config: ");

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






