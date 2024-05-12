#include "incu_spiffs.h"

void incuSpiffsInit() {
  if (!SPIFFS.begin()) {
    Serial.println("Cannot mount SPIFFS volume...");
  }
}

uint8_t incuSpiffsReadConfig(IncuConfig* config) { 
    Serial.print("Read Config:");
    File configFile = SPIFFS.open("/config.json", "r"); // Open file for reading. 

    if (!configFile) {
        Serial.println("- failed to open config file for writing");
        return(INCU_SPIFFS_ERR);
    }

    JsonDocument jsonDoc; // Create a json doc. 
    deserializeJson(jsonDoc, configFile); // Deserialise contents of config file to json doc.
    configFile.close();
    // Store parameter in json doc. 
    if(jsonDoc["setpoint"]){
      config->setpoint = (double)jsonDoc["setpoint"];
    }
    else {
      config->setpoint = 25;
    }

    if(jsonDoc["enable"]){
      config->mode = INCU_CONST_TEMP;
    }
    else {
      config->mode = INCU_OFF;
    }
    //Read PID parameters
    if(jsonDoc["pid-kp"]){
      config->pidKp = (double)jsonDoc["pid-kp"];
    }
    else {
      config->pidKp = 240;
    }

    if(jsonDoc["pid-ki"]){
      config->pidKp = (double)jsonDoc["pid-ki"];
    }
    else {
      config->pidKp = 60;
    }

    if(jsonDoc["pid-kd"]){
      config->pidKp = (double)jsonDoc["pid-kd"];
    }
    else {
      config->pidKp = 12;
    }
    
    Serial.println("Read OK");
    return(INCU_OK);
}

uint8_t incuSpiffsSaveConfig(IncuConfig *config) {        
    Serial.print("Save system config: ");

    // Create a json doc. 
    JsonDocument jsonDoc;

    // Read file contents
    File configFile = SPIFFS.open("/config.json", "r"); // Open file for reading. 

    if (!configFile) {
        Serial.println("- failed to open config file for writing");
        return(INCU_SPIFFS_ERR);
    }

    deserializeJson(jsonDoc, configFile); // Deserialise contents of config file to json doc.
    configFile.close();
        
    // Store parameter in json doc. 
    jsonDoc["setpoint"] = config->setpoint; 
    jsonDoc["enable"] = config->mode;
    jsonDoc["pid-kp"] = config->pidKp; 
    jsonDoc["pid-ki"] = config->pidKi; 
    jsonDoc["pid-kd"] = config->pidKd; 
    
    configFile = SPIFFS.open("/config.json", "w"); // Open file to write. 
    serializeJson(jsonDoc, configFile); // Save json object. 
    
    configFile.close(); // Close file. 
    Serial.println(""); 
    Serial.println(" - config.json saved - OK.");

    return (INCU_OK);
}
