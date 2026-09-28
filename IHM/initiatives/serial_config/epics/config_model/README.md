# Epic: config_model

The pure model behind every SERIAL setting, with no AVR, UI or MAVLink
dependencies, so it is native-testable like `PWMWireConfig`. Ships the
generator fields; bus initiatives extend it (initiative README,
"Extension contract").

## Tasks

```
1-config-struct-and-steps   SerialConfig generator fields, ranges, RE2 step helpers, extension recipe   (blocked: Q5)
2-eeprom-image              versioned + length + CRC EEPROM image, append-only extension, defaults      (deps: 1; blocked: Q4)
```

## Acceptance gate

`test_native/run_tests.ps1` passes with the new tests; `platformio run` builds.
