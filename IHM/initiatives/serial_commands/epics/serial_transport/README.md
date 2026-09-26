# Epic: serial_transport

Move the board's MAVLink link off the debug port and onto **Serial2 (USART2,
RX2 = D17, TX2 = D16)**, and replace the polled `HardwareSerial` path with an
**interrupt-driven** one: tiny RX/TX ISRs that only move bytes between the
UART and ring buffers, plus one bounded fast handler that does the parsing
and decoding.

## History

The first plan (tasks 1-2, 2026-09-20) put the same idea on `Serial`
(Serial0). That was reverted: **Serial0 is debug-only**. Tasks 1-2 are
superseded by tasks 3-4 below; their branches (`1-rx-tick-parser`,
`2-tx-nonblocking`) hold only the reverted attempt.

## Why (Rafael, 2026-09-23)

> move it to Serial2, and change the low level from polling to interruption.
> the interruption itself is extremely small moving the received bytes to a
> buffer and one fast handler making the necessary processing and eventually
> decoding. the same for transmission

## Design (decided here, not left to the implementer)

```
 PC --USB-TTL--> RX2 --USART2_RX_vect--> rxRing[64] --fast_handler (Timer2)--> parser --> dispatch --> slots
 PC <--USB-TTL-- TX2 <-USART2_UDRE_vect-- txRing[64] <--send*() (superloop, whole frame or drop)
```

- **Own USART2 driver, not `HardwareSerial`.** `lib/Uart2` defines
  `USART2_RX_vect` / `USART2_UDRE_vect` itself. The Arduino core's
  `HardwareSerial2.cpp` (which defines the same vectors) is only linked when
  something references `Serial2`, so firmware code must never touch
  `Serial2`. The `serial2_loopback_test` bench env still uses `Serial2`.
  That is fine because it is a separate env that doesn't link `Uart2`.
- **RX ISR:** read `UCSR2A` + `UDR2`, count line errors (DOR2/FE2), push
  into the ring (count if full). Nothing else.
- **TX ISR:** pop one byte into `UDR2`; when the ring is empty, disable
  `UDRIE2`. Nothing else.
- **Rings:** `ByteRing<N>`: single-producer/single-consumer, power-of-2,
  `uint8_t` indices, no locking (each index has exactly one writer).
  Header-only and host-testable.
- **Fast handler:** `MavlinkComms::fast_handler()` feeds at most
  `MAVLINK_RX_BYTES_PER_TICK` (default 32, `-D` overridable) bytes per tick
  to `mavlink_frame_char_buffer()` and dispatches complete frames into the
  storage slots. At 250000 baud the line rate is ~25 B/ms, so 32 per ~1 ms
  tick keeps up with a saturated line.
- **It runs last in `TIMER2_COMPA_vect`, and the whole tick is
  preemptible** (`sei()` is its first statement -- task 6). Why: AVR ISRs
  don't nest, and USART2 has ~3 bytes of hardware slack (~120 us at 250k).
  The tick's other handlers alone take up to ~220 us (measured), so any
  interrupts-off stretch that long overruns RX. There is no re-entry guard:
  the tick must always finish inside its ~1 ms slot. (Task 4 re-enabled
  interrupts only around the parser, behind a busy flag; the bench showed
  that still lost ~60 % of PC -> board frames.)
- **TX is drop-not-block:** `send*()` packs in the superloop, then enqueues
  the whole frame or nothing (a partial frame would corrupt the stream) and
  counts the drop. The next ~100 ms telemetry frame replaces it.
- **Every ISR-to-superloop hand-off is an atomic copy-out**
  (`ATOMIC_BLOCK(ATOMIC_RESTORESTATE)`), never a pointer into storage the
  fast handler writes.
- **Baud stays 250000** (`MAVLINK_SERIAL_BAUD`, `-D` overridable). That's 0%
  error at 16 MHz with U2X, and the PC scripts already default to it. Only
  `--port` changes (the USB-TTL adapter's COM port instead of the board's
  USB port).

## Tasks

```
3-uart2-irq-driver    lib/Uart2: ByteRing + USART2 ISRs + counters;       (no deps)
                      native ByteRing tests
4-mavlink-on-serial2  MavlinkComms on Uart2: fast_handler from Timer2,     (depends on 3)
                      atomic copy-outs, drop-not-block TX; main.cpp; docs
5-remove-vendored-arduinolib  stock framework = arduino                  (depends on 4)
6-preemptible-timer2-tick     sei() first in the Timer2 ISR (RX overrun   (depends on 4)
                              fix found on the bench)
```

## Acceptance gate

- `platformio run` builds; RAM/Flash reported (flag a material jump).
- `test_native/run_tests.ps1` passes.
- Bench (Rafael, on the new-revision board with the USB-TTL adapter on
  D16/D17): telemetry arrives on the adapter's COM port at 250000;
  `pwm_config.py --port <adapter>` and `sim_input.py` still work; a sustained
  stream from the companion causes no framing/CRC errors while the UI keeps
  rendering.
