# Task 1: spi-transactions

**Status:** planned -- blocked on can_bus open question 3 (Serial1 given up for D18/D19)
**Depends on:** nothing

## Contract

1. A small `lib/SpiBus` (or the Arduino `SPI` library's
   `beginTransaction`/`usingInterrupt`) giving each device (CAN-1 CS D18,
   CAN-2 CS D28, SD CS per open question 2) its own settings (mode,
   clock: MCP2515 <= 10 MHz, so 8 MHz; SD 4-8 MHz) and CS handling.
2. Rule: a transaction started from the main loop masks the CAN INT
   lines (or all interrupts) for its duration; the ISR-side CAN read
   uses the same wrapper.
3. D53 (SS) kept as an output so the ATmega stays SPI master.
4. Document the rule in `ARCHITECTURE.md`.
