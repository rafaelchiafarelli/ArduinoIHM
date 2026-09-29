#pragma once
#include <stdint.h>

/**
 * Modbus RTU framing, pure (initiative rs485_modbus, rtu_framing task 1):
 * CRC-16/MODBUS, a frame assembler driven by bytes plus "silence" events,
 * the address filter, request parsing for the slave's function codes, and
 * response / exception builders. No UART, timer or register map here --
 * uart3_driver feeds the assembler, slave_register_map answers requests.
 * Host-tested (test_native/test_modbus_rtu.cpp).
 *
 * An ADU is: address, function code, data, CRC (low byte first). A frame
 * ends when the line is silent for 3.5 character times; the UART layer
 * detects that and calls rtuEndOfFrame().
 */

// Largest RTU ADU the spec allows (address + 253-byte PDU + CRC).
#ifndef MODBUS_RTU_MAX_ADU
#define MODBUS_RTU_MAX_ADU 256
#endif

#define MODBUS_BROADCAST_ADDRESS 0
#define MODBUS_MIN_SLAVE_ADDRESS 1
#define MODBUS_MAX_SLAVE_ADDRESS 247

// Function codes the slave implements (rs485_modbus decision 2 / question 5).
#define MODBUS_FC_READ_COILS 0x01
#define MODBUS_FC_READ_DISCRETE_INPUTS 0x02
#define MODBUS_FC_READ_HOLDING_REGISTERS 0x03
#define MODBUS_FC_READ_INPUT_REGISTERS 0x04
#define MODBUS_FC_WRITE_SINGLE_COIL 0x05
#define MODBUS_FC_WRITE_SINGLE_REGISTER 0x06
#define MODBUS_FC_WRITE_MULTIPLE_COILS 0x0F
#define MODBUS_FC_WRITE_MULTIPLE_REGISTERS 0x10

// Quantity limits from the spec.
#define MODBUS_MAX_READ_BITS 2000
#define MODBUS_MAX_READ_REGISTERS 125
#define MODBUS_MAX_WRITE_BITS 1968
#define MODBUS_MAX_WRITE_REGISTERS 123

enum ModbusException {
    MODBUS_EX_NONE = 0,
    MODBUS_EX_ILLEGAL_FUNCTION = 0x01,
    MODBUS_EX_ILLEGAL_DATA_ADDRESS = 0x02,
    MODBUS_EX_ILLEGAL_DATA_VALUE = 0x03,
    MODBUS_EX_SERVER_DEVICE_FAILURE = 0x04
};

/** CRC-16/MODBUS (poly 0xA001 reflected, init 0xFFFF). Sent low byte first. */
uint16_t crc16Modbus(const uint8_t* buf, uint16_t len);

// ---- frame assembly ------------------------------------------------------

enum RtuFrameStatus {
    RTU_FRAME_OK,            // valid frame for us (or broadcast): *out filled
    RTU_FRAME_EMPTY,         // silence with no bytes: nothing happened
    RTU_FRAME_TOO_SHORT,     // fewer than 4 bytes (address, function, CRC)
    RTU_FRAME_OVERFLOW,      // more than MODBUS_RTU_MAX_ADU bytes
    RTU_FRAME_CORRUPT,       // rtuMarkCorrupt() during the frame (1.5-char gap)
    RTU_FRAME_BAD_CRC,
    RTU_FRAME_NOT_FOR_US     // valid CRC, another slave's address
};

struct RtuAssembler {
    uint8_t buf[MODBUS_RTU_MAX_ADU];
    uint16_t len;
    uint8_t overflow;
    uint8_t corrupt;
};

/** A received frame; `data` points into the assembler (valid until the next byte). */
struct RtuFrame {
    uint8_t address;         // 0 = broadcast
    uint8_t function;
    const uint8_t* data;     // after the function code, CRC excluded
    uint16_t dataLen;
};

void rtuReset(RtuAssembler& a);

/** One received byte (UART RX ISR). */
void rtuPushByte(RtuAssembler& a, uint8_t b);

/** A gap > 1.5 characters inside a frame: the frame is discarded at its end. */
void rtuMarkCorrupt(RtuAssembler& a);

/**
 * Silence >= 3.5 characters: the frame is complete. Checks length, CRC and
 * address (ownAddress or broadcast), fills *out on RTU_FRAME_OK, and
 * readies the assembler for the next frame. *out stays valid until the
 * next rtuPushByte().
 */
RtuFrameStatus rtuEndOfFrame(RtuAssembler& a, uint8_t ownAddress, RtuFrame* out);

// ---- request parsing -----------------------------------------------------

struct ModbusRequest {
    uint8_t function;
    uint16_t address;        // starting address (0-based, as on the wire)
    uint16_t quantity;       // bits or registers; 1 for FC 05/06
    uint16_t value;          // FC 05: 0 or 1 (0xFF00 on the wire); FC 06: the register value
    const uint8_t* values;   // FC 0F: packed bits, FC 10: big-endian registers
};

/**
 * Parses a frame's PDU. Returns MODBUS_EX_NONE and fills *out, or the
 * exception to answer: ILLEGAL_FUNCTION for a function code the slave
 * doesn't implement, ILLEGAL_DATA_VALUE for a malformed PDU or a quantity
 * outside the spec's limits (or a coil value other than 0x0000/0xFF00),
 * ILLEGAL_DATA_ADDRESS when address + quantity runs past 0xFFFF. Whether
 * the addresses exist is the register map's check, not this one.
 */
ModbusException modbusParseRequest(const RtuFrame& f, ModbusRequest* out);

/** Bit i (0-based) of packed FC 0F values / FC 01-02 response data. */
bool modbusGetBit(const uint8_t* packed, uint16_t i);
/** Register i (0-based) of big-endian FC 10 values. */
uint16_t modbusGetRegister(const uint8_t* values, uint16_t i);
/** Sets bit i of a packed bit buffer (for building FC 01/02 responses). */
void modbusSetBit(uint8_t* packed, uint16_t i, bool on);

// ---- response building ---------------------------------------------------
// Each writes a complete ADU (address ... CRC) to `out` (at least
// MODBUS_RTU_MAX_ADU bytes) and returns its length.

/** FC 01/02: `quantity` bits, packed LSB-first as the spec sends them. */
uint16_t modbusBuildReadBits(uint8_t* out, uint8_t address, uint8_t function,
                             const uint8_t* packedBits, uint16_t quantity);

/** FC 03/04: `quantity` registers. */
uint16_t modbusBuildReadRegisters(uint8_t* out, uint8_t address, uint8_t function,
                                  const uint16_t* registers, uint16_t quantity);

/** FC 05/06: the echo of the request (FC 05 value is 0/1 -> 0x0000/0xFF00). */
uint16_t modbusBuildWriteSingle(uint8_t* out, uint8_t address, uint8_t function,
                                uint16_t registerAddress, uint16_t value);

/** FC 0F/10: starting address and quantity written. */
uint16_t modbusBuildWriteMultiple(uint8_t* out, uint8_t address, uint8_t function,
                                  uint16_t registerAddress, uint16_t quantity);

/** Exception response: function | 0x80, then the exception code. */
uint16_t modbusBuildException(uint8_t* out, uint8_t address, uint8_t function, ModbusException code);
