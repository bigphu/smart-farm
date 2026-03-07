#ifndef DEVICE_H
#define DEICE_H

// ============================================================
//            device.h  —  Read device MAC address
// ============================================================


#include <Arduino.h>

/**
 * Read MAC address of ESP32.
 * 
 * Return a string in the form of "AA:BB:CC:DD:EE:FF" (capitalized).
 * 
 * Used as unique DeviceID across the system.
 */
String deviceGetMac();

#endif