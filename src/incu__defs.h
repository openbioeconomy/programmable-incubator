#ifndef INCU__DEFS_h
#define INCU__DEFS_h

/* Sensor address */
#define INCU_SHT31_ADDRESS 0x44

/* Peltier heat pin */
#define INCU_PIN_HEAT 17 

/* Peltier cool pin */
#define INCU_PIN_COOL 16

/* Maximum size of the PID window */
#define INCU_PID_WINDOW_SIZE_MAX 5000

/* Minimum size of the PID window */
#define INCU_PID_WINDOW_SIZE_MIN -5000

/* Maximum temperature */
#define INCU_TEMP_MAX 90

/* Minimum temperature */
#define INCU_TEMP_MIN 10

// WiFi network credentials
//static const char *WIFI_SSID = "TALKTALK42E7AF"; 
//static const char *WIFI_PASSWORD = "44ER8RTH"; 
static const char *INCU_WIFI_SSID = "Makespace"; 
static const char *INCU_WIFI_PASSWORD = "getexc1tedandmaketh1ngs";  

#define INCU_HTTP_PORT 80

/*
 * @brief Incubator commands
 */
enum IncuCmd {
    /*! NULL command */
    INCU_NULL_CMD,
    /*! System enable */
    INCU_ENABLE_CONST_TEMP,
    /*! System disable */
    INCU_DISABLE,
    /*! Set temperature setpoint */
    INCU_SET_SETPOINT,
    /*! Set PID parameters */
    INCU_SAVE_CONFIG
    /*! Save config */
};

enum IncuErr {
    /*! All OK */
    INCU_OK,
    /*! Spiffs file error  */
    INCU_SPIFFS_ERR,
    /*! Sensor error */
    INCU_SENSOR_ERR
};

enum IncuMode
{
    /*! Off */
    INCU_OFF,
    /*! Constant temp */
    INCU_CONST_TEMP,
    /*! Scheduled */
    INCU_SCED
};

/*
 * @brief Incubator data config structure
 */
struct IncuWrite
{
    /*! System command */
    IncuCmd cmd;
    /*! Temperature setpoint */
    double setpoint;
    /*! PID Proportional */
    float pidKp;
    /*! PID Integral */
    float pidKi;
    /* PID Derivative */
    float pidKd;
};

/*
 * @brief Incubator state structure
 */
struct IncuConfig
{
    /*! System mode */
    IncuMode mode;
    /*! Temperature setpoint */
    double setpoint;
    /*! New data flag */
    bool newData;
    /*! Temperature reading */
    double temperature;
    /*! Humidity reading */
    double humidity;
    /*! PID Proportional */
    float pidKp;
    /*! PID Integral */
    float pidKi;
    /* PID Derivative */
    float pidKd;
};

#endif /* ICU_DEFS_H_ */