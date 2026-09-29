#include "mini_test.h"
#include "ModbusRtu.h"
#include <string.h>
#include <vector>

// Modbus RTU framing (rs485_modbus / rtu_framing task 1). Frames are the
// Modbus spec's examples as pymodbus 3.15 (FramerRTU) builds them, CRC
// included, so the CRC and layout are checked against an independent
// implementation.

namespace {
typedef std::vector<uint8_t> Bytes;

// Feeds `bytes` then the end-of-frame silence.
RtuFrameStatus feed(RtuAssembler& a, const Bytes& bytes, uint8_t own, RtuFrame* f) {
    for (uint8_t b : bytes) rtuPushByte(a, b);
    return rtuEndOfFrame(a, own, f);
}

bool built(const uint8_t* out, uint16_t len, const Bytes& expect) {
    return len == expect.size() && memcmp(out, expect.data(), len) == 0;
}

const Bytes kReadHoldingReq = { 0x11, 0x03, 0x00, 0x6B, 0x00, 0x03, 0x76, 0x87 };
}  // namespace

TEST(ModbusRtu, Crc16KnownVectors) {
    const uint8_t a[] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x0A };
    CHECK_EQ((int)crc16Modbus(a, 6), 0xCDC5);   // sent C5 CD
    CHECK_EQ((int)crc16Modbus(kReadHoldingReq.data(), 6), 0x8776);   // spec: 76 87
}

TEST(ModbusRtu, AssemblesAValidFrameForUs) {
    RtuAssembler a; rtuReset(a);
    RtuFrame f;
    CHECK_EQ((int)feed(a, kReadHoldingReq, 0x11, &f), (int)RTU_FRAME_OK);
    CHECK_EQ((int)f.address, 0x11);
    CHECK_EQ((int)f.function, 0x03);
    CHECK_EQ((int)f.dataLen, 4);
    CHECK_EQ((int)f.data[1], 0x6B);
}

TEST(ModbusRtu, FiltersOtherAddressesButKeepsBroadcast) {
    RtuAssembler a; rtuReset(a);
    RtuFrame f;
    CHECK_EQ((int)feed(a, kReadHoldingReq, 0x12, &f), (int)RTU_FRAME_NOT_FOR_US);
    // Broadcast write single register 0x0001 = 3 (address 0), CRC recomputed.
    uint8_t out[MODBUS_RTU_MAX_ADU];
    uint16_t n = modbusBuildWriteSingle(out, 0, MODBUS_FC_WRITE_SINGLE_REGISTER, 1, 3);
    CHECK_EQ((int)feed(a, Bytes(out, out + n), 0x12, &f), (int)RTU_FRAME_OK);
    CHECK_EQ((int)f.address, 0);
}

TEST(ModbusRtu, RejectsBadCrcShortOverflowAndCorrupt) {
    RtuAssembler a; rtuReset(a);
    RtuFrame f;
    Bytes bad = kReadHoldingReq; bad[7] ^= 0x01;
    CHECK_EQ((int)feed(a, bad, 0x11, &f), (int)RTU_FRAME_BAD_CRC);
    CHECK_EQ((int)feed(a, Bytes{ 0x11, 0x03, 0x00 }, 0x11, &f), (int)RTU_FRAME_TOO_SHORT);
    CHECK_EQ((int)rtuEndOfFrame(a, 0x11, &f), (int)RTU_FRAME_EMPTY);
    for (int i = 0; i < MODBUS_RTU_MAX_ADU + 1; i++) rtuPushByte(a, 0x11);
    CHECK_EQ((int)rtuEndOfFrame(a, 0x11, &f), (int)RTU_FRAME_OVERFLOW);
    for (int i = 0; i < 3; i++) rtuPushByte(a, kReadHoldingReq[i]);
    rtuMarkCorrupt(a);   // a 1.5-char gap mid-frame
    for (int i = 3; i < 8; i++) rtuPushByte(a, kReadHoldingReq[i]);
    CHECK_EQ((int)rtuEndOfFrame(a, 0x11, &f), (int)RTU_FRAME_CORRUPT);
    // ...and the next clean frame is fine again.
    CHECK_EQ((int)feed(a, kReadHoldingReq, 0x11, &f), (int)RTU_FRAME_OK);
}

TEST(ModbusRtu, ParsesReadRequests) {
    RtuAssembler a; rtuReset(a);
    RtuFrame f; ModbusRequest r;
    feed(a, kReadHoldingReq, 0x11, &f);
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_NONE);
    CHECK_EQ((int)r.address, 0x6B);
    CHECK_EQ((int)r.quantity, 3);
    feed(a, Bytes{ 0x01, 0x01, 0x00, 0x13, 0x00, 0x13, 0x8C, 0x02 }, 1, &f);   // read coils 0x13 x19
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_NONE);
    CHECK_EQ((int)r.function, 1);
    CHECK_EQ((int)r.quantity, 0x13);
}

TEST(ModbusRtu, ParsesWriteSingleCoilAndRegister) {
    RtuAssembler a; rtuReset(a);
    RtuFrame f; ModbusRequest r;
    feed(a, Bytes{ 0x01, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x4C, 0x1B }, 1, &f);
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_NONE);
    CHECK_EQ((int)r.address, 0xAC);
    CHECK_EQ((int)r.value, 1);
    feed(a, Bytes{ 0x01, 0x06, 0x00, 0x01, 0x00, 0x03, 0x98, 0x0B }, 1, &f);
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_NONE);
    CHECK_EQ((int)r.address, 1);
    CHECK_EQ((int)r.value, 3);
}

TEST(ModbusRtu, ParsesWriteMultipleCoilsAndRegisters) {
    RtuAssembler a; rtuReset(a);
    RtuFrame f; ModbusRequest r;
    feed(a, Bytes{ 0x01, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD, 0x01, 0x72, 0xCB }, 1, &f);
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_NONE);
    CHECK_EQ((int)r.quantity, 10);
    CHECK_TRUE(modbusGetBit(r.values, 0));    // 0xCD = 1100 1101, LSB first
    CHECK_TRUE(!modbusGetBit(r.values, 1));
    CHECK_TRUE(modbusGetBit(r.values, 8));    // 0x01
    CHECK_TRUE(!modbusGetBit(r.values, 9));
    feed(a, Bytes{ 0x01, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02, 0x92, 0x30 }, 1, &f);
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_NONE);
    CHECK_EQ((int)modbusGetRegister(r.values, 0), 0x000A);
    CHECK_EQ((int)modbusGetRegister(r.values, 1), 0x0102);
}

namespace {
// Builds a frame from address + function + data with the CRC appended.
RtuFrameStatus feedPdu(RtuAssembler& a, Bytes adu, RtuFrame* f) {
    uint16_t crc = crc16Modbus(adu.data(), (uint16_t)adu.size());
    adu.push_back((uint8_t)crc);
    adu.push_back((uint8_t)(crc >> 8));
    return feed(a, adu, 1, f);
}
}  // namespace

TEST(ModbusRtu, RequestExceptions) {
    RtuAssembler a; rtuReset(a);
    RtuFrame f; ModbusRequest r;
    feedPdu(a, { 1, 0x08, 0x00, 0x00, 0x12, 0x34 }, &f);   // diagnostics: not implemented
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_ILLEGAL_FUNCTION);
    feedPdu(a, { 1, 0x03, 0x00, 0x00, 0x00, 126 }, &f);    // 126 registers > 125
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_ILLEGAL_DATA_VALUE);
    feedPdu(a, { 1, 0x03, 0x00, 0x00, 0x00, 0x00 }, &f);   // quantity 0
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_ILLEGAL_DATA_VALUE);
    feedPdu(a, { 1, 0x01, 0x07, 0xD1, 0x00, 0x00 }, &f);   // bit read, quantity 0
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_ILLEGAL_DATA_VALUE);
    feedPdu(a, { 1, 0x01, 0x00, 0x00, 0x07, 0xD1 }, &f);   // 2001 bits > 2000
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_ILLEGAL_DATA_VALUE);
    feedPdu(a, { 1, 0x03, 0xFF, 0xFF, 0x00, 0x02 }, &f);   // runs past 0xFFFF
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_ILLEGAL_DATA_ADDRESS);
    feedPdu(a, { 1, 0x05, 0x00, 0x01, 0x12, 0x34 }, &f);   // coil value not 0/FF00
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_ILLEGAL_DATA_VALUE);
    feedPdu(a, { 1, 0x10, 0x00, 0x01, 0x00, 0x02, 0x03, 0, 1, 2 }, &f);   // byte count 3 != 4
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_ILLEGAL_DATA_VALUE);
    feedPdu(a, { 1, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x02, 0xCD }, &f);     // data shorter than byte count
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_ILLEGAL_DATA_VALUE);
    feedPdu(a, { 1, 0x03, 0x00, 0x00, 0x00 }, &f);         // truncated PDU
    CHECK_EQ((int)modbusParseRequest(f, &r), (int)MODBUS_EX_ILLEGAL_DATA_VALUE);
}

TEST(ModbusRtu, BuildsResponsesMatchingPymodbus) {
    uint8_t out[MODBUS_RTU_MAX_ADU];
    const uint16_t regs[3] = { 0x022B, 0x0000, 0x0064 };
    CHECK_TRUE(built(out, modbusBuildReadRegisters(out, 0x11, 0x03, regs, 3),
                     { 0x11, 0x03, 0x06, 0x02, 0x2B, 0x00, 0x00, 0x00, 0x64, 0xC8, 0xBA }));
    const uint16_t in[1] = { 0x000A };
    CHECK_TRUE(built(out, modbusBuildReadRegisters(out, 1, 0x04, in, 1), { 0x01, 0x04, 0x02, 0x00, 0x0A, 0x39, 0x37 }));
    // Coils 0x13..0x25 (19 bits): CD 6B 05; stray bits past 19 must be sent as 0.
    uint8_t bits[3] = { 0xCD, 0x6B, 0xFD };
    CHECK_TRUE(built(out, modbusBuildReadBits(out, 1, 0x01, bits, 19),
                     { 0x01, 0x01, 0x03, 0xCD, 0x6B, 0x05, 0x42, 0x82 }));
    CHECK_TRUE(built(out, modbusBuildWriteSingle(out, 1, 0x05, 0xAC, 1), { 0x01, 0x05, 0x00, 0xAC, 0xFF, 0x00, 0x4C, 0x1B }));
    CHECK_TRUE(built(out, modbusBuildWriteSingle(out, 1, 0x06, 0x01, 3), { 0x01, 0x06, 0x00, 0x01, 0x00, 0x03, 0x98, 0x0B }));
    CHECK_TRUE(built(out, modbusBuildWriteMultiple(out, 1, 0x0F, 0x13, 10), { 0x01, 0x0F, 0x00, 0x13, 0x00, 0x0A, 0x24, 0x09 }));
    CHECK_TRUE(built(out, modbusBuildWriteMultiple(out, 1, 0x10, 0x01, 2), { 0x01, 0x10, 0x00, 0x01, 0x00, 0x02, 0x10, 0x08 }));
    CHECK_TRUE(built(out, modbusBuildException(out, 1, 0x03, MODBUS_EX_ILLEGAL_DATA_ADDRESS), { 0x01, 0x83, 0x02, 0xC0, 0xF1 }));
}

TEST(ModbusRtu, SetBitPacksLsbFirst) {
    uint8_t p[2] = { 0, 0 };
    modbusSetBit(p, 0, true);
    modbusSetBit(p, 9, true);
    CHECK_EQ((int)p[0], 0x01);
    CHECK_EQ((int)p[1], 0x02);
    modbusSetBit(p, 0, false);
    CHECK_EQ((int)p[0], 0x00);
}

TEST(ModbusRtu, LargestReadResponseFitsTheBuffer) {
    uint8_t out[MODBUS_RTU_MAX_ADU];
    uint16_t regs[MODBUS_MAX_READ_REGISTERS] = {};
    CHECK_EQ((int)modbusBuildReadRegisters(out, 1, 0x03, regs, MODBUS_MAX_READ_REGISTERS), 255);
    uint8_t bits[250] = {};
    CHECK_EQ((int)modbusBuildReadBits(out, 1, 0x01, bits, MODBUS_MAX_READ_BITS), 255);
}
