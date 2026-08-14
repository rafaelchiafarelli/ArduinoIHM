# MultiOutput

Everything that drives a physical output lives here: relays, servos, DC/
stepper motors, and the 4-channel PWM generator. `MultiOutput` is the
composition root. `Relay`/`ServoMotor`/`MotorDC` don't own independent
pins -- they're three devices behind one shared multiplexed bus (see
below); `PWM` is fully separate, direct-to-output hardware timers.

## Files

| File | Owns | API surface |
|---|---|---|
| `Relay.h` | One device on the multiplexed bus (`dig_1`) | `setRelay`, `enableRelay`, `disableRelay`, `ultra_slow_handler` |
| `ServoMotor.h/.cpp` | One device on the multiplexed bus (`dig_0`) | `load`, `enableEngine`, `timer_handler` |
| `MotorDC.h` (`.cpp` is a 0-byte placeholder) | One device on the multiplexed bus (`dig_2`) | `setup`, `setMotorA/B`, `stopMotorA/B`, `fast_handler` |
| `PWM.h/.cpp` + `PWMConfig.cpp` | Timer1/3/4/5 (see table below) | `setupPWMChannel0..3` |
| `PWMChannelConfig.h/.cpp` | -- (pure data + math) | UI-editable single-channel PWM config, feeds `applySimplexPWMConfig` |
| `PWMTiming.h/.cpp` | -- (pure data + math) | frequency-selector -> prescaler/WGM-bits lookup |
| `PWMLabelFormat.h/.cpp` | -- (pure formatting) | renders PWM config fields as display strings |
| `RelayConfig.h` | -- (pure data) | UI-editable on/off state, mirrors `PWMChannelConfig`'s shape |
| `MultiOutput.h/.cpp` | composes all of the above | `setup`, `timer_handler`, `fast_handler`, `slow_handler`, `getRelays` |

`PWMChannelConfig`/`PWMTiming`/`PWMLabelFormat`/`RelayConfig` have no AVR
dependency and are the natively-unit-tested layer (`test_native/`) --
[HAL/RegisterIO.h](../HAL/README.md) is what makes that possible for the
register-writing half (`PWMConfig.cpp`'s `applySimplexPWMConfig`/
`applyComplexPWMConfig`).

## PWM channel -> timer map (from `PWM.h`)

| Channel | Timer | Outputs | Caller |
|---|---|---|---|
| 0 | Timer3 | OC3A (simplex) | `PWMSimplex` #0 |
| 1 | Timer5 | OC5A (simplex) | `PWMSimplex` #1 |
| 2 | Timer1 | OC1A/B/C (complex) | `PWMComplex` #2 |
| 3 | Timer4 | OC4A/B/C (complex) | `PWMComplex` #3 |

## Known gaps in the handler wiring (as of 2026-08-12)

`MultiOutput`'s handler methods exist but two of the four are not
actually reachable from `main.cpp` today:

- `MultiOutput::slow_handler()` -> `Relay::ultra_slow_handler()` -- **is**
  called, from `TIMER2_COMPA_vect`'s ~25ms branch. UI -> `Relay::setRelay`
  -> `MultiplexedBus::write()` now implements the real settle-strobe-drop
  protocol (see below) -- "wired" still means software-verified only, no
  physical hardware was available to confirm an actual relay click.
- `MultiOutput::fast_handler()` -> `MotorDC::fast_handler()` (stepper
  microstepping) -- **not called anywhere.** Both call sites in
  `main.cpp` are commented out. `MotorDC` also still has `analogWrite(...)`
  commented out in `setMotorA`/`setMotorB` for DC speed control -- unlike
  `PWM.cpp`, speed was never ported to direct-register PWM.
- `MultiOutput::timer_handler()` -> `ServoMotor::timer_handler()` --
  **not called anywhere**, and its `OCR4A`-based approach needs
  rethinking now that `ServoMotor` is known to go through the
  multiplexed bus below rather than owning a dedicated timer.

## The multiplexed output bus (driver written 2026-08-13)

An earlier version of this section described a Timer4 register conflict
between PWM channel 3 and `ServoMotor`. That was based on a wrong
assumption -- that `Relay`/`ServoMotor`/`MotorDC` each drive independent
GPIO pins via `BinaryOutputs`. Checked against the user's `IOs IHM.xlsx`
and confirmed directly with the user: they don't. All three share one
8-bit data bus feeding three separate `74LS373` latches, one per device,
each captured by its own strobe line:

| Signal | AVR pin | Role |
|---|---|---|
| Data bus (8 bits) | `PC2,PC1,PC0,PD7,PG2,PG1,PG0,PL7` | Shared |
| `dig_0` | `PH6` | Strobe -- Servo's latch |
| `dig_1` | `PG5` | Strobe -- Relay's latch |
| `dig_2` | `PF4` | Strobe -- Motor's latch |
| `OUTPUT_EN` | `PB4` | Shared tri-state control (not part of the write sequence) |

A `74LS373` is transparent (not edge-triggered): outputs follow the
inputs while the enable line is high, and hold whatever was present the
instant it falls. Correct write sequence: **settle the data bus -> raise
the target device's strobe -> drop it again** -- the falling edge
captures the byte. Each device needs to track its own current byte in
RAM and rewrite the whole thing on any change (the bus is shared,
byte-wide, not individually addressable).

**None of `PWM`'s 8 hardware-PWM pins (`OC1A/B/C`, `OC3A`, `OC4A/B/C`,
`OC5A`) are part of this bus** -- they're confirmed direct-to-output, no
buffer, entirely separate from Relay/Servo/Motor. The Timer4 conflict this
section used to describe doesn't apply: `ServoMotor` doesn't touch Timer4
or any PWM-capable pin under this design.

**`MultiplexedBus`** (`lib/MultiOutput/src/MultiplexedBus.h`) is that
driver: `write(strobeIndex, byte)` settles all 8 data-bus bits (via
`BinaryOutputs::SetOutput()`, indices 0-7), raises the target device's
strobe, then drops it, all inside `ATOMIC_BLOCK(ATOMIC_RESTORESTATE)` --
`ServoMotor`'s ISR-context writes and `Relay`/`MotorDC`'s foreground
writes can't interleave and tear a byte mid-sequence. `enableOutputs()`
drives `OUTPUT_EN` (index 11) **low** once at setup -- confirmed against
the KiCad schematic that `74LS373`'s `OE` pin is the part's only
electrically-inverted pin, i.e. active-low, so low is what actually
enables the latches' outputs (not the more intuitive-looking `true`).
Holds a `const BinaryOutputs&` (not a copy) since `BinaryOutputs` is a
160-byte table and every device already keeps its own copy -- a fourth
copy here would cost another 160 bytes of already-scarce RAM for nothing.

`Relay` is wired to it: `relays[]` still holds one bool per relay, but
every mutation now calls a private `refreshBus()` that assembles all 8
relays' state into one byte and calls
`bus.write(MUX_RELAY_STROBE, value)`, instead of the old
one-`SetOutput()`-per-relay immediate-write model. Net RAM effect was a
*decrease* (79.4% vs. 81.3% before), not an increase, despite the new
class -- the single assembled-byte write compiles smaller than the old
per-relay call sequence did.

`ServoMotor`/`MotorDC` are **not** touched by this change -- they still
take a raw `BinaryOutputs` and are still not called from `main.cpp`'s
live handlers (see "Known gaps" below). `MultiplexedBus` is generic
enough for either to use once someone picks up items 1/2, but doing so
needs each device's own bit-layout decided first (see
`IHM/NEXT-SESSION.md`'s open sub-questions, still unresolved for
`MotorDC`).

## Depends on

[BinaryOutputs](../BinaryOutputs/README.md), [HAL/RegisterIO.h](../HAL/README.md).
