# Task 2: tx-nonblocking

**Status:** not started
**Branch:** `2-tx-nonblocking` (from `tasks`)
**Depends on:** nothing

## Contract

### Delivers

- `MavlinkComms` private helper `bool sendFrame(const uint8_t* buf, uint16_t len)`:
  writes only if `serial->availableForWrite() >= len`, otherwise increments
  a saturating `uint8_t txDropped` counter and returns `false`. Both
  `sendBoardState()` and `sendRelayState()` go through it.
- `uint8_t txDroppedCount() const` getter (for a future status field).
- No change to any message layout, no change to call sites in `main.cpp`.
- One-paragraph note in `mavlink/README.md`: telemetry is best-effort,
  dropped when the TX ring is full; the next tick's frame replaces it.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; task file
marked done in the same commit.
