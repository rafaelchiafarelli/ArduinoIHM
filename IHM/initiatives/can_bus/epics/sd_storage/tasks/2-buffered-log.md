# Task 2: buffered-log

**Status:** planned
**Depends on:** task 1, `main_loop_timing/1`

## Contract

1. Double-buffered appends (512 B sectors): the producer fills one
   buffer while the other is written, and full buffers are dropped with
   a counter rather than blocking.
2. Bench: sustained CAN logging at a rate recorded here with 0 drops for
   10 minutes, and the drop counter visible (debug or telemetry).
