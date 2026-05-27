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
  Serial.println("MQTT Cmd on " + String(topic));
 
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, message, length);
  if (err) {
    Serial.println("ERR JSON parse: " + String(err.c_str()));
    return;
  }
 
  const char* targetID = doc["macAddress"];
  if (!targetID || _deviceID != String(targetID)) {
    Serial.println("MQTT Cmd ignored (wrong ID)");
    return;
  }
 
  const char* action = doc["Action"];
  if (!action) return;
 
  if (strcmp(action, "ON") == 0) {
    relayOn();
  } else if (strcmp(action, "OFF") == 0) {
    relayOff();
  } else {
    Serial.println("ERR Unknown action: " + String(action));
  }
}

/**
 * Establishes a connection to the MQTT server. 
 * 
 * Called during initial setup or when attempting to reconnect after a dropped connection.
 */
static void _connect() {
  _mqtt.setServer(AIO_SERVER, AIO_SERVERPORT);
  _mqtt.setCallback(_onMessage);
 
  String clientID = "ESP32-" + _deviceID;
  clientID.replace(":", "");
 
  while (!_mqtt.connected()) {
    Serial.println("MQTT Connecting...");
    if (_mqtt.connect(clientID.c_str(), AIO_USERNAME, AIO_KEY)) {
      _mqtt.subscribe(TOPIC_COMMANDS);
      Serial.println("MQTT Connected to Adafruit IO.");
      Serial.println("MQTT Subscribed: " TOPIC_COMMANDS);
    } else {
      char msg[32];
      snprintf(msg, sizeof(msg), "Failed rc=%d, retry 5s", _mqtt.state());
      Serial.println("ERR " + String(msg));
      Serial.println("MQTT Connection failed. Retrying...");
      delay(5000);
    }
  }
}

void mqttInit(const String& devID) { 
  _deviceID = devID;
  _connect();
}

void mqttLoop() {
  if (!_mqtt.connected()) {
    Serial.println("MQTT Lost connection. Reconnecting...");
    _connect();
  }
  _mqtt.loop();
}

bool mqttPublishSensors(const SensorData& data) {
 if (!data.isValid) {
    Serial.println("MQTT Skip publish - invalid data");
    return false;
  }
 
  JsonDocument doc;
  JsonDocument sensorData;
  doc["mac"]     = _deviceID;
  
  sensorData["soil"]  = data.rawSoil;
  sensorData["temp"]  = serialized(String(data.temperature, 1));
  sensorData["humid"] = data.humidity;
  sensorData["light"] = data.rawLight;

  doc["data"]    = sensorData;
 
  char buf[256];
  size_t n = serializeJson(doc, buf);
 
  if (_mqtt.publish(TOPIC_SENSORS, buf, n)) {
    Serial.println("MQTT Published OK");
    return true;
  } else {
    Serial.println("MQTT Publish FAILED");
    return false;
  }
}

bool mqttIsConnected() {
  return _mqtt.connected();
}