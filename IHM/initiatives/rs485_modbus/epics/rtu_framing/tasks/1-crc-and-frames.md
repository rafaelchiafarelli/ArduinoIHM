# Task 1: crc-and-frames

**Status:** planned
**Depends on:** nothing

## Contract

1. Pure `lib/ModbusRtu`: `crc16Modbus(buf, len)`; a frame assembler fed
   bytes plus "silence >= 3.5 char" events; address filter (own address,
   broadcast 0).
2. Request parsing for FC 01/02/03/04/05/06/15/16 (bounds: quantity limits
   per the spec: 2000 bits, 125 registers read, 123 write), and
   response and exception (01/02/03/04) builders.
3. Native tests with spec and pymodbus vectors.
