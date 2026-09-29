#include "ModbusRtu.h"
#include <string.h>

namespace {

uint16_t be16(const uint8_t* p) { return (uint16_t)(((uint16_t)p[0] << 8) | p[1]); }

void putBe16(uint8_t* p, uint16_t v)
{
    p[0] = (uint8_t)(v >> 8);
    p[1] = (uint8_t)v;
}

// Appends the CRC (low byte first) to out[0..len) and returns the new length.
uint16_t seal(uint8_t* out, uint16_t len)
{
    uint16_t crc = crc16Modbus(out, len);
    out[len] = (uint8_t)crc;
    out[len + 1] = (uint8_t)(crc >> 8);
    return (uint16_t)(len + 2);
}

// address + quantity must stay within the 16-bit address space.
bool rangeFits(uint16_t address, uint16_t quantity)
{
    return (uint32_t)address + quantity <= 0x10000UL;
}

}  // namespace

uint16_t crc16Modbus(const uint8_t* buf, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= buf[i];
        for (uint8_t b = 0; b < 8; b++)
            crc = (crc & 1) ? (uint16_t)((crc >> 1) ^ 0xA001) : (uint16_t)(crc >> 1);
    }
    return crc;
}

void rtuReset(RtuAssembler& a)
{
    a.len = 0;
    a.overflow = 0;
    a.corrupt = 0;
}

void rtuPushByte(RtuAssembler& a, uint8_t b)
{
    if (a.len < MODBUS_RTU_MAX_ADU)
        a.buf[a.len++] = b;
    else
        a.overflow = 1;
}

void rtuMarkCorrupt(RtuAssembler& a)
{
    if (a.len > 0)
        a.corrupt = 1;
}

RtuFrameStatus rtuEndOfFrame(RtuAssembler& a, uint8_t ownAddress, RtuFrame* out)
{
    uint16_t len = a.len;
    uint8_t overflow = a.overflow, corrupt = a.corrupt;
    rtuReset(a);   // the buffer keeps its bytes until the next rtuPushByte()

    if (len == 0 && !overflow)
        return RTU_FRAME_EMPTY;
    if (overflow)
        return RTU_FRAME_OVERFLOW;
    if (corrupt)
        return RTU_FRAME_CORRUPT;
    if (len < 4)
        return RTU_FRAME_TOO_SHORT;
    uint16_t crc = (uint16_t)(a.buf[len - 2] | ((uint16_t)a.buf[len - 1] << 8));
    if (crc16Modbus(a.buf, (uint16_t)(len - 2)) != crc)
        return RTU_FRAME_BAD_CRC;
    uint8_t address = a.buf[0];
    if (address != ownAddress && address != MODBUS_BROADCAST_ADDRESS)
        return RTU_FRAME_NOT_FOR_US;

    out->address = address;
    out->function = a.buf[1];
    out->data = a.buf + 2;
    out->dataLen = (uint16_t)(len - 4);
    return RTU_FRAME_OK;
}

ModbusException modbusParseRequest(const RtuFrame& f, ModbusRequest* out)
{
    const uint8_t* d = f.data;
    ModbusRequest r;
    r.function = f.function;
    r.values = 0;
    r.value = 0;

    switch (f.function) {
        case MODBUS_FC_READ_COILS:
        case MODBUS_FC_READ_DISCRETE_INPUTS:
        case MODBUS_FC_READ_HOLDING_REGISTERS:
        case MODBUS_FC_READ_INPUT_REGISTERS: {
            if (f.dataLen != 4)
                return MODBUS_EX_ILLEGAL_DATA_VALUE;
            r.address = be16(d);
            r.quantity = be16(d + 2);
            bool bits = f.function <= MODBUS_FC_READ_DISCRETE_INPUTS;
            uint16_t max = bits ? MODBUS_MAX_READ_BITS : MODBUS_MAX_READ_REGISTERS;
            if (r.quantity < 1 || r.quantity > max)
                return MODBUS_EX_ILLEGAL_DATA_VALUE;
            if (!rangeFits(r.address, r.quantity))
                return MODBUS_EX_ILLEGAL_DATA_ADDRESS;
            break;
        }
        case MODBUS_FC_WRITE_SINGLE_COIL: {
            if (f.dataLen != 4)
                return MODBUS_EX_ILLEGAL_DATA_VALUE;
            r.address = be16(d);
            r.quantity = 1;
            uint16_t v = be16(d + 2);
            if (v != 0x0000 && v != 0xFF00)
                return MODBUS_EX_ILLEGAL_DATA_VALUE;
            r.value = v ? 1 : 0;
            break;
        }
        case MODBUS_FC_WRITE_SINGLE_REGISTER:
            if (f.dataLen != 4)
                return MODBUS_EX_ILLEGAL_DATA_VALUE;
            r.address = be16(d);
            r.quantity = 1;
            r.value = be16(d + 2);
            break;
        case MODBUS_FC_WRITE_MULTIPLE_COILS:
        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS: {
            if (f.dataLen < 5)
                return MODBUS_EX_ILLEGAL_DATA_VALUE;
            r.address = be16(d);
            r.quantity = be16(d + 2);
            uint8_t byteCount = d[4];
            bool bits = f.function == MODBUS_FC_WRITE_MULTIPLE_COILS;
            uint16_t max = bits ? MODBUS_MAX_WRITE_BITS : MODBUS_MAX_WRITE_REGISTERS;
            uint16_t expect = bits ? (uint16_t)((r.quantity + 7) / 8) : (uint16_t)(r.quantity * 2);
            if (r.quantity < 1 || r.quantity > max || byteCount != expect || f.dataLen != (uint16_t)(5 + byteCount))
                return MODBUS_EX_ILLEGAL_DATA_VALUE;
            if (!rangeFits(r.address, r.quantity))
                return MODBUS_EX_ILLEGAL_DATA_ADDRESS;
            r.values = d + 5;
            break;
        }
        default:
            return MODBUS_EX_ILLEGAL_FUNCTION;
    }
    *out = r;
    return MODBUS_EX_NONE;
}

bool modbusGetBit(const uint8_t* packed, uint16_t i)
{
    return (packed[i >> 3] >> (i & 7)) & 1;
}

uint16_t modbusGetRegister(const uint8_t* values, uint16_t i)
{
    return be16(values + 2 * i);
}

void modbusSetBit(uint8_t* packed, uint16_t i, bool on)
{
    uint8_t mask = (uint8_t)(1u << (i & 7));
    if (on)
        packed[i >> 3] |= mask;
    else
        packed[i >> 3] &= (uint8_t)~mask;
}

uint16_t modbusBuildReadBits(uint8_t* out, uint8_t address, uint8_t function,
                             const uint8_t* packedBits, uint16_t quantity)
{
    uint8_t byteCount = (uint8_t)((quantity + 7) / 8);
    out[0] = address;
    out[1] = function;
    out[2] = byteCount;
    memcpy(out + 3, packedBits, byteCount);
    // Bits past `quantity` in the last byte are sent as 0 (spec).
    if (quantity & 7)
        out[3 + byteCount - 1] &= (uint8_t)((1u << (quantity & 7)) - 1);
    return seal(out, (uint16_t)(3 + byteCount));
}

uint16_t modbusBuildReadRegisters(uint8_t* out, uint8_t address, uint8_t function,
                                  const uint16_t* registers, uint16_t quantity)
{
    out[0] = address;
    out[1] = function;
    out[2] = (uint8_t)(quantity * 2);
    for (uint16_t i = 0; i < quantity; i++)
        putBe16(out + 3 + 2 * i, registers[i]);
    return seal(out, (uint16_t)(3 + 2 * quantity));
}

uint16_t modbusBuildWriteSingle(uint8_t* out, uint8_t address, uint8_t function,
                                uint16_t registerAddress, uint16_t value)
{
    out[0] = address;
    out[1] = function;
    putBe16(out + 2, registerAddress);
    putBe16(out + 4, function == MODBUS_FC_WRITE_SINGLE_COIL ? (value ? 0xFF00 : 0x0000) : value);
    return seal(out, 6);
}

uint16_t modbusBuildWriteMultiple(uint8_t* out, uint8_t address, uint8_t function,
                                  uint16_t registerAddress, uint16_t quantity)
{
    out[0] = address;
    out[1] = function;
    putBe16(out + 2, registerAddress);
    putBe16(out + 4, quantity);
    return seal(out, 6);
}

uint16_t modbusBuildException(uint8_t* out, uint8_t address, uint8_t function, ModbusException code)
{
    out[0] = address;
    out[1] = (uint8_t)(function | 0x80);
    out[2] = (uint8_t)code;
    return seal(out, 3);
}
