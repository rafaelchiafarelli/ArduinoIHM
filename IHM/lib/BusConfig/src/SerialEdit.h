#pragma once
#include <stdint.h>
#include "SerialConfig.h"

/**
 * RE2 editing of the SERIAL tab's fields (serial_config / board_editing
 * task 2). Pure: main.cpp maps a focused widget's action to a (bus, field)
 * and calls serialEditStep() once per RE2 click. Host-tested.
 */

// Buses as the SERIAL tab orders them; same numbering as IHM_SERIAL_BUS.
#define SERIAL_BUS_CAN0 0
#define SERIAL_BUS_CAN1 1
#define SERIAL_BUS_RS485 2
#define SERIAL_BUS_COUNT 3

enum SerialField {
    SERIAL_FIELD_ENABLE,       // switch: CW = on, CCW = off (set, never flip)
    SERIAL_FIELD_ID,           // CAN: hex, digit-accelerated up to the ID kind's top digit
    SERIAL_FIELD_DLC,          // CAN: 0-8, one per click
    SERIAL_FIELD_EXTENDED,     // CAN: switch; leaving extended clamps the ID
    SERIAL_FIELD_LENGTH,       // RS-485: 0-32, x10 acceleration
    SERIAL_FIELD_PERIOD,       // 10 ms units, up to 65530 ms, x10 acceleration
    SERIAL_FIELD_REPEAT,       // 0 (until disabled) - 65535, x10 acceleration
    SERIAL_FIELD_BYTE_INDEX,   // which data byte BYTE_VALUE edits: 0 .. DLC/LEN-1
    SERIAL_FIELD_BYTE_VALUE    // hex 00-FF, x16 acceleration
};

// serialEditStep() result bits.
#define SERIAL_EDIT_CHANGED 0x01         // something in the config or the byte index changed
#define SERIAL_EDIT_ENABLE_TOGGLED 0x02  // the bus's enable changed (serial_config question 3: save)

/**
 * One RE2 click (`cw` = clockwise) on `field` of `bus`, at `nowMs`.
 * `byteIndex` is the per-bus "B<n>" selection (SERIAL_BUS_COUNT entries).
 * `accel` is the caller's acceleration state for the focused field --
 * digitAccelReset() it when the focus moves to another field.
 * Returns SERIAL_EDIT_* bits; 0 = nothing changed (at a limit, or a field
 * that doesn't exist on that bus).
 */
uint8_t serialEditStep(SerialConfig& c, uint8_t byteIndex[SERIAL_BUS_COUNT], uint8_t bus,
                       SerialField field, bool cw, DigitAccel& accel, uint32_t nowMs);

/** pbRE1 on a bus's enable switch: flips it. Returns SERIAL_EDIT_* bits. */
uint8_t serialToggleEnable(SerialConfig& c, uint8_t bus);

/** Number of data bytes the bus currently uses (DLC / LEN). */
uint8_t serialDataCount(const SerialConfig& c, uint8_t bus);
