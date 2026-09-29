#pragma once
#include <stdint.h>

/**
 * Relay state kept across power cycles (fixes/000014, Rafael 2026-09-28:
 * every relay change is recorded, and boot restores what the relays were
 * at power-off). Pure ring-buffer logic, host-tested
 * (test_native/test_relay_store.cpp); the AVR EEPROM access is
 * RelayStoreEeprom.h.
 *
 * Wear levelling: RELAY_STORE_SLOTS slots, each [value, ~value, seq].
 * Every change goes into the next slot, so a cell is written once per
 * RELAY_STORE_SLOTS changes (32 x ~100k cycles ~ 3.2M changes).
 *   - seq counts 0..254 (255 = erased EEPROM); the newest slot is the valid
 *     one whose successor isn't valid or doesn't hold seq + 1.
 *   - A slot is valid when seq != 0xFF and value == ~check.
 *   - Written in address order value, ~value, seq: a write torn after the
 *     value bytes leaves the old seq (not seq + 1), a write torn between
 *     them fails the check, so the previous slot stays the newest.
 */

#define RELAY_STORE_SLOTS 32
#define RELAY_STORE_SLOT_BYTES 3
#define RELAY_STORE_BYTES (RELAY_STORE_SLOTS * RELAY_STORE_SLOT_BYTES)
// EEPROM placement (ARCHITECTURE.md, "EEPROM"): after SerialConfig's 0-255.
#define RELAY_STORE_EEPROM_ADDR 256

struct RelayStoreCursor {
    uint8_t slot;    // newest slot
    uint8_t seq;     // its seq
    uint8_t valid;   // 0 = nothing stored yet
};

/**
 * Scans a RELAY_STORE_BYTES image. Returns true and the newest stored relay
 * mask in *mask (bit i = relay i) if there is one; false (mask 0) on a blank
 * or fully invalid store. *cursor is where the next write goes from.
 */
bool relayStoreFind(const uint8_t* image, uint8_t* mask, RelayStoreCursor* cursor);

/**
 * The next write for `mask`: advances *cursor and fills the slot index and
 * its 3 bytes (write them in order, at RELAY_STORE_EEPROM_ADDR +
 * slot * RELAY_STORE_SLOT_BYTES).
 */
void relayStoreNext(RelayStoreCursor* cursor, uint8_t mask, uint8_t* slot, uint8_t bytes[RELAY_STORE_SLOT_BYTES]);
