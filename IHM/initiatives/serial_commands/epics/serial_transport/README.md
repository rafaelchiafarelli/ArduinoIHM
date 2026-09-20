# Epic: serial_transport

> **REVERTED 2026-09-20 -- do not implement as written.** This epic assumed
> MAVLink lives on the debug `Serial` (Serial0) port. It does not: **Serial0
> is debug-only**, and MAVLink (or any other protocol) must be carried on a
> *different* hardware serial (e.g. Serial2 / the regular COM port). The
> Timer2-tick RX and drop-not-block TX code was reverted; the design below is
> kept only as reference. Re-plan it against the real protocol serial once
> that port is chosen and wired. Tasks are reset to "not started".

Make the board's MAVLink receive/transmit path **non-blocking and
tick-driven**, so PC commands (PWM now, relay/motor/dac later) can never
stall the superloop or the UI.

## Why (Rafael's notes in `src/main.cpp`, 2026-09-20)

> serial protocol must not be blocking or dependent on the presence of
> data in the buffer. The interruption should be a few instructions only,
> and a handler function must be installed in the timer handler.

Today `mavlinkComms.poll()` runs in the superloop and drains the whole RX
buffer in one unbounded `while (serial->available())` (also listed in
`NEXT-SESSION.md`), and `serial->write()` spins when the 64-byte TX ring
is full.

## Design (decided here, not left to the implementer)

- **Bytes still arrive through the Arduino core's USART0 RX ISR** into its
  ring buffer -- that ISR already is "a few instructions". We do not
  replace it.
- **A bounded drain runs from the existing Timer2 tick** (`TIMER2_COMPA_vect`,
  ~1.008 ms): `MavlinkComms::tick()` feeds at most
  `MAVLINK_RX_BYTES_PER_TICK` bytes (default 16, `-D` overridable) to
  `mavlink_frame_char_buffer()`. When a frame completes it is decoded
  straight into the storage slots (a <=38-byte copy) and a pending/dirty
  flag is set. Nothing else happens in the ISR -- no hardware is driven
  from it.
- **The superloop only consumes flags** (`take*()` accessors), exactly the
  dirty-flag shape task 3 of `pwm_control` already used.
- **Every accessor that hands data from the ISR side to the superloop is
  an atomic copy-out** (`ATOMIC_BLOCK(ATOMIC_RESTORESTATE)`), never a
  pointer into ISR-written storage -- otherwise a frame landing mid-read
  tears the struct.
- **TX is drop-not-block**: a telemetry frame is sent only if
  `availableForWrite() >= len`, else dropped and counted.

Sizing: 250000 baud = ~25 B/ms worst case; 16 B/tick sustains ~16 kB/s,
far above any command rate (a PWM config is 12+13 B, sent by hand), and
the core's 64 B ring absorbs bursts. If a sustained-rate use ever appears,
raise the macro -- but see the risk below.

## Risk (needs Rafael's bench, cannot be verified in this session)

AVR ISRs don't nest. While the Timer2 tick runs, a USART0 byte waits; the
UART has ~2 bytes of slack (~80 us at 250k). The tick already does
`multiOuput.fast_handler()` + `userInputs.fast_handler()`; adding a drain
must not push the total past that. The drain is placed **last** in the ISR
and bounded, but the real number is unmeasured -- bench step: send a
sustained stream from the companion and confirm no CRC/framing errors.
Mitigation if it fails: lower `MAVLINK_RX_BYTES_PER_TICK`, or enable
`ISR_NOBLOCK` for the drain only.

## Tasks

```
1-rx-tick-parser     tick() bounded drain from Timer2; atomic take*()    (no deps)
                     accessors; main loop stops calling poll()
2-tx-nonblocking     drop-not-block telemetry writes + drop counter     (no deps; parallel-safe)
```

`pwm_control` task 3 depends on task 1 here (it consumes the atomic
accessors).

## Acceptance gate

- `platformio run` builds; RAM/Flash reported (flag a material jump).
- `test_native/run_tests.ps1` passes.
- Bench (Rafael): board keeps rendering the UI while the companion streams
  commands; no framing errors at 250000 baud.
