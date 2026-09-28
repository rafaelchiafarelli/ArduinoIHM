# Task 2: mode-switch-and-generator

**Status:** planned
**Depends on:** task 1, `uart3_driver/1`, `slave_register_map/2`

## Contract

1. The RS-485 mode from `SerialConfig` (task 1) selects what owns
   USART3: nothing, the raw generator, or the Modbus slave engine.
2. Raw generator, event-driven: an `event_timer` expiry every
   `period_ms` starts one TX of 302's `data[0..length)` via the UDRE ISR,
   `repeat_count` times (0 = until disabled). No deadline checks.
3. Modbus slave: address, baud and format from `SerialConfig` (task 1).
4. A mode or parameter change re-initialises USART3 cleanly (TX drained,
   DE released), triggered by the change event, never by re-checking the
   config.
