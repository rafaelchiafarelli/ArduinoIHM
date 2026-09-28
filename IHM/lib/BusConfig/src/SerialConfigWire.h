#pragma once
#include "SerialConfig.h"
#include "mavlink.h"

/**
 * SerialConfig <-> MAVLink adapters, kept out of SerialConfig.h so the
 * model itself needs no MAVLink headers. Pure (the generated headers are
 * plain C), host-tested.
 */

/**
 * CAN_SIGNAL_CONFIG (301) -> generator. False (out untouched) when the
 * message is out of range: bus_id > 1, dlc > 8, extended_id > 1, enable > 1,
 * or an ID above the ID kind's maximum.
 */
bool canGeneratorFromWire(const mavlink_can_signal_config_t& m, CanGeneratorConfig* out);

/** Generator -> CAN_SIGNAL_CONFIG (301) for `bus`. */
void canGeneratorToWire(uint8_t bus, const CanGeneratorConfig& g, mavlink_can_signal_config_t* out);

/** RS485_SIGNAL_CONFIG (302) -> generator. False (out untouched) when length > 32 or enable > 1. */
bool rs485GeneratorFromWire(const mavlink_rs485_signal_config_t& m, Rs485GeneratorConfig* out);

/** Generator -> RS485_SIGNAL_CONFIG (302). */
void rs485GeneratorToWire(const Rs485GeneratorConfig& g, mavlink_rs485_signal_config_t* out);
