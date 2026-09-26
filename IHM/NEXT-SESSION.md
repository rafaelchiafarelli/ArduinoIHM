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

- **`MotorDC` bit layout is a placeholder.** The bit positions within its
  one latched byte (`enA`=bit0, `dirA`=bit1, `enB`=bit2, `dirB`=bit3) are
  unconfirmed against `IOs IHM.xlsx` / the KiCad schematic. The backend
  driver (bus wiring + software-PWM speed control) works; there is no
  on-screen TFT tab for motor output yet, and that tab is a real effort
  of its own.
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
- **PWM tab shows a stale frequency next to the live one.** Each channel
  row in `lib/GUI/pwm.screen.yaml` still has a static placeholder label
  (`pwm_chN_freq`: "1200Hz" / "2400Hz" / "500Hz" / "8000Hz") beside the live
  `chN_state_label`. Remove the placeholders and regenerate (planned as a
  fix on `dev`, after serial_commands merges).
- **A full-screen redraw after an on-screen action blocks the superloop
  > 0.7 s** (`janus_render_screen` in main.cpp's ACTION case). Two presses
  inside that window merge into one, real or simulated. Relevant to the
  `navigation` initiative (redraw only dirty widgets instead).
- **BusStatus tab passive refresh.** `bus_status_instance`'s CAN/RS-485
  fields only repaint on tab-switch or box-toggle -- the ~100ms tick
  redraws only the status bar. Live refresh there needs its own trigger
  (e.g. an action fired from the MAVLink receive path). See the comment
  in `main.cpp`'s ~100ms block and `ARCHITECTURE.md`'s UI section.
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
from. A successful flash there does **not** verify `Relay` / `MotorDC`'s
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

## Related repo

Two PC apps exist. **`C:\Users\rafae\source\repos\IHMPCController`**
(Win32, no third-party libs; `HOW_TO_USE.md`) is the current companion the
`serial_commands` initiative targets. The older `workspace/IHM-PCApp` -- the
PC-side counterpart. Win32/DirectX11/ImGui
app that speaks this board's MAVLink dialect over the debug/programming
COM port: shows `IHM_BOARD_STATE` telemetry, sends `CAN_SIGNAL_CONFIG` /
`RS485_SIGNAL_CONFIG` (which the SERIAL tab displays). Separate repo, its
own handoff notes.
