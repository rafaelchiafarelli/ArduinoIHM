# MultiOutput

Everything that drives a physical output lives here: relays and the
4-channel PWM generator. `MultiOutput` is the composition root. `Relay`
doesn't own independent pins -- it's one device behind a shared
multiplexed bus (see below); `PWM` is fully separate, direct-to-output
hardware timers.

**DC/stepper motor control is removed** (`fixes/000013`, 2026-09-28):
the motor was never connected, and `MotorDC` (constructed in stepper mode)
was re-latching a stepper sequence onto the unused motor latch every
~1 ms tick. Its latch (`dig_2`) stays wired; `MultiOutput::setup()`
latches 0 into it once, and its outputs are free for `rs485_modbus`'s
RS-485 DE/RE line. The deleted driver is in git history.

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
| `PWM.h/.cpp` + `PWMConfig.cpp` | Timer1/3/4/5 (see table below) | `setupPWMChannel0..3` |
| `PWMChannelConfig.h/.cpp` | -- (pure data + math) | UI-editable single-channel PWM config, feeds `applySimplexPWMConfig` |
| `PWMTiming.h/.cpp` | -- (pure data + math) | frequency-selector -> prescaler/WGM-bits lookup |
| `PWMLabelFormat.h/.cpp` | -- (pure formatting) | renders PWM config fields as display strings |
| `RelayConfig.h` | -- (pure data) | UI-editable on/off state, mirrors `PWMChannelConfig`'s shape |
| `RelayStore.h/.cpp` + `RelayStoreEeprom.h/.cpp` | EEPROM 256-351 | Relay state across power cycles (`fixes/000014`): every change is recorded in a 32-slot wear-levelled ring (~3.2M changes per cell budget), boot restores the last mask. The ring logic is pure and host-tested (`test_native/test_relay_store.cpp`); `main.cpp` saves once per superloop pass when the mask changed. |
| `MultiOutput.h/.cpp` | composes all of the above | `setup`, `slow_handler`, `getRelays` |

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
- `MultiOutput::fast_handler()` (-> `MotorDC::fast_handler()`, every
  tick) is gone with the motor driver (`fixes/000013`).

(`MultiOutput::timer_handler()` -> `ServoMotor::timer_handler()` used to
be listed here as never-called, `OCR4A`-conflicting dead code -- both
sides are gone now, along with the empty `ISR(TIMER1_COMPA_vect)` in
`main.cpp` that only ever existed to call it.)

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
| `dig_2` | `PF4` | Strobe -- Motor's latch (physically wired; no driver since `fixes/000013`, latched 0 at setup) |
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
buffer, entirely separate from this bus. The Timer4 conflict this
section used to describe doesn't apply to anything live in this repo
anymore: the code that would have touched `OCR4A` is deleted.

**`MultiplexedBus`** (`lib/MultiOutput/src/MultiplexedBus.h`) is that
driver: `write(strobeIndex, byte)` settles all 8 data-bus bits (via
`BinaryOutputs::SetOutput()`, indices 0-7), raises the target device's
strobe, then drops it, all inside `ATOMIC_BLOCK(ATOMIC_RESTORESTATE)` --
`Relay`'s foreground writes and any ISR-context write (the deleted motor
driver's, every ~1ms tick; a future RS-485 DE/RE line's) can't interleave
and tear a byte mid-sequence.
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
per-relay call sequence did. (`MotorDC` used the same driver on
`dig_2` from 2026-08-16 until its removal in `fixes/000013`.)

`ServoMotor`, the third device this driver was built generic enough to
support, was deleted 2026-08-16 rather than wired up -- servo control
isn't part of the IHM solution. A future, separate servo controller
could reuse `MultiplexedBus` on `dig_0` if it ends up using this same
bus, but that's a decision for whoever designs it, not an obligation
carried over from this driver.

## Depends on

[BinaryOutputs](../BinaryOutputs/README.md), [HAL/RegisterIO.h](../HAL/README.md).
