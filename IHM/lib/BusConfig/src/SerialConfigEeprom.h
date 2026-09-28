#pragma once
#include "SerialConfig.h"

/**
 * AVR-only: moves SerialConfig's image (SerialConfigImage.h) in and out of
 * EEPROM at SERIAL_CONFIG_EEPROM_ADDR. Superloop-only (blocking EEPROM
 * access, no ISR use).
 */

/**
 * Loads the saved config into *out, used as saved (serial_config question
 * 4). A blank or corrupt EEPROM gives the defaults; returns false then.
 */
bool loadSerialConfig(SerialConfig* out);

/**
 * Saves `c`. eeprom_update_block writes only the bytes that changed; each
 * written byte blocks ~3.3 ms (a toggle typically changes the enable byte
 * and the CRC: ~10-15 ms).
 */
void saveSerialConfig(const SerialConfig& c);
