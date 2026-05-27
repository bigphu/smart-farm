#include "wifi-manager.h"
#include "config.h"
#include <WiFi.h>
#include <WiFiManager.h>

void wifiProvision(const String& deviceID) {
  String apName = String(WIFI_AP_PREFIX) + deviceID.substring(12);
  apName.replace(":", "");
 
  WiFiManager wm;
  wm.setConfigPortalTimeout(WIFI_CONFIG_TIMEOUT);
 
  Serial.printf("[WIFI] Starting provisioning AP: %s\n", apName.c_str());
 
  if (!wm.autoConnect(apName.c_str())) {
    Serial.println("[WIFI] Config portal timed out! Rebooting...");
    delay(1000);
    ESP.restart();
  }
 
  Serial.printf("[WIFI] Connected! SSID: %s  IP: %s\n",
                WiFi.SSID().c_str(),
                WiFi.localIP().toString().c_str());
}