# GUI

Top-level screen orchestrator. Owns a `StatusBar`, `TabSelector`,
`PWMScreen`, and `RelayScreen` (all from [Elements](../Elements/README.md))
and draws through the injected `Display*`.

## API

- `setup()` -- one-time display init, called once from `main.cpp`'s
  `setup()`.
- `update(DIRECTION_TYPE d0, d1, d2, uint8_t btnMap)` -- the per-superloop
  UI tick: advances tab/field selection from encoder direction + button
  state, applies edits, redraws. Called every pass of `main()`'s `while
  (1)` loop, not from any ISR/handler.
- `updateTimeStatistics(double)` -- feeds the status bar's timing display.
- `rotateGUI`, `receiveData`, `showWarning/showEmergency/
  showNormalOperation/showRegularLoop` -- present but effectively unused;
  mostly placeholder debug text, not wired into real logic.

## Hardware resources

None directly -- `GUI` only holds a `Display*` and pointers into
`MultiOutput` (`PWM*`, `Relay*` via `multiOutput.getRelays()`), all owned
by `main.cpp`.

## Coupling

- Encoder/button bit-layout is hardcoded in `GUI::update()` (e.g.
  `0b01000000` for "select", `0b00100000` for tab-local) -- this
  duplicates knowledge of `ButtonMap`'s output format
  (`lib/RotaryEncoder/ButtonMap.h`) without calling into it directly;
  `main.cpp` calls `buildButtonMap()` and passes the result in.
- Includes `RotaryEncoder.h` only for the `DIRECTION_TYPE` enum, not the
  class itself.
- Direct compile-time dependency on `Display`, `PWM`, `Relay`.
