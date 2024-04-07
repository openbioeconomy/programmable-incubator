#include "incubator.h"

Incubator::Incubator()
{
   
}
/// @brief 

bool Incubator::begin(const uint32_t i2cSpeed){

    Wire.begin();             // Start I2C as master
    Wire.setClock(i2cSpeed);  // and set bus speed
    _I2CSpeed = i2cSpeed;

    initWiFi();
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

void Incubator::initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSSID, wifiPass);
    Serial.printf("Trying to connect [%s] ", WiFi.macAddress().c_str());
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.printf(" %s\n", WiFi.localIP().toString().c_str());
}

void Incubator::initWebServer() {
    _server = new AsyncWebServer(HTTP_PORT);
    _server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
        request->send(SPIFFS, "/index.html", "text/html");
    });
    _server->serveStatic("/", SPIFFS, "/");
    _server->begin();
}

void Incubator::initWebSocket() {
    _ws = new AsyncWebSocket("/ws");
    _ws->onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        onEvent(server, client, type, arg, data, len);
    });
    _server->addHandler(_ws); 
}

void Incubator::onEvent(AsyncWebSocket       *server,
             AsyncWebSocketClient *client,
             AwsEventType          type,
             void                 *arg,
             uint8_t              *data,
             size_t                len) {

    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

//void Incubator::onRootRequest(AsyncWebServerRequest *request) {
  //request->send(SPIFFS, "/index.html", "text/html");
//}

void Incubator::handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        Serial.println("test");
    }
}

void Incubator::notifyClients(String message) {
    _ws->textAll(message);
}

void Incubator::initPid() {
    //Specify the links and initial tuning parameters
    pidSetpoint = 25;
    _aggKp=4, _aggKi=0.2, _aggKd=1;
    _consKp=1, _consKi=0.05, _consKd=0.25;
    _pid = new PID(&sensorTemperature, &_pidOutput, &pidSetpoint, _aggKp, _aggKi, _aggKd, DIRECT);
    //tell the PID to range between 0 and the full window size
    _pid->SetOutputLimits(0, PID_WINDOW_SIZE_MAX);
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
    if (_pidOutput < elapsedTime) {
        peltierCool();
    }
    else {
        peltierHeat();
    }
}

