#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "config.h"
#include "device.h"
#include "sensors.h"
#include "relay.h"
#include "wifi-manager.h"
#include "mqtt-client.h"

static String deviceID;
static unsigned long lastTelemetryMs = 0;
static unsigned long lastMqttMs = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);
  deviceID = deviceGetMac();
  
  relayInit();
  sensorsInit();
  // wifiProvision(deviceID);
  // mqttInit(deviceID);
}

// Listen for incoming commands from the PC Python script
void handlePCCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd == "PUMP_TOGGLE") {
      if (relayIsOn()) {
        relayOff();
      } else {
        relayOn();
      }
    }
  }
}

void loop() {
  // mqttLoop();
  handlePCCommands();

  unsigned long now = millis();

  // 1. Send JSON Telemetry to PC every 1 second
  if (now - lastTelemetryMs >= SERIAL_TELEMETRY_MS) {
    SensorData d = sensorsRead();
    lastTelemetryMs = now;

    JsonDocument doc;
  
    doc["valid"] = d.isValid;
    if (d.isValid) {
      doc["temp"]  = serialized(String(d.temperature, 1));
      doc["humid"] = serialized(String(d.humidity, 1));
      doc["soil"]  = d.soilMoisture;
      doc["light"] = d.lightLevel;
      doc["rawSoil"] = d.rawSoil;
      doc["rawLight"] = d.rawLight;
    }
    doc["pump"]   = relayIsOn();
    // doc["mqtt"]   = mqttIsConnected();
    // doc["rssi"]   = WiFi.RSSI();
    doc["uptime"] = now / 1000;

    serializeJson(doc, Serial);
    Serial.println(); // Signal end of JSON object to PC

    // 2. Publish to Adafruit IO every 10 seconds
    // if (now - lastMqttMs >= MQTT_INTERVAL_MS) {
    //   lastMqttMs = now;
    //   if (d.isValid) mqttPublishSensors(d);
    // }
  }
}