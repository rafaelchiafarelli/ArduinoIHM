#pragma once
#include <stdint.h>
/**
 * IHM_RELAY_COMMAND (309) merge rule. Pure, no AVR and no relay driver, so
 * it's host-tested (test_native/test_relay_command.cpp) and MavlinkComms can
 * use it without depending on MultiOutput (serial_commands' layering rule).
 *
 * Bit i of every mask/state byte is relay i; a mask bit of 1 means "set this
 * relay to its state bit", 0 means "leave it alone".
 */

// Folds a newer command into a pending one the superloop hasn't taken yet
// (MavlinkComms::dispatch(), two frames in one pass): the newer state wins
// for the bits in its mask, pending bits outside it are kept.
inline void relayCommandAccumulate(uint8_t &pendMask, uint8_t &pendState, uint8_t mask, uint8_t state) {
    pendState = (uint8_t)((pendState & ~mask) | (state & mask));
    pendMask = (uint8_t)(pendMask | mask);
}

// The relay bitmask after applying a command to `current`.
inline uint8_t relayCommandApply(uint8_t current, uint8_t mask, uint8_t state) {
    return (uint8_t)((current & ~mask) | (state & mask));
}
