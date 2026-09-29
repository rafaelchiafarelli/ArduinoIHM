# Next session

The "where things stand / what's next" handoff. `IHM/CHANGELOG.md` has
the full per-change history; this file is only the currently-open work.

## Top priority

**The `MCP4725` DACs hang `setup()`.** `dac0.begin(0x62)` /
`dac1.begin(0x63)` block forever in `twi.c`'s unbounded TWI wait loops
when the DAC doesn't ACK. Both `.begin()` calls and the two
`dac*.setVoltage()` calls in `main.cpp` are commented out as a bypass,
not a fix. Resolve by either confirming the DAC wiring / I2C address
against real hardware, or adding a timeout around the TWI waits so a
missing DAC can't hang startup. This is the one thing between the current
build and full functionality -- the display works, DAC output doesn't.

## Open items

- **PC -> board DAC commands are not scoped.** Relays got
  `IHM_RELAY_COMMAND` (309) and companion switches on 2026-09-27.
  **dac_control** (`MCP4725` voltage over MAVLink) is blocked on the DAC
  hang in `setup()` (top priority above); that fix is its own task.
  (Motor control is gone: `MotorDC` was removed 2026-09-28 in
  `fixes/000013` -- the motor was never connected, and its latch's
  outputs are kept free for `rs485_modbus`'s RS-485 DE/RE line.)
- **Relay commands: on-screen check owed.** `IHM_RELAY_COMMAND` read back
  correctly on the bench (script and companion), but nobody has watched the
  TFT's Output tab switch/LED follow a PC command yet. Old board revision
  only, so the relay bus itself is also unverified (see Hardware
  verification).
- **`SerialCommunication::receive()` is never called.** Nothing forwards
  UART0 bytes into its framing/checksum parser, so `voltage0` /
  `voltage1` never update from real serial input. Needs a `USART0_RX_vect`
  override calling `comms.receive()`, or main-loop polling of
  `Serial.available()` / `Serial.read()`.
- **`MCP4725` dac0/dac1 vs. voltage0/voltage1 naming looks crossed** in
  `main.cpp` (`dac1.setVoltage(voltage0,...)`, `dac0.setVoltage(voltage1,...)`).
  May match board wiring -- check deliberately before assuming
  "channel 0 = voltage0".
- **MAVLink on Serial2 is bench-verified (2026-09-25)**, on COM3 (PL2303
  adapter on D16/D17). The Timer2 tick is now preemptible (serial_transport
  task 6); before that, ~60 % of PC -> board frames were lost to USART2
  overruns. The UART drop/error counters (`uart2::*Count()`) still aren't
  in telemetry. Add a field if trouble returns.
- **Full-screen redraws block the superloop** (`janus_render_screen`):
  > 0.7 s after an on-screen action (two presses inside that window merge
  into one), and 1.85 s on a switch to the PWM tab, which stalls
  `IHM_UI_STATE` past the companion mirror's 1.5 s "no board" timeout, so
  the overlay flashes (measured 2026-09-27). Owned by the
  `non_blocking_redraw` initiative (planned 2026-09-27, Rafael's choice
  over a longer mirror timeout).
- **`docs/architecture.drawio` is not regenerated.** Its module map
  predates the Janus-generated UI. `ARCHITECTURE.md`'s prose and
  `docs/gui-render-pipeline.drawio` are current; the drawio module map is
  not.

## Shelved

- **`IHM/ui-rotation.md`** -- portrait/landscape UI support driven by the
  housekeeping rotation pin (bit 4 of `bMap`, PA0), switched live via a
  non-blocking incremental redraw. Scoped, not started. Key open
  decision: where the redraw-scheduler logic lives. `ui-rotation.md` has
  the full context and open questions (notably how one sensor bit selects
  among 4 rotation angles).

## Hardware verification

The board currently connected (COM7) is an **old hardware revision** --
not the one the `MultiplexedBus` / `74LS373`-latch model was derived
from. A successful flash there does **not** verify `Relay`'s
write sequence. Ask the user before testing against whatever is connected.

## How to pick up dev work

- **Native unit tests:** `IHM/test_native/run_tests.ps1` (PowerShell;
  drives MSVC directly, no gcc on this machine -- see the script header).
  Adding a new native-testable file means adding it to the allowlists
  near the top (`$libAllowlist` for include dirs, `$prodSourceAllowlist`
  for production `.cpp` files) -- explicit allowlists, not globs, because
  including everything under `lib/` pulls in AVR-only headers that don't
  compile natively.
- **AVR build:** `platformio run` from `IHM/`. Re-run it for a current
  RAM/Flash number rather than trusting any figure written in a doc.
- **Demo:** `IHM/demo/run_demo.ps1` prints the register-level
  walkthrough; `IHM/demo/HARDWARE_RUNBOOK.md` has the equivalent checks
  for real hardware.

## Bench steps owed

1. `pwm_control`: the PWM tab mirrors every channel (verified
   2026-09-25). **The output pins themselves were not probed** -- scope
   OC3A / OC1A-B per `demo/HARDWARE_RUNBOOK.md` when an instrument is at
   hand.
2. `serial_config` (closed 2026-09-28, bench-verified by script and a
   headless companion run): turn RE2 by hand on a SERIAL field to judge
   the acceleration (4 clicks < 80 ms apart up a digit, 300 ms pause down
   one, 1.5 s reset -- `lib/BusConfig/src/SerialConfig.h`), and look at
   the companion mirror's SERIAL tab following a knob edit. Known gap:
   the mirror always shows data byte `B0` (the board's `B<n>` selection
   isn't in 311/312; adding it is a wire change).

## Related repo

Two PC apps exist. **`C:\Users\rafae\source\repos\IHMPCController`**
(Win32, no third-party libs; `HOW_TO_USE.md`) is the current companion. It
is not under git: each change leaves a `.bak` beside the edited file, and the
task file of whichever initiative made the change is the record. The older `workspace/IHM-PCApp` -- the
PC-side counterpart. Win32/DirectX11/ImGui
app that speaks this board's MAVLink dialect over the debug/programming
COM port: shows `IHM_BOARD_STATE` telemetry, sends `CAN_SIGNAL_CONFIG` /
`RS485_SIGNAL_CONFIG` (which the SERIAL tab displays). Separate repo, its
own handoff notes.
