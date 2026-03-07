#include "mqtt-client.h"
#include "config.h"
#include "relay.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

/**
 * Read documentation at: 
 *  - https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
 *  - http://pubsubclient.knolleary.net/api
 */
static WiFiClient    _wifiClient;
static PubSubClient  _mqtt(_wifiClient);

static String        _deviceID;
static unsigned long _lastPublishMs = 0;  

/**
 * MQTT callback function. 
 * 
 * Called automatically every time a message is received from a subscribed topic.
 * 
 * @param topic The MQTT topic the message was published to.
 * @param message The payload/data sent from the server (not null-terminated).
 * @param length The length of the message payload in bytes.
 */
static void _onMessage(char* topic, byte* message, unsigned int length) {

}

/**
 * Establishes a connection to the MQTT server. 
 * 
 * Called during initial setup or when attempting to reconnect after a dropped connection.
 */
static void _connect() {

}

void mqttInit(const String& devID) { 

}

void mqttLoop() {

}

bool mqttPublishSensors(const SensorData& data) {
  return true;
}

bool mqttIsConnected() {
  return true;
}