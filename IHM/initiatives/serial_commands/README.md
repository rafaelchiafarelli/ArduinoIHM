# Initiative: serial_commands

PC-driven serial control of the board's hardware outputs, for bench
testing against real hardware without going through the on-screen
touch-free UI.

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
| `serial_transport` | **reverted** | Was Timer2-tick MAVLink RX + drop-not-block TX on `Serial`. Reverted: Serial0 is debug-only, MAVLink must go on another serial. Needs re-planning. |
| `pwm_control` | tasks done, bench pending | PC configures the 4 PWM channels (2 simplex, 2 complex) over MAVLink. |

| `pc_companion` | tasks done, bench pending | The Windows companion (`IHMPCController`) gains PWM/serial send UI + a how-to-use doc. |

### Not yet scoped (future epics under this initiative)

- **`relay_control`** -- relay on/off over MAVLink. The `Relay` driver is
  confirmed-working; lowest-risk of the remaining outputs.
- **`motor_control`** -- `MotorDC` drive/stop over MAVLink. Blocked on
  confirming `MotorDC`'s bit layout against `IOs IHM.xlsx` / the KiCad
  schematic first (see `IHM/NEXT-SESSION.md`); exercising it over serial
  is a way to *do* that confirmation.
- **`dac_control`** -- `MCP4725` voltage output over MAVLink. Blocked on
  the `twi.c` unbounded-wait hang in `dac*.begin()` (see
  `IHM/NEXT-SESSION.md`) -- that fix is its own task, not folded in here.

## Branch chain

```
dev -> features -> serial_commands -> epics -> pwm_control -> tasks -> <task>
```
