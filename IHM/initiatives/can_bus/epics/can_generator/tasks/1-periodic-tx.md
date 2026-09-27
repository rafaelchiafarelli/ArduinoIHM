# Task 1: periodic-tx

**Status:** planned
**Depends on:** `event_timer/1`, `mcp2515_driver/2`, `mcp2515_driver/3`

## Contract

1. Event-driven: enabling the generator arms an `event_timer` for
   `period_ms`; each expiry queues one frame on the driver's TX queue,
   decrements the repeat count and re-arms (unless done). No loop checks a
   deadline.
2. A full TX queue drops the frame and counts it (never blocks, never
   retries by polling).
3. Pure repeat/period bookkeeping native-tested.
4. Once `serial_config` exists, the generator reads its config from
   `SerialConfig` instead of the raw MAVLink copy.
