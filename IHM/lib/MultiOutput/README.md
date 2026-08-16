# MultiOutput

Everything that drives a physical output lives here: relays, DC/stepper
motors, and the 4-channel PWM generator. `MultiOutput` is the
composition root. `Relay`/`MotorDC` don't own independent pins -- they're
two devices behind one shared multiplexed bus (see below); `PWM` is
fully separate, direct-to-output hardware timers.

**Servo control is not part of the IHM solution.** `ServoMotor` (a third
device that would have used the multiplexed bus below, on `dig_0`) was
deleted 2026-08-16 -- it predated the bus finding, its `OCR4A`-based pulse
generation directly conflicted with `PWM` channel 3's live use of that
register, and giving it real position resolution under this bus would
have needed timing precision no free hardware timer here can provide
(see `CHANGELOG.md`'s 2026-08-16 entry for the full reasoning). A
dedicated servo controller is planned as separate future work, not a
revival of this driver.

## Files

| File | Owns | API surface |
|---|---|---|
| `Relay.h` | One device on the multiplexed bus (`dig_1`) | `setRelay`, `enableRelay`, `disableRelay`, `ultra_slow_handler` |
| `MotorDC.h` (`.cpp` is a 0-byte placeholder) | One device on the multiplexed bus (`dig_2`) | `setMotorA/B`, `stopMotorA/B`, `fast_handler` |
| `PWM.h/.cpp` + `PWMConfig.cpp` | Timer1/3/4/5 (see table below) | `setupPWMChannel0..3` |
| `PWMChannelConfig.h/.cpp` | -- (pure data + math) | UI-editable single-channel PWM config, feeds `applySimplexPWMConfig` |
| `PWMTiming.h/.cpp` | -- (pure data + math) | frequency-selector -> prescaler/WGM-bits lookup |
| `PWMLabelFormat.h/.cpp` | -- (pure formatting) | renders PWM config fields as display strings |
| `RelayConfig.h` | -- (pure data) | UI-editable on/off state, mirrors `PWMChannelConfig`'s shape |
| `MultiOutput.h/.cpp` | composes all of the above | `setup`, `fast_handler`, `slow_handler`, `getRelays` |

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

## Known gaps in the handler wiring (as of 2026-08-16)

`MultiOutput` has two handler methods, both actually reachable from
`main.cpp` today:

- `MultiOutput::slow_handler()` -> `Relay::ultra_slow_handler()` -- **is**
  called, from `TIMER2_COMPA_vect`'s ~25ms branch. UI -> `Relay::setRelay`
  -> `MultiplexedBus::write()` now implements the real settle-strobe-drop
  protocol (see below) -- "wired" still means software-verified only, no
  physical hardware was available to confirm an actual relay click.
- `MultiOutput::fast_handler()` -> `MotorDC::fast_handler()` -- **fixed
  2026-08-16**, now called from `TIMER2_COMPA_vect`'s every-tick branch.
  `MotorDC` was rewritten onto `MultiplexedBus` (`MUX_MOTOR_STROBE`), same
  pattern as `Relay`; stepper commutation and DC speed control (coarse
  software PWM, ~99Hz carrier / 10% duty steps -- no dedicated fast timer
  is free, see below) both go through it. Still open: the bit layout
  within `MotorDC`'s one latched byte is a **placeholder**
  (`enA`=bit0/`dirA`=bit1/`enB`=bit2/`dirB`=bit3), unconfirmed against
  `IOs IHM.xlsx`/the KiCad schematic; and the on-screen TFT UI tab for
  motor output is unstarted.

(`MultiOutput::timer_handler()` -> `ServoMotor::timer_handler()` used to
be listed here as never-called, `OCR4A`-conflicting dead code -- both
sides are gone now, along with the empty `ISR(TIMER1_COMPA_vect)` in
`main.cpp` that only ever existed to call it.)

**Why `MotorDC` doesn't use a hardware timer for speed control:**
Timer1/3/4/5 are fully committed to `PWM`'s 4-channel generator (all 8 of
their compare-output pins are live PWM outputs). Timer3/Timer5 each only
drive one of their three compare units as an actual output (`OC3A`,
`OC5A` -- simplex channels), so their other compare units are technically
free to fire an interrupt without touching a pin -- but doing that would
couple `MotorDC`'s PWM carrier frequency to whatever prescaler that PWM
channel's user-editable frequency setting currently has. Rejected in
favor of driving duty-cycle toggling off the existing ~1.008ms system
tick instead: coarser resolution (10 steps/period at best), but zero new
coupling between two otherwise-independent subsystems.

## The multiplexed output bus (driver written 2026-08-13)

An earlier version of this section described a Timer4 register conflict
between PWM channel 3 and the since-deleted `ServoMotor`. That was based
on a wrong assumption -- that `Relay`/`ServoMotor`/`MotorDC` each drive
independent GPIO pins via `BinaryOutputs`. Checked against the user's
`IOs IHM.xlsx` and confirmed directly with the user: they don't. All
three share one 8-bit data bus feeding three separate `74LS373` latches,
one per device, each captured by its own strobe line -- though only two
of the three latches are actually driven by firmware in this repo (see
below):

| Signal | AVR pin | Role |
|---|---|---|
| Data bus (8 bits) | `PC2,PC1,PC0,PD7,PG2,PG1,PG0,PL7` | Shared |
| `dig_0` | `PH6` | Strobe -- Servo's latch (physically wired, undriven -- servo control isn't part of the IHM solution) |
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
buffer, entirely separate from Relay/Motor. The Timer4 conflict this
section used to describe doesn't apply to anything live in this repo
anymore: the code that would have touched `OCR4A` is deleted.

**`MultiplexedBus`** (`lib/MultiOutput/src/MultiplexedBus.h`) is that
driver: `write(strobeIndex, byte)` settles all 8 data-bus bits (via
`BinaryOutputs::SetOutput()`, indices 0-7), raises the target device's
strobe, then drops it, all inside `ATOMIC_BLOCK(ATOMIC_RESTORESTATE)` --
`Relay`'s foreground writes and `MotorDC`'s ISR-context writes (called
every ~1ms tick) can't interleave and tear a byte mid-sequence.
`enableOutputs()` drives `OUTPUT_EN` (index 11) **low** once at setup --
confirmed against the KiCad schematic that `74LS373`'s `OE` pin is the
part's only electrically-inverted pin, i.e. active-low, so low is what
actually enables the latches' outputs (not the more intuitive-looking
`true`). Holds a `const BinaryOutputs&` (not a copy) since
`BinaryOutputs` is a 160-byte table and every device already keeps its
own copy -- a fourth copy here would cost another 160 bytes of
already-scarce RAM for nothing.

`Relay` is wired to it: `relays[]` still holds one bool per relay, but
every mutation now calls a private `refreshBus()` that assembles all 8
relays' state into one byte and calls
`bus.write(MUX_RELAY_STROBE, value)`, instead of the old
one-`SetOutput()`-per-relay immediate-write model. Net RAM effect was a
*decrease* (79.4% vs. 81.3% before), not an increase, despite the new
class -- the single assembled-byte write compiles smaller than the old
per-relay call sequence did. `MotorDC` was moved onto the same driver
2026-08-16 (assembles both motors' enable/direction bits into one byte
via its own `refreshBus()`, `bus.write(MUX_MOTOR_STROBE, value)`) -- its
bit layout is a placeholder, not yet confirmed against the schematic.

`ServoMotor`, the third device this driver was built generic enough to
support, was deleted 2026-08-16 rather than wired up -- servo control
isn't part of the IHM solution. A future, separate servo controller
could reuse `MultiplexedBus` on `dig_0` if it ends up using this same
bus, but that's a decision for whoever designs it, not an obligation
carried over from this driver.

## Depends on

[BinaryOutputs](../BinaryOutputs/README.md), [HAL/RegisterIO.h](../HAL/README.md).
