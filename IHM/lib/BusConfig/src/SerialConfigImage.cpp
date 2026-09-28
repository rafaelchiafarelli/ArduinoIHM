#include "SerialConfigImage.h"
#include <string.h>

namespace {

struct Writer {
    uint8_t* buf;
    uint16_t pos;
    void u8(uint8_t v) { buf[pos++] = v; }
    void u16(uint16_t v) { u8((uint8_t)v); u8((uint8_t)(v >> 8)); }
    void u32(uint32_t v) { u16((uint16_t)v); u16((uint16_t)(v >> 16)); }
    void bytes(const uint8_t* p, uint16_t n) { memcpy(buf + pos, p, n); pos += n; }
};

// Reads fields while the stored payload has them; a field that isn't fully
// there leaves the destination (already holding its default) untouched.
struct Reader {
    const uint8_t* buf;
    uint16_t pos;
    uint16_t end;
    bool has(uint16_t n) const { return (uint32_t)pos + n <= end; }
    void u8(uint8_t* v) { if (has(1)) { *v = buf[pos]; } pos += 1; }
    void u16(uint16_t* v) { if (has(2)) { *v = (uint16_t)(buf[pos] | ((uint16_t)buf[pos + 1] << 8)); } pos += 2; }
    void u32(uint32_t* v) {
        if (has(4)) {
            *v = (uint32_t)buf[pos] | ((uint32_t)buf[pos + 1] << 8) |
                 ((uint32_t)buf[pos + 2] << 16) | ((uint32_t)buf[pos + 3] << 24);
        }
        pos += 4;
    }
    void bytes(uint8_t* p, uint16_t n) { if (has(n)) { memcpy(p, buf + pos, n); } pos += n; }
};

void writeCan(Writer& w, const CanGeneratorConfig& g) {
    w.u8(g.enable); w.u32(g.id); w.u8(g.extended); w.u8(g.dlc);
    w.bytes(g.data, SERIAL_CAN_DATA_MAX); w.u16(g.period_ms); w.u16(g.repeat_count);
}

void readCan(Reader& r, CanGeneratorConfig& g) {
    CanGeneratorConfig d = g;
    r.u8(&g.enable); r.u32(&g.id); r.u8(&g.extended); r.u8(&g.dlc);
    r.bytes(g.data, SERIAL_CAN_DATA_MAX); r.u16(&g.period_ms); r.u16(&g.repeat_count);
    if (!canGeneratorValid(g)) g = d;
}

void writeRs485(Writer& w, const Rs485GeneratorConfig& g) {
    w.u8(g.enable); w.u8(g.length); w.bytes(g.data, SERIAL_RS485_DATA_MAX);
    w.u16(g.period_ms); w.u16(g.repeat_count);
}

void readRs485(Reader& r, Rs485GeneratorConfig& g) {
    Rs485GeneratorConfig d = g;
    r.u8(&g.enable); r.u8(&g.length); r.bytes(g.data, SERIAL_RS485_DATA_MAX);
    r.u16(&g.period_ms); r.u16(&g.repeat_count);
    if (!rs485GeneratorValid(g)) g = d;
}

}  // namespace

uint16_t serialConfigCrc16(const uint8_t* buf, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)buf[i] << 8;
        for (uint8_t b = 0; b < 8; b++)
            crc = (crc & 0x8000) ? (uint16_t)((crc << 1) ^ 0x1021) : (uint16_t)(crc << 1);
    }
    return crc;
}

uint16_t serialConfigSerialize(const SerialConfig& c, uint8_t* buf)
{
    Writer w = { buf, 0 };
    w.u8('S'); w.u8('C'); w.u8(SERIAL_CONFIG_IMAGE_VERSION);
    w.u16(SERIAL_CONFIG_PAYLOAD_LEN);
    // Payload order is the on-EEPROM contract: only ever append.
    for (uint8_t b = 0; b < SERIAL_CAN_BUSES; b++)
        writeCan(w, c.can[b].gen);
    writeRs485(w, c.rs485.gen);
    w.u16(serialConfigCrc16(buf, w.pos));
    return w.pos;
}

bool serialConfigDeserialize(const uint8_t* buf, uint16_t len, SerialConfig* out)
{
    serialConfigDefaults(*out);
    if (len < SERIAL_CONFIG_IMAGE_HEADER + SERIAL_CONFIG_IMAGE_CRC)
        return false;
    if (buf[0] != 'S' || buf[1] != 'C' || buf[2] != SERIAL_CONFIG_IMAGE_VERSION)
        return false;
    uint16_t payload = (uint16_t)(buf[3] | ((uint16_t)buf[4] << 8));
    uint32_t total = (uint32_t)SERIAL_CONFIG_IMAGE_HEADER + payload + SERIAL_CONFIG_IMAGE_CRC;
    if (total > len)
        return false;
    uint16_t crcAt = (uint16_t)(SERIAL_CONFIG_IMAGE_HEADER + payload);
    uint16_t stored = (uint16_t)(buf[crcAt] | ((uint16_t)buf[crcAt + 1] << 8));
    if (serialConfigCrc16(buf, crcAt) != stored)
        return false;

    Reader r = { buf, SERIAL_CONFIG_IMAGE_HEADER, crcAt };
    for (uint8_t b = 0; b < SERIAL_CAN_BUSES; b++)
        readCan(r, out->can[b].gen);
    readRs485(r, out->rs485.gen);
    return true;
}
