# Task 2: slave-engine

**Status:** planned -- blocked on open question 2 (where the slave runs)
**Depends on:** task 1

## Contract

1. A slave engine: complete request -> register-map callbacks
   (read bits/registers, write bits/registers) -> response queued to the
   UART, with a bounded amount of work per call so it can run in the
   Timer2 tick next to `MavlinkComms::fast_handler()`.
2. Broadcast writes are applied without a reply.
3. Counters: CRC errors, exceptions sent, requests served (for
   diagnostics and a future FC 08).
