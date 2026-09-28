#include "SerialConfigWire.h"
#include <string.h>

bool canGeneratorFromWire(const mavlink_can_signal_config_t& m, CanGeneratorConfig* out)
{
    if (m.bus_id >= SERIAL_CAN_BUSES)
        return false;
    CanGeneratorConfig g;
    g.enable = m.enable;
    g.id = m.can_id;
    g.extended = m.extended_id;
    g.dlc = m.dlc;
    memcpy(g.data, m.data, SERIAL_CAN_DATA_MAX);
    g.period_ms = m.period_ms;
    g.repeat_count = m.repeat_count;
    if (!canGeneratorValid(g))
        return false;
    *out = g;
    return true;
}

void canGeneratorToWire(uint8_t bus, const CanGeneratorConfig& g, mavlink_can_signal_config_t* out)
{
    out->bus_id = bus;
    out->can_id = g.id;
    out->extended_id = g.extended;
    out->dlc = g.dlc;
    memcpy(out->data, g.data, SERIAL_CAN_DATA_MAX);
    out->period_ms = g.period_ms;
    out->repeat_count = g.repeat_count;
    out->enable = g.enable;
}

bool rs485GeneratorFromWire(const mavlink_rs485_signal_config_t& m, Rs485GeneratorConfig* out)
{
    Rs485GeneratorConfig g;
    g.enable = m.enable;
    g.length = m.length;
    memcpy(g.data, m.data, SERIAL_RS485_DATA_MAX);
    g.period_ms = m.period_ms;
    g.repeat_count = m.repeat_count;
    if (!rs485GeneratorValid(g))
        return false;
    *out = g;
    return true;
}

void rs485GeneratorToWire(const Rs485GeneratorConfig& g, mavlink_rs485_signal_config_t* out)
{
    out->length = g.length;
    memcpy(out->data, g.data, SERIAL_RS485_DATA_MAX);
    out->period_ms = g.period_ms;
    out->repeat_count = g.repeat_count;
    out->enable = g.enable;
}
