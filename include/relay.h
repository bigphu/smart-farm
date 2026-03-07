#ifndef RELAY_H
#define RELAY_H

// ============================================================
//    relay.h  —  API controlling Relay module / water pump
// ============================================================

// --------------------- PUBLIC API ---------------------

/**
 * Initialize GPIO relay, the default state is OFF.
 * Call once in setup().
 */
void relayInit();

/**
 * Turn ON water pump (kéo GPIO xuống LOW để kích hoạt relay).
 */
void relayOn();

/**
 * Turn OFF water pump (kéo GPIO lên HIGH để nhả relay).
 */
void relayOff();

/**
 * Return TRUE if water pump is ON.
 */
bool relayIsOn();

#endif