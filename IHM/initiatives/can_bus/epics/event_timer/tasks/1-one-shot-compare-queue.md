# Task 1: one-shot-compare-queue

**Status:** planned -- blocked on can_bus open question 8 (which free timer)
**Depends on:** nothing

## Contract

1. `lib/EventTimer`: `arm(id, delay_us, callback)`, `cancel(id)`, and a
   fixed-size sorted queue (no malloc; proposal 8 slots). The compare
   register is always set for the earliest deadline only, so there is
   one interrupt per expiry and no periodic scanning.
2. Delays longer than one timer period are handled by counting the
   timer's own period interrupts (Timer2 Compare-B) or wraps
   (Timer0), whichever open question 8 picks, and recorded here.
3. The compare ISR re-enables interrupts (`sei()`) before running
   callbacks, and callbacks must be bounded (initiative open question 4).
4. Pure queue logic native-tested; the hardware part documented in
   `ARCHITECTURE.md` next to the Timer2 tick.
