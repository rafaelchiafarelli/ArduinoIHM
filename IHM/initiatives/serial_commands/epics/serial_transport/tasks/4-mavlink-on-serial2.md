# Task 4: mavlink-on-serial2

**Status:** done
**Branch:** `4-mavlink-on-serial2` (from `tasks`, after task 3 merged)
**Depends on:** task 3 (`lib/Uart2`)

## Contract

### Delivers

1. **`lib/MavlinkComms/src/MavlinkComms.h`**
   - The constructor takes no serial pointer. Add `begin(uint32_t baud)`,
     which calls `uart2::begin`. `MAVLINK_SERIAL_BAUD` defaults to 250000
     and is `#ifndef`-guarded.
   - `poll()` is removed. `fast_handler()` replaces it: it drains at most
     `MAVLINK_RX_BYTES_PER_TICK` (default 32) bytes via `uart2::read` into
     `mavlink_frame_char_buffer()` and dispatches on `MAVLINK_FRAMING_OK`.
     It must be ISR-safe: no `Serial`, no heap, bounded.
   - `sendBoardState` / `sendRelayState` enqueue through
     `uart2::writeFrame` (drop-not-block).
   - The superloop accessors become atomic copy-outs:
     `takePwmChannelConfig`, `consumeSimulatedEncoderDirection`,
     `consumeSimulatedButtons`, plus
     `bool getCanSignalConfig(uint8_t bus, mavlink_can_signal_config_t*)` /
     `bool getRs485SignalConfig(mavlink_rs485_signal_config_t*)` (these
     previously returned pointers).
2. **`src/main.cpp`**
   - `MavlinkComms mavlinkComms;` and `mavlinkComms.begin(MAVLINK_SERIAL_BAUD)`
     in `setup()`. `Serial.begin(250000)` stays as the debug port.
   - Delete the `mavlinkComms.poll()` call and its stopgap note.
   - At the end of `TIMER2_COMPA_vect`: busy-flag guard, `sei()`,
     `mavlinkComms.fast_handler()`, `cli()`.
   - `refreshBusStatusInstance()` uses the copy-out getters.
3. **Docs:** `mavlink/README.md` "Serial port" section (Serial2, ISR/ring/
   fast-handler path, drop-not-block TX, `--port` is now the adapter);
   `ARCHITECTURE.md` known gap 3; `NEXT-SESSION.md` MAVLink items;
   `CHANGELOG.md` entry; `demo/HARDWARE_RUNBOOK.md` port note.

### Tests

`MavlinkComms` pulls in AVR-only code, so there's no native test here. Don't
restructure it for testability. The coverage is the AVR build plus the bench
check in the epic acceptance gate.

## Definition of done

`platformio run` builds (report RAM/Flash before/after);
`test_native/run_tests.ps1` passes; task file marked done in the same commit.
Bench verification is Rafael's (on the new-revision board, not the old COM7
board).
