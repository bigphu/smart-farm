#include "relay.h"
#include "config.h"
#include <Arduino.h>

static bool _relayOn = false;

void relayInit() {
  pinMode(PIN_RELAY, OUTPUT);
  relayOff();  
}

void relayOn() {
  digitalWrite(PIN_RELAY, LOW); 
  _relayOn = true;
}

void relayOff() {
  digitalWrite(PIN_RELAY, HIGH);
  _relayOn = false;
}

bool relayIsOn() {
  return _relayOn;
}