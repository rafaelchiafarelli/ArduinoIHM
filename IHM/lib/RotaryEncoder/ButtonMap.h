#pragma once
#include <stdint.h>
/**
 * Pure decode of the packed input bitmap (see BinaryInputs::fast_handler,
 * BinaryInput.h) into the compact button map GUI::update() expects. No
 * AVR dependency, so this is directly unit-testable on the host.
 *
 * Bit positions are BinaryInputs pin-table indices (BinaryInput.h):
 *   bits 0-3  -> push buttons 0-3, copied through unchanged
 *   bit 4     -> rot2's push button (index 14, PC7)
 *   bit 5     -> rot1's push button (index 11, PL0)
 *   bit 6     -> rot0's push button (index 8,  PL4)
 *   bit 7     -> always set to 1 (hardcoded sentinel, unrelated to any
 *                input pin)
 */
uint8_t buildButtonMap(uint16_t bMap);
