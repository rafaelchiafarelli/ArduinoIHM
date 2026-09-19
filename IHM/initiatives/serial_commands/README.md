# Initiative: serial_commands

PC-driven serial control of the board's hardware outputs, and PC-driven
simulation of its hardware inputs, for bench testing against real
hardware without going through the on-screen touch-free UI.

## Transport

The board's existing private MAVLink dialect (`mavlink/ihm_dialect.xml`),
carried on the debug/programming `Serial` port at 250000 baud -- the same
port `MavlinkComms` already uses for `IHM_BOARD_STATE` telemetry and the
`CAN_SIGNAL_CONFIG` / `RS485_SIGNAL_CONFIG` inbound configs. No second
UART, no build-flag mode: inbound MAVLink command frames and outbound
telemetry coexist on the port exactly as the two existing inbound configs
already do. `MavlinkComms::poll()` is already called once per superloop
pass in `main.cpp` and dispatches by `msgid`.

Rationale for MAVLink over the hand-rolled `lib/Comms/SerialCommunication`
framing (which is unwired and unrelated -- see `lib/Comms/README.md`) and
over a new ASCII line protocol: robust framing/CRC already in place, one
decode path, and a PC side (`workspace/IHM-PCApp`, plus throwaway
pymavlink scripts) that already speaks this dialect.

## Layering rule

`MavlinkComms` owns the wire protocol only -- it decodes and stores
inbound command payloads and exposes getters, the same storage-only shape
as `getCanSignalConfig()`. It never gains a dependency on `PWM`,
`MultiOutput`, `MotorDC`, or the Janus UI headers. `main.cpp` is the only
place that reads a decoded command and drives hardware with it -- the
same split as `sendBoardState()` (the class packs the message,
`main.cpp` gathers the values).

## Epics

| Epic | Status | Scope |
|---|---|---|
| `pwm_control` | planned | PC configures the 4 PWM channels (2 simplex, 2 complex) over MAVLink. |
| `relay_control` | telemetry done | Board -> PC read-only relay-state bitmask (`IHM_RELAY_STATE`), on the `relay_control` branch. PC -> board relay *control* is still not scoped -- see below. |
| `input_simulation` | encoder sim done | PC injects simulated encoder CW/CCW (`IHM_SIMULATE_ENCODER`), on the `input_simulation` branch, applied only when the real encoder is idle. Simulated button presses not yet scoped -- see below. |

### Not yet scoped (future epics/tasks under this initiative)

- **`relay_control` (PC -> board control)** -- actual relay on/off
  commands from the PC, now that the reverse (telemetry) direction
  exists. The `Relay` driver is confirmed-working.
- **`input_simulation` (simulated button presses)** -- same wire pattern
  as `IHM_SIMULATE_ENCODER`, extended to the 7 buttons (4 standalone + 3
  encoder push-buttons); needed to simulate activating/confirming a
  focused item, not just moving focus.
- **`motor_control`** -- `MotorDC` drive/stop over MAVLink. Blocked on
  confirming `MotorDC`'s bit layout against `IOs IHM.xlsx` / the KiCad
  schematic first (see `IHM/NEXT-SESSION.md`); exercising it over serial
  is a way to *do* that confirmation.
- **`dac_control`** -- `MCP4725` voltage output over MAVLink. Blocked on
  the `twi.c` unbounded-wait hang in `dac*.begin()` (see
  `IHM/NEXT-SESSION.md`) -- that fix is its own task, not folded in here.

## Branch chain

```
dev -> features -> serial_commands -> epics -> <epic> -> tasks -> <task>
```

`pwm_control`, `relay_control`, and `input_simulation` are sibling epic
branches off the shared `epics` container -- each gets merged into
`epics` independently once its own tasks are done; `epics` only
promotes up to `serial_commands` once every epic here has landed in it.
