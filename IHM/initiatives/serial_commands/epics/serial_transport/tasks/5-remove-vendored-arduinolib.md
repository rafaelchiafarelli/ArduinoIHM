# Task 5: remove-vendored-arduinolib

**Status:** done
**Branch:** `5-remove-vendored-arduinolib` (from `tasks`)
**Depends on:** task 4

## Contract

Rafael, 2026-09-23: remove the vendored copy of the Arduino core
(`lib/ArduinoLib`). The project builds against the stock PlatformIO
`framework = arduino` instead. Four files in the copy differed from stock
(`HardwareSerial0.cpp`, `Stream.h`, `wiring.c`, `Wire.cpp`); per Rafael,
they were not compared and those local edits are dropped. The `Serial2`
rule in `lib/Uart2/README.md` still applies: the stock core has its own
`HardwareSerial2.cpp` with the same vectors.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; references to
`ArduinoLib` in docs/comments updated.
