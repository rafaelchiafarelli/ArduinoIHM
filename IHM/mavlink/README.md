# mavlink

The board's wire protocol to the PC: a private MAVLink dialect, not the
hand-rolled `SerialCommunication` framing (see `lib/Comms/README.md` --
that class is unrelated and untouched by this).

- `ihm_dialect.xml` -- the source of truth. Message field layout deliberately
  does *not* bit-pack across byte boundaries (e.g. each rotary encoder gets
  its own byte instead of 3 encoders sharing 9 bits) -- with
  `MAVLINK_MAX_PAYLOAD_LEN` raised to 64 for headroom, the few bytes tight
  packing would save weren't worth the hand-rolled bit-shift/mask code on
  both ends.
- `generate.py` -- regenerates `generated/` from the XML. Requires
  `pip install pymavlink`. Run after any edit to `ihm_dialect.xml`;
  `generated/` is committed (AVR builds shouldn't depend on Python/pymavlink
  being installed), so a stale `generated/` after an XML edit is a real bug,
  not just an inconvenience.
- `generated_py/ihm_dialect.py` -- mavgen Python binding of the same dialect, for
  bench tooling (`scripts/`). Committed like `generated/`; `generate.py` now emits
  both trees.
- `scripts/pwm_config.py` -- CLI that sends one `PWM_CHANNEL_CONFIG` (see
  `--help`; worked examples in `demo/HARDWARE_RUNBOOK.md`). Needs
  `pip install pymavlink pyserial`.
- `generated/` -- mavgen output, C only. This is a *complete*, self-contained
  MAVLink v2 C implementation (mavgen vendors `mavlink_helpers.h`,
  `mavlink_types.h`, `protocol.h`, `checksum.h` alongside the dialect-specific
  headers) -- no separate MAVLink library dependency in `platformio.ini`.
- No `<include>common.xml</include>`: none of our 3 messages reference
  anything from the standard dialect (no `HEARTBEAT`, etc. yet). Add the
  include back if that changes -- it's a large file, not pulled in for free.

## Messages (as of this writing)

| Message | Direction | Bytes | Purpose |
|---|---|---|---|
| `IHM_BOARD_STATE` | board -> PC | 19 | Buttons, 3x encoder direction, house-keeping, 4x analog input, loop-timing debug (`time_statistics`/`time_counter`, mirrors `main.cpp`'s variables of the same name) |
| `CAN_SIGNAL_CONFIG` | PC -> board | 20 | Configure/start/stop a generated signal on one of the 2 CAN buses (`bus_id` selects which) -- one active signal per bus, arbitrary bytes, no on-board waveform math |
| `RS485_SIGNAL_CONFIG` | PC -> board | 38 | Same idea for the single RS-485 connection -- no bus_id needed |
| `PWM_CHANNEL_CONFIG` | PC -> board | 13 | Configure one of the 4 PWM channels (0/1 simplex: output A only; 2/3 complex: outputs A/B/C) -- frequency selector, raw ICRn TOP for the variable mode, per-output enable/invert/duty. No ack; last writer wins |
| `IHM_RELAY_STATE` | board -> PC | 1 | Bitmask of all 8 relay outputs, read-only telemetry -- mirrors `main.cpp`'s `relayState[]`; no PC -> board relay command exists yet |
| `IHM_SIMULATE_ENCODER` | PC -> board | 2 | Inject one simulated CW/CCW rotation step on the given encoder; only applied when that encoder's real hardware read was idle the same pass -- real input always wins |
| `IHM_SIMULATE_BUTTON` | PC -> board | 1 | Press the buttons in `button_mask` (bit layout as `IHM_BOARD_STATE.buttons`) for exactly one superloop pass; OR-ed with the real read, so real input wins |
| `IHM_PWM_STATE` | board -> PC | 13 | The config currently applied to one PWM channel, same fields as `PWM_CHANNEL_CONFIG`; one channel per ~100 ms tick, round-robin (all 4 every ~400 ms). Mirrors `main.cpp`'s `pwmLast[]`, i.e. the readback `PWM_CHANNEL_CONFIG` has no ack for |
| `IHM_UI_STATE` | board -> PC | 8 | Janus's `janus_remote_state_t` (active screen, widget focus, nav focus, open-box bitmask), sent on change and every 500 ms, for the companion's screen mirror (`janus_remote_state_apply`). Initiative `desktop_mirror` |

Largest message is 38 bytes, hence the 64-byte cap (some margin for the
still-undesigned SD-card-status and UI-state messages -- see
`IHM/NEXT-SESSION.md`).

## Sending commands (PC side)

| Command | Message | Tool |
|---|---|---|
| Configure a PWM channel | `PWM_CHANNEL_CONFIG` (303) | companion app "PWM command" panel, or `scripts/pwm_config.py` |
| Simulate an encoder step | `IHM_SIMULATE_ENCODER` (305) | companion app CCW/CW buttons, or `scripts/sim_input.py encoder` |
| Simulate a button click | `IHM_SIMULATE_BUTTON` (306) | `scripts/sim_input.py button` (companion app has no button UI yet) |

None has an ack; the board's periodic `IHM_BOARD_STATE` is the proof of
life, and for PWM the periodic `IHM_PWM_STATE` shows what was actually applied. Invalid frames are dropped silently. The companion app
(`C:\Users\rafae\source\repos\IHMPCController`, see its `HOW_TO_USE.md`)
keeps its own copy of `generated/ihm_dialect/`: after regenerating here, copy
it over (last synced 2026-09-26, ids 300-307).

## Serial port

**MAVLink runs on Serial2 (USART2: RX2 = D17, TX2 = D16) at 250000 baud**
(`MAVLINK_SERIAL_BAUD`). On the PC that's the USB-TTL adapter's COM port,
not the board's USB port. **Serial0 (`Serial`, USB/programming) is
debug-only.** Scripts: `--port <adapter COM port>`; the default baud is
unchanged.

Receive and transmit are interrupt-driven (`lib/Uart2`, see its README):

```
RX: USART2_RX_vect -> rxRing[64] -> MavlinkComms::fast_handler() (Timer2 tick,
    <= MAVLINK_RX_BYTES_PER_TICK bytes, interrupts re-enabled) -> dispatch()
    -> storage slots -> superloop take*/consume*/get* (atomic copy-outs)
TX: send*() (superloop) packs -> uart2::writeFrame() whole frame or drop
    -> txRing[128] -> USART2_UDRE_vect -> UDR2
```

The ISRs only move bytes. Telemetry is best-effort: a frame that doesn't
fit in the TX ring is dropped and counted (`uart2::txDroppedCount()`), and
the next ~100 ms frame replaces it. Per ~100 ms tick the board queues
`IHM_BOARD_STATE` (31 B on the wire) + `IHM_RELAY_STATE` (13 B) +
`IHM_PWM_STATE` (25 B) = 69 B, which is why the TX ring is 128, not 64. RX ring overflow and UART
overrun/framing errors are counted too (`rxDroppedCount()`,
`rxLineErrorCount()`). None of these counters are in telemetry yet.

## Parsing: use `mavlink_frame_char_buffer()`, not `mavlink_parse_char()`

`MavlinkComms::fast_handler()` feeds incoming bytes to `mavlink_frame_char_buffer()`,
not the more commonly-shown `mavlink_parse_char()`. Measured, not
theoretical: switching from the latter to the former dropped this project's
build from 7059 to 6663 bytes of RAM (86.2% -> 81.3%) with zero functional
change. Cause: `mavlink_parse_char()`'s error-handling path references
`mavlink_get_channel_buffer()`/`mavlink_get_channel_status()`, which declare
`static mavlink_message_t m_mavlink_buffer[MAVLINK_COMM_NUM_BUFFERS]` and a
matching `mavlink_status_t` array (`MAVLINK_COMM_NUM_BUFFERS` defaults to 4
off-desktop) -- ~400 bytes of RAM that gets linked in regardless of whether
you supply your own buffers, purely because the function *can* fall back to
them. `mavlink_frame_char_buffer()` is the library's own documented
"parser that doesn't use any global variables" variant -- pass `NULL` for
its `r_message`/`r_mavlink_status` output params (unnecessary: once it
returns `MAVLINK_FRAMING_OK`, the complete message is already sitting in
whatever buffer you passed as `rxmsg`).

A smaller, unavoidable-without-hand-rolling-the-packer cost remains: sending
(`mavlink_msg_ihm_board_state_pack()` -> `mavlink_finalize_message()` ->
`mavlink_get_channel_status()`) still links the ~80-byte status array alone,
for per-channel outgoing sequence tracking. Not worth chasing.

## `MAVLINK_MAX_PAYLOAD_LEN`

Set via `-D` in `platformio.ini`, not a header define, so it's guaranteed to
apply before `mavlink.h` is included from *any* translation unit regardless
of include order. This only matters on the AVR build -- it's a local
RAM-sizing knob, not a wire-protocol parameter (actual transmitted bytes are
always the specific message's real length; MAVLink2 truncates trailing
zeros). A PC-side consumer of this same dialect can leave it at the library
default (255) since RAM isn't a constraint there.

## Regenerating after an XML change

```sh
pip install pymavlink   # once
python mavlink/generate.py   # writes generated/ (C) and generated_py/ (Python)
```

Regenerating rewrites version/date stamps in `generated/ihm_dialect/ihm_dialect.h`
and `mavlink.h` even when nothing else changed -- revert those two if the XML
didn't.
