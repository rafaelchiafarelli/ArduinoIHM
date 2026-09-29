# Task 1: crc-and-frames

**Status:** done (2026-09-28)
**Depends on:** nothing

## Contract

1. Pure `lib/ModbusRtu`: `crc16Modbus(buf, len)`; a frame assembler fed
   bytes plus "silence >= 3.5 char" events; address filter (own address,
   broadcast 0).
2. Request parsing for FC 01/02/03/04/05/06/15/16 (bounds: quantity limits
   per the spec: 2000 bits, 125 registers read, 123 write), and
   response and exception (01/02/03/04) builders.
3. Native tests with spec and pymodbus vectors.

## Result

- `lib/ModbusRtu/src/ModbusRtu.h/.cpp` (+ README): CRC-16/MODBUS, frame
  assembler (bytes, silence event, 1.5-char corrupt mark), address filter
  (own / broadcast), request parsing for FC 01/02/03/04/05/06/15/16 with
  the spec's limits, response and exception builders.
- `test_native/test_modbus_rtu.cpp`: 11 tests; vectors are the spec's
  examples as pymodbus 3.15 `FramerRTU` builds them (pymodbus installed
  on this machine for that). 197 native tests pass.
- Compiles clean for AVR (avr-g++ -Wall -Wextra): ~1.2 KB flash; an
  assembler costs 260 B of RAM once instantiated (not linked into the
  firmware yet -- `uart3_driver` / `rtu_framing/2` will).
- Decision left to the register map: whether an address *exists*
  (exception 02) -- the parser only rejects ranges past 0xFFFF.
