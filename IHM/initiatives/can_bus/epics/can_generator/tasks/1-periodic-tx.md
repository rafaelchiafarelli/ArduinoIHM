# Task 1: periodic-tx

**Status:** planned
**Depends on:** `mcp2515_driver/3`

## Contract

1. A pure scheduler (native-tested): given the config and a millisecond
   clock, when the next frame is due and when the repeat count is used up.
2. `main.cpp` sends due frames through the driver; a full TX buffer
   retries next pass (never blocks).
3. Once `serial_config` exists, the generator reads its config from
   `SerialConfig` instead of the raw MAVLink copy.
