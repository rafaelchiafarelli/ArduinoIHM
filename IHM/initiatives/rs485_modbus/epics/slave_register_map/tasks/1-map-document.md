# Task 1: map-document

**Status:** planned -- blocked on open question 3 (map details)
**Depends on:** nothing

## Contract

`docs/MODBUS_MAP.md` (for integrators), covering:

- **Discrete inputs (FC 02):** buttons B0-B3, encoder push buttons.
- **Input registers (FC 04):** analog inputs, battery voltage, encoder
  counts/direction, relay mask, PWM applied state (per channel), bus
  config readback.
- **Coils (FC 01/05/15):** relays 0-7.
- **Holding registers (FC 03/06/16):** PWM per channel (selector or Hz,
  TOP, enable/invert/duty per output), bus config (per open question 6).

Every entry states its address, type, unit/scaling, range and access.
The document is the contract `2-read-and-write` implements.
