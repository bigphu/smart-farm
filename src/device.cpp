#include "device.h"

#include <esp_mac.h>

String deviceGetMac() {
  uint8_t octets[6];
  esp_read_mac(octets, ESP_MAC_WIFI_STA);

  // MAC address format (18 chars, \0 included): 
  //  "AA:BB:CC:DD:EE:FF"
  char macAddr[18];
  snprintf(macAddr, sizeof(macAddr),
          "%02X:%02X:%02X:%02X:%02X:%02X",
          octets[0], octets[1], octets[2], octets[3], octets[4], octets[5]);

  return String(macAddr);
}