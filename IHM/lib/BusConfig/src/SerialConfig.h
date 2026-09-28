#pragma once
#include <stdint.h>

/**
 * SerialConfig: the settings behind the SERIAL tab, one section per bus
 * (initiative serial_config). Pure: no AVR, UI or MAVLink headers, so it's
 * host-testable like PWMWireConfig. The MAVLink adapters live in
 * SerialConfigWire.h, the EEPROM image in SerialConfigImage.h.
 *
 * This initiative ships the signal-generator settings only (the fields
 * CAN_SIGNAL_CONFIG / RS485_SIGNAL_CONFIG already carry). Bus parameters
 * and protocol settings are added by the initiative that uses them, through
 * the extension recipe in lib/BusConfig/README.md.
 */

#define SERIAL_CAN_BUSES 2
#define SERIAL_CAN_DATA_MAX 8
#define SERIAL_RS485_DATA_MAX 32
#define SERIAL_CAN_STD_ID_MAX 0x7FFUL
#define SERIAL_CAN_EXT_ID_MAX 0x1FFFFFFFUL
#define SERIAL_U16_MAX 65535UL

// RE2 editing units and ceilings (serial_config question 5).
#define SERIAL_PERIOD_UNIT_MS 10
#define SERIAL_PERIOD_EDIT_MAX_MS 65530UL

struct CanGeneratorConfig {
    uint8_t enable;          // 0/1
    uint32_t id;             // <= 0x7FF standard, <= 0x1FFFFFFF extended
    uint8_t extended;        // 0/1
    uint8_t dlc;             // 0-8
    uint8_t data[SERIAL_CAN_DATA_MAX];
    uint16_t period_ms;      // 0 = send once
    uint16_t repeat_count;   // 0 = until disabled
};

struct Rs485GeneratorConfig {
    uint8_t enable;          // 0/1
    uint8_t length;          // 0-32
    uint8_t data[SERIAL_RS485_DATA_MAX];
    uint16_t period_ms;
    uint16_t repeat_count;
};

// Extension fields go after the generator in their bus's section.
struct CanBusConfig {
    CanGeneratorConfig gen;
};

struct Rs485BusConfig {
    Rs485GeneratorConfig gen;
};

struct SerialConfig {
    CanBusConfig can[SERIAL_CAN_BUSES];
    Rs485BusConfig rs485;
};

/**
 * Defaults (also what a blank or corrupt EEPROM loads as): every generator
 * disabled, CAN ID 0x000 standard, DLC 8, RS-485 length 8, data all 0,
 * period 100 ms, repeat 0 (until disabled).
 */
void serialConfigDefaults(SerialConfig& c);

/** Range checks matching the 301/302 message ranges. Invalid = rejected, not clamped. */
bool canGeneratorValid(const CanGeneratorConfig& g);
bool rs485GeneratorValid(const Rs485GeneratorConfig& g);

/** Highest CAN ID for the ID kind. */
uint32_t canIdMax(uint8_t extended);

/**
 * Sets EXT to `on`. Switching to standard clamps the ID to 0x7FF. Returns
 * true if anything changed.
 */
bool canSetExtended(CanGeneratorConfig& g, bool on);

/**
 * RE2 numeric step: v + delta, clamped to [0, max]. A value already above
 * `max` (a PC may send one, e.g. period 65535 > 65530) is left alone by an
 * upward step instead of being pulled down to `max`.
 */
uint32_t serialStepValue(uint32_t v, int32_t delta, uint32_t max);

/** RE2 enum step: `steps` places through [0, count), clamped (wrap = false) or wrapping. */
uint8_t serialStepEnum(uint8_t v, int8_t steps, uint8_t count, bool wrap);

/** RE2 on a switch: sets (never flips) `flag` to `on`. Returns true if it changed. */
bool serialSetFlag(uint8_t& flag, bool on);

/**
 * Digit-at-a-time RE2 acceleration (serial_config question 2). Call
 * digitAccelStep() once per RE2 click with the click's time; it returns the
 * multiplier for that click: base^level. Fast clicks (< DIGIT_ACCEL_FAST_MS
 * apart) DIGIT_ACCEL_FAST_RUN times in a row raise the level one digit; a
 * pause of DIGIT_ACCEL_DROP_MS lowers it one digit; DIGIT_ACCEL_RESET_MS
 * (or digitAccelReset(), on a focus change) goes back to one unit. The level
 * never exceeds `maxLevel` (the field's top digit).
 */
#define DIGIT_ACCEL_FAST_MS 80UL
#define DIGIT_ACCEL_FAST_RUN 4
#define DIGIT_ACCEL_DROP_MS 300UL
#define DIGIT_ACCEL_RESET_MS 1500UL

struct DigitAccel {
    uint32_t lastMs;
    uint8_t level;
    uint8_t fastRun;
    uint8_t active;   // 0 until the first click after a reset
};

void digitAccelReset(DigitAccel& a);
uint32_t digitAccelStep(DigitAccel& a, uint32_t nowMs, uint8_t base, uint8_t maxLevel);
