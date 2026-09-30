#pragma once
#include <stdint.h>

/**
 * AVR-only EEPROM access for RelayStore.h's ring (EEPROM 256-351).
 * Superloop-only (blocking EEPROM writes, ~3.3 ms per changed byte).
 */

/** Reads the ring; returns the relay mask saved at the last change (0 if none). */
uint8_t relayStoreLoad();

/** Records `mask` in the next slot (3 bytes, ~10 ms). */
void relayStoreSave(uint8_t mask);
