# Task 2: int-driven-io

**Status:** planned
**Depends on:** task 1; can_bus open questions 4 (ISR budget) and 7 (RAM)

## Contract

Strictly event-driven (initiative rule): nothing polls the module.

1. The module's INT (D19/INT2, D3/INT5) ISR re-enables interrupts
   (`sei()`), reads CANINTF, and:
   - **RX:** drains RX0/RX1 and hands each frame to the bus's registered
     consumer callback (protocol layer or generator) in ISR context,
     bounded. Frames nobody consumes go to a small per-bus RAM ring
     (proposal 16 frames, ~225 B) for the UI, and the superloop is told
     via a "work pending" flag.
   - **TX-complete:** starts the next frame from a per-bus TX queue.
   - **Errors:** updates error state; bus-off is reported the same way.
2. SPI access from the ISR goes through `spi_sharing`.
3. Native tests: TX queue and RX ring (wrap, overflow count).
4. Bench: sustained frames from the other module at a rate recorded
   here, with no loss while the TFT redraws, and USART2 (MAVLink) error
   counters unchanged.
