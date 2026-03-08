#ifndef CONFIG_H
#define CONFIG_H


// ============================================================
//            PROJECT'S CONFIGURATION CONSTANTS
// ============================================================

#define DEBUG_MODE  1  // 0 - No debug JSON output
                       // 1 - Enable debug JSON output


// --------------------- PIN DEFINITIONS ---------------------
#define PIN_DHT            25     // GPIO4 - Temperatur and humidity sensor
#define DHT_TYPE           DHT22 
#define PIN_SOIL_MOISTURE  34     // GPIO34 - Soil moisture sensor (ADC, input-only)
#define PIN_LDR            32     // GPIO35 - Photoresistor (ADC, input-only)
#define PIN_RELAY          21     // GPIO26 - Relay IN (LOW = activate pump)


// --------------------- ADC ---------------------
#define SOIL_DRY_VALUE  4095  // Raw ADC when soil is dry
#define SOIL_WET_VALUE  0     // Raw ADC when soil is flooded

#define LDR_DIM_VALUE     4095  // Raw ADC when no light is detected
#define LDR_BRIGHT_VALUE  0     // Raw ADC when light source is next to sensor


// --------------------- DEVICE IDENTITY ---------------------
#define PIN_CODE  "123456"  // Later use for frontend and backend

// --------------------- WIFI PROVISIONING ---------------------
#define WIFI_AP_PREFIX       "SmartFarm-"  
#define WIFI_CONFIG_TIMEOUT  120           

// --------------------- ADAFRUIT IO (MQTT) ---------------------
// #define AIO_SERVER      "io.adafruit.com"
// #define AIO_SERVERPORT  1883               
// #define AIO_USERNAME    SECRET_AIO_USER 
// #define AIO_KEY         SECRET_AIO_KEY  // TODO: Put this in an .env file for security 

// #define TOPIC_SENSORS   AIO_USERNAME "/feeds/farm.sensors"
// #define TOPIC_COMMANDS  AIO_USERNAME "/feeds/farm.commands"

// --------------------- TIMING ---------------------
#define MQTT_INTERVAL_MS     10000UL  // 10 seconds to prevent Adafruit IO ban
#define SERIAL_TELEMETRY_MS  1000UL  // 1 second delay before packaging data into JSON
#define SENSOR_INTERVAL_MS   1000UL  // 1 second delay before reading sensors


// --------------------- SERIAL ---------------------
#define SERIAL_BAUD  115200

#endif