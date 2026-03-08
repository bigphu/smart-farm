#include "sensors.h"
#include "config.h"
#include <DHT.h>

static DHT dht(PIN_DHT, DHT_TYPE);

void sensorsInit() {
  dht.begin();
}

SensorData sensorsRead() {
  SensorData data;
  data.temperature = dht.readTemperature();
  data.humidity    = dht.readHumidity();
  
  if (isnan(data.temperature) || isnan(data.humidity)) {
    data.isValid = false;
  } else {
    data.isValid = true;
  }

  // TODO: Convert LDR reading from raw Ohm to Lux
  
  if (data.isValid) {
    data.rawSoil = analogRead(PIN_SOIL_MOISTURE);
    data.soilMoisture = constrain((int)map(data.rawSoil, SOIL_DRY_VALUE, SOIL_WET_VALUE, 0, 100), 0, 100);

    data.rawLight = analogRead(PIN_LDR);
    data.lightLevel = constrain((int)map(data.rawLight, LDR_DIM_VALUE, LDR_BRIGHT_VALUE, 0, 100), 0, 100);
  }
  
  return data;
}