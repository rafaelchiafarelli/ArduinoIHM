# Task 2: rx-ring-and-isr

**Status:** planned
**Depends on:** task 1; RAM budget per can_bus open question 7

## Contract

1. Per-bus RX ring of fixed-size frames (id, ext, dlc, data[8],
   timestamp), size set by the RAM budget (proposal 16 frames per bus,
   about 225 B each).
2. The module's INT (D19/INT2, D3/INT5) ISR drains RX0/RX1 into the ring
   and counts overflows; the main loop pops frames.
3. Native tests for the ring (wrap, overflow count, pop order).
4. Bench: sustained frames from the other module, overflow count stays 0
   at a rate recorded here, while the TFT redraws.
