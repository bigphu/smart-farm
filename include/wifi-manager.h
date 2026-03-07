#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

// ============================================================
// wifi_manager.h  —  Smart WiFi Provisioning via WiFiManager
// ============================================================


/**
 * Connect ESP32 to WiFi.
 * Blocking until:
 * 
 *   - Successful WiFi connection.
 * 
 *   - Timeout after WIFI_CONFIG_TIMEOUT -> automatically reboot ESP32
 * 
 * Call once in setup()
 * 
 * @param deviceID MAC address used to name the access point (SmartFarm-XXXX)
 */
void wifiProvision(const String& deviceID);

#endif