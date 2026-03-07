#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

// ============================================================
//             sensors.h  —  API controlling sensors
// ============================================================

// --------------------- CORE DATA STRUCTS ---------------------

struct SensorData {
  float temperature;   // Degree Celcius (°C)
  float humidity;      // Percentage (%)
  int   soilMoisture;  // Percentage (%) 
  int   lightLevel;    // Lux (lx)
  int   rawSoil;       // Ohm (Ω)
  int   rawLight;      // Ohm (Ω)
  bool  isValid;      
};

// --------------------- PUBLIC API ---------------------

/**
 * Initialize GPIO sensors.
 * 
 * Call once in setup().
 */
void sensorsInit();

/**
 * Read values from sensors and bundle them into a SensorData.
 * 
 * Call in loop() every SENSOR_INTERVAL_MS.
 */
SensorData sensorsRead();

#endif