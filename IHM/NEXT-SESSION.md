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
- **`MavlinkComms::poll()` drains its whole RX ring buffer in one
  unbounded `while (serial->available())` loop.** A burst of buffered
  bytes can hold up the rest of the superloop until they're all parsed.
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

## Related repo

`workspace/IHM-PCApp` -- the PC-side counterpart. Win32/DirectX11/ImGui
app that speaks this board's MAVLink dialect over the debug/programming
COM port: shows `IHM_BOARD_STATE` telemetry, sends `CAN_SIGNAL_CONFIG` /
`RS485_SIGNAL_CONFIG` (which the SERIAL tab displays). Separate repo, its
own handoff notes.
