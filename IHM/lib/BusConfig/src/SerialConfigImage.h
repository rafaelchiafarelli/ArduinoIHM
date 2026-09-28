#pragma once
#include <stdint.h>
#include "SerialConfig.h"

/**
 * SerialConfig's EEPROM image, pure (host-tested). The AVR load/save that
 * moves it in and out of EEPROM is SerialConfigEeprom.h.
 *
 *   offset 0  'S' 'C'           magic
 *          2  version           SERIAL_CONFIG_IMAGE_VERSION
 *          3  payload length    uint16, little-endian
 *          5  payload           fields one by one, little-endian, in the
 *                               order serialConfigSerialize() writes them
 *          5+n CRC-16/CCITT     over bytes 0 .. 4+n, little-endian
 *
 * Append-only extension rule: a new setting is appended to the payload
 * (the version stays). On load, fields past the stored payload length keep
 * their defaults and everything before is kept, so firmware that adds a
 * setting doesn't wipe the user's saved ones. A newer, longer image loads
 * its known prefix. Bad magic, CRC or version loads all defaults; the
 * version is bumped only for a non-append change. A section whose loaded
 * values fail its range check falls back to that section's defaults.
 */

#define SERIAL_CONFIG_IMAGE_VERSION 1
#define SERIAL_CONFIG_IMAGE_HEADER 5
#define SERIAL_CONFIG_IMAGE_CRC 2
// Current payload: 2 x CAN generator (19 B) + RS-485 generator (38 B).
#define SERIAL_CONFIG_PAYLOAD_LEN 76
#define SERIAL_CONFIG_IMAGE_LEN (SERIAL_CONFIG_IMAGE_HEADER + SERIAL_CONFIG_PAYLOAD_LEN + SERIAL_CONFIG_IMAGE_CRC)
// EEPROM bytes reserved for the image (ARCHITECTURE.md), extensions included.
#define SERIAL_CONFIG_EEPROM_ADDR 0
#define SERIAL_CONFIG_EEPROM_RESERVED 256

/** CRC-16/CCITT-FALSE (poly 0x1021, init 0xFFFF). */
uint16_t serialConfigCrc16(const uint8_t* buf, uint16_t len);

/** Writes the image of `c` to buf (SERIAL_CONFIG_IMAGE_LEN bytes); returns the length. */
uint16_t serialConfigSerialize(const SerialConfig& c, uint8_t* buf);

/**
 * Reads an image of `len` bytes (at most what the EEPROM area holds) into
 * *out. Always leaves a usable config in *out; returns false when it fell
 * back to all defaults (blank, corrupt or wrong-version image).
 */
bool serialConfigDeserialize(const uint8_t* buf, uint16_t len, SerialConfig* out);
