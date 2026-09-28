#include "SerialEdit.h"

namespace {

// Steps a numeric value: `unit` x the acceleration multiplier, clamped to
// [0, max]. Returns true if it changed.
template <typename T>
bool stepNumeric(T& v, bool cw, uint32_t unit, uint8_t base, uint8_t maxLevel, uint32_t max,
                 DigitAccel& accel, uint32_t nowMs)
{
    uint32_t delta = unit * digitAccelStep(accel, nowMs, base, maxLevel);
    uint32_t next = serialStepValue((uint32_t)v, cw ? (int32_t)delta : -(int32_t)delta, max);
    if (next == (uint32_t)v)
        return false;
    v = (T)next;
    return true;
}

uint8_t* dataOf(SerialConfig& c, uint8_t bus)
{
    return bus < SERIAL_CAN_BUSES ? c.can[bus].gen.data : c.rs485.gen.data;
}

uint8_t& enableOf(SerialConfig& c, uint8_t bus)
{
    return bus < SERIAL_CAN_BUSES ? c.can[bus].gen.enable : c.rs485.gen.enable;
}

// Keeps a bus's byte index inside its data count after a DLC/LEN change.
void clampIndex(const SerialConfig& c, uint8_t byteIndex[SERIAL_BUS_COUNT], uint8_t bus)
{
    uint8_t count = serialDataCount(c, bus);
    if (byteIndex[bus] >= count)
        byteIndex[bus] = count ? (uint8_t)(count - 1) : 0;
}

}  // namespace

uint8_t serialDataCount(const SerialConfig& c, uint8_t bus)
{
    if (bus < SERIAL_CAN_BUSES)
        return c.can[bus].gen.dlc;
    return bus == SERIAL_BUS_RS485 ? c.rs485.gen.length : 0;
}

uint8_t serialToggleEnable(SerialConfig& c, uint8_t bus)
{
    if (bus >= SERIAL_BUS_COUNT)
        return 0;
    uint8_t& e = enableOf(c, bus);
    e = e ? 0 : 1;
    return SERIAL_EDIT_CHANGED | SERIAL_EDIT_ENABLE_TOGGLED;
}

uint8_t serialEditStep(SerialConfig& c, uint8_t byteIndex[SERIAL_BUS_COUNT], uint8_t bus,
                       SerialField field, bool cw, DigitAccel& accel, uint32_t nowMs)
{
    if (bus >= SERIAL_BUS_COUNT)
        return 0;
    bool can = bus < SERIAL_CAN_BUSES;
    bool changed = false;

    switch (field) {
        case SERIAL_FIELD_ENABLE:
            return serialSetFlag(enableOf(c, bus), cw) ? (SERIAL_EDIT_CHANGED | SERIAL_EDIT_ENABLE_TOGGLED) : 0;
        case SERIAL_FIELD_ID:
            if (!can) return 0;
            changed = stepNumeric(c.can[bus].gen.id, cw, 1, 16, c.can[bus].gen.extended ? 7 : 2,
                                  canIdMax(c.can[bus].gen.extended), accel, nowMs);
            break;
        case SERIAL_FIELD_DLC:
            if (!can) return 0;
            changed = stepNumeric(c.can[bus].gen.dlc, cw, 1, 10, 0, SERIAL_CAN_DATA_MAX, accel, nowMs);
            break;
        case SERIAL_FIELD_EXTENDED:
            if (!can) return 0;
            changed = canSetExtended(c.can[bus].gen, cw);
            break;
        case SERIAL_FIELD_LENGTH:
            if (can) return 0;
            changed = stepNumeric(c.rs485.gen.length, cw, 1, 10, 1, SERIAL_RS485_DATA_MAX, accel, nowMs);
            break;
        case SERIAL_FIELD_PERIOD: {
            uint16_t& p = can ? c.can[bus].gen.period_ms : c.rs485.gen.period_ms;
            changed = stepNumeric(p, cw, SERIAL_PERIOD_UNIT_MS, 10, 3, SERIAL_PERIOD_EDIT_MAX_MS, accel, nowMs);
            break;
        }
        case SERIAL_FIELD_REPEAT: {
            uint16_t& r = can ? c.can[bus].gen.repeat_count : c.rs485.gen.repeat_count;
            changed = stepNumeric(r, cw, 1, 10, 4, SERIAL_U16_MAX, accel, nowMs);
            break;
        }
        case SERIAL_FIELD_BYTE_INDEX: {
            uint8_t count = serialDataCount(c, bus);
            if (count == 0) return 0;
            changed = stepNumeric(byteIndex[bus], cw, 1, 10, 0, (uint32_t)(count - 1), accel, nowMs);
            break;
        }
        case SERIAL_FIELD_BYTE_VALUE: {
            if (serialDataCount(c, bus) == 0) return 0;
            clampIndex(c, byteIndex, bus);
            changed = stepNumeric(dataOf(c, bus)[byteIndex[bus]], cw, 1, 16, 1, 0xFF, accel, nowMs);
            break;
        }
    }
    clampIndex(c, byteIndex, bus);
    return changed ? SERIAL_EDIT_CHANGED : 0;
}
