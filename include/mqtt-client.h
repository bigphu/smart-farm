#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "sensors.h"  

// ============================================================
// mqtt_client.h  —  MQTT publish / subscribe / reconnect API
// ============================================================

/**
 * Configure MQTT server and callback, then make first connection.
 * deviceID is used as ClientID and to filter in-comming commands.
 * 
 * Call once in setup() after successfully connected to WiFi.
 *
 * @param devID MAC address of ESP32 (global deviceID)
 */
void mqttInit(const String& devID);

/**
 * MQTT connection keep-alive; Re-establish if connnection is lost.
 * 
 * Call every loop().
 */
void mqttLoop();

/**
 * Package SensorData into a JSON document and publish it to TOPIC_SENSORS.
 * 
 * Call after every MQTT_INTERVAL_MS
 * 
 * @param data SensorData returned by sensorsRead()
 */
bool mqttPublishSensors(const SensorData& data);

/**
 * Return TRUE if connected to MQTT server.
 */
bool mqttIsConnected();

#endif