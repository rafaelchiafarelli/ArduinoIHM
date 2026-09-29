# ModbusRtu

Modbus RTU framing for the RS-485 slave (initiative `rs485_modbus`,
`rtu_framing` epic). Pure C++, no UART, timer or register map, so it is
host-tested (`test_native/test_modbus_rtu.cpp`, vectors from the Modbus
spec as pymodbus builds them).

| Piece | What it does |
|---|---|
| `crc16Modbus()` | CRC-16/MODBUS; sent low byte first. |
| `RtuAssembler` + `rtuPushByte()` / `rtuMarkCorrupt()` / `rtuEndOfFrame()` | Collects a frame from RX bytes. The UART layer calls `rtuEndOfFrame()` on 3.5 characters of silence and `rtuMarkCorrupt()` on a > 1.5-character gap inside a frame. Checks length, CRC and address (own or broadcast 0). |
| `modbusParseRequest()` | Parses FC 01/02/03/04/05/06/15/16 and returns the exception to answer for an unknown function, a malformed PDU, an out-of-spec quantity (2000 bits / 125 registers read, 1968 / 123 write) or an address range past 0xFFFF. Whether an address exists is the register map's check. |
| `modbusBuild*()` | Complete response ADUs (with CRC): read bits / registers, write single / multiple echoes, exceptions. |

RAM: an `RtuAssembler` is 260 B (`MODBUS_RTU_MAX_ADU` 256 + state); the
response buffer is the caller's. Flash ~1.2 KB.
