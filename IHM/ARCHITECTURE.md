# ArduinoIHM Architecture

A top-down map of the project, for finding where to make a change. Each
module also has its own `README.md` (linked below) with more detail; this
document is about how they fit together. See `docs/architecture.drawio`
for a diagram of the same information.

Target: PlatformIO `megaatmega2560` (ATmega2560). See `IHM/README.md` for
the one-paragraph pitch (PWM function generator + relay/servo/motor I/O
behind a parallel-TFT touch-free UI) and `IHM/NEXT-SESSION.md` /
`IHM/CHANGELOG.md` for the current work log.

## The two strategies referenced everywhere

Two conventions repeat across this codebase; when other docs say "the
usual strategy," this is what they mean.

1. **Register-direct I/O, never `digitalWrite`/`analogWrite`.** Two
   shapes of the same idea, for two different needs:
   - [`lib/Ports/Ports.h`](lib/Ports/README.md)'s `port_type` -- a full
     *pin* (register pointer + set/reset bitmask) as one unit. Used by
     [`BinaryOutputs`](lib/BinaryOutputs/README.md) (20-pin output table)
     and `BinaryInput` (15-pin input table) to build declarative,
     compile-time pin tables.
   - [`lib/HAL/RegisterIO.h`](lib/HAL/README.md)'s `Reg8`/`Reg16` -- a
     single *register* handle. Used by [`PWM`](lib/MultiOutput/README.md)
     so its timer-register math (`applySimplexPWMConfig`/
     `applyComplexPWMConfig`) can run identically on-device and in
     native host tests.
   Both are a bare `volatile` pointer at heart -- no virtual dispatch, no
   Arduino pin-number lookup table, compiles down to the same
   instructions as naming the SFR directly.

2. **No `delay()` -- a hardware timer tick drives scheduling instead.**
   `ISR(TIMER2_COMPA_vect)` in `src/main.cpp` fires every ~1.008ms and
   dispatches to modules at three cadences (every tick / every 25th tick
   / an unused every-10th-tick slot); `main()`'s `while(1)` superloop
   handles everything else with no fixed cadence. See
   [`src/README.md`](src/README.md) for the exact dispatch table. PWM/
   servo timing runs on its own dedicated hardware timers (Timer1/3/4/5),
   not on delay loops.

The one place these two rules don't apply is the vendored
[`lib/Display`](lib/Display/README.md) parallel-TFT driver, and even
there: it turns out to already follow rule 1 (its `PIN_LOW`/`PIN_HIGH`
macros compile to direct `PORTx` bit ops on this target), and its rule-2
exceptions (`delay()` in the reset/init sequence) are confined to
one-time startup before the scheduler is even running -- see that
module's README for the full investigation (2026-08-12).

## Module map

```
                         ISR(TIMER2_COMPA_vect)  ~1.008ms tick
                                    |
        +---------------+----------+----------+------------------+
        | every tick     every 25th tick        every 10th tick   |
        v                v                      (unused slot)     |
  BinaryInputs      MultiOutput.slow_handler                      |
  .fast_handler()     -> Relay.ultra_slow_handler()               |
        |            Comms.fast_handler()  (parses; see gap below)|
        |            RotaryEncoder.ms_handler()                   |
        v                                                         |
      bMap  ---------------------------------------------------> main()
                                                                 superloop
  ISR(TIMER1_COMPA_vect)  -- present, empty (see gap below)
                                                                    |
   +----------------------------------------------------------------+
   | main() while(1) superloop -- no fixed cadence                  |
   |   buildButtonMap(bMap) -> btnMap                                |
   |   RotaryEncoder.getDirection() x3 -> dir[]                      |
   |   GUI.update(dir, btnMap)  ---> Elements (PWMScreen/RelayScreen)|
   |                                    -> MultiOutput (PWM, Relay)  |
   |   MCP4725 dac0/dac1.setVoltage(voltage0/1)  (fed by Comms, once |
   |                                              the gap is fixed)  |
   +------------------------------------------------------------------+
```

| Layer | Modules | README |
|---|---|---|
| Composition root | `main.cpp`, `Timer2Config` | [src/README.md](src/README.md) |
| Inputs | `BinaryInput`, `RotaryEncoder`, `ButtonMap` | [lib/BinaryInput](lib/BinaryInput/README.md), [lib/RotaryEncoder](lib/RotaryEncoder/README.md) |
| Outputs | `BinaryOutputs`, `Relay`, `ServoMotor`, `MotorDC`, `PWM` + config/timing/label-format types | [lib/BinaryOutputs](lib/BinaryOutputs/README.md), [lib/MultiOutput](lib/MultiOutput/README.md) |
| UI | `GUI`, `Elements` (widgets, PWMScreen, RelayScreen), `PWMStateMachine` | [lib/GUI](lib/GUI/README.md), [lib/Elements](lib/Elements/README.md), [lib/StateMachine](lib/StateMachine/README.md) |
| Comms/peripherals | `SerialCommunication`, `MCP4725` | [lib/Comms](lib/Comms/README.md), [lib/MCP4725](lib/MCP4725/README.md) |
| HAL / shared low-level | `Ports`, `HAL/RegisterIO`, `BusIO` (vendored) | [lib/Ports](lib/Ports/README.md), [lib/HAL](lib/HAL/README.md), [lib/BusIO](lib/BusIO/README.md) |
| Vendored, mostly untouched | `Display` (parallel-TFT, in active use), `ArduinoLib`, `SD`, `TouchScreen` (not instantiated anywhere) | [lib/Display](lib/Display/README.md) |

## Shared hardware resource allocation

**Corrected 2026-08-12, superseding an earlier version of this section**
that assumed `Relay`/`ServoMotor`/`MotorDC` each drive independent GPIO
pins through `BinaryOutputs`. Checked against the user's `IOs IHM.xlsx`
(`v0` sheet) and confirmed directly with the user: that's not how the
real board works.

### The multiplexed output bus (`Relay`/`ServoMotor`/`MotorDC`)

`Relay`, `ServoMotor`, and `MotorDC` share one 8-bit data bus feeding
three separate `74LS373` transparent latches, one per device, each
captured by its own strobe line:

| Signal | AVR pin | Role |
|---|---|---|
| Data bus (8 bits) | `PC2,PC1,PC0,PD7,PG2,PG1,PG0,PL7` | Shared -- holds the byte about to be latched |
| `dig_0` | `PH6` | Strobe -- Servo's latch |
| `dig_1` | `PG5` | Strobe -- Relay's latch |
| `dig_2` | `PF4` | Strobe -- Motor's latch |
| `OUTPUT_EN` | `PB4` | Shared tri-state control, not part of the write sequence |

A `74LS373` is *transparent*, not edge-triggered: its outputs follow the
inputs continuously while its enable line is high, and hold whatever
value was present the instant that line falls. So a correct write is:
**settle the data bus -> raise the target device's strobe -> drop it
again** -- the *falling* edge of the strobe is what actually captures the
byte into that device's latch, leaving the other two devices' latches
untouched. Each device needs to keep its own current 8-bit state in RAM
and rewrite the whole byte on every change (the bus is shared and
byte-wide, not individually addressable per bit).

**`BinaryOutputs::SetOutput()` does not implement this protocol** -- it
does immediate, independent per-pin GPIO writes with no bus/strobe
sequence at all. This means `Relay`'s existing UI wiring (2026-08-11,
previously believed complete) likely does not drive real hardware
correctly. Writing a driver for this protocol is `NEXT-SESSION.md` item
0, and blocks `ServoMotor`/`MotorDC` UI wiring (items 1/2) as well as
fixing `Relay`.

**Open questions for whoever implements this:** `MotorDC`'s exact bit
layout within its one byte (confirmed 1 device, `dig_2`, layout tbd); and
an atomicity concern, since the bus is physically shared hardware -- a
write from `ServoMotor`'s ISR context racing a write from `Relay`'s or
`MotorDC`'s foreground context could corrupt either write unless writes
are bracketed against interrupts.

### Hardware timers (PWM only -- fully decoupled from the bus above)

| Timer | Owner | Status |
|---|---|---|
| Timer2 | System tick (`ISR(TIMER2_COMPA_vect)`) | active |
| Timer1 | PWM channel 2 (`OC1A/B/C`, complex) | active (PWM tab, channel index 2) |
| Timer3 | PWM channel 0 (`OC3A`, simplex) | active (PWM tab, channel index 0) |
| Timer4 | PWM channel 3 (`OC4A/B/C`, complex) | active (PWM tab, channel index 3) |
| Timer5 | PWM channel 1 (`OC5A`, simplex) | active (PWM tab, channel index 1) |

All 8 of these timer-compare pins (`OC1A/B/C`, `OC3A`, `OC4A/B/C`,
`OC5A`) are confirmed direct-to-output, no buffer, and are entirely
separate from `Relay`/`ServoMotor`/`MotorDC`'s bus above -- `ServoMotor`
does not use `OCR4A`/Timer4 under the current hardware design (an earlier
version of this section described a Timer4 conflict between `ServoMotor`
and PWM channel 3; that no longer applies, since `ServoMotor` goes
through the bus instead).

## Known gaps (as of 2026-08-12)

These aren't bugs introduced by any recent change -- they're pre-existing
gaps this documentation pass surfaced while mapping the codebase. Listed
here so they're visible, not implying any of them need fixing today.

1. **`ServoMotor::timer_handler()` is never called**, and its whole
   `OCR4A`-based pulse-generation approach needs rethinking now that
   `ServoMotor` is known to go through the multiplexed bus, not a
   dedicated timer -- see "The multiplexed output bus" above.
   (`NEXT-SESSION.md` item 1, blocked on item 0.)
2. **`MotorDC::fast_handler()` is never called** (stepper microstepping
   logic is fully written, unreachable). `setMotorA`/`setMotorB`'s DC
   speed control (`analogWrite`, commented out) was also never ported to
   direct-register PWM. (`NEXT-SESSION.md` item 2, blocked on item 0.)
3. **`BinaryOutputs::SetOutput()` doesn't implement the multiplexed-bus
   protocol** `Relay`/`ServoMotor`/`MotorDC` actually need (immediate
   independent per-pin GPIO writes instead of bus-settle + strobe) -- see
   "The multiplexed output bus" above. This is the real blocker, not a
   pin-index overlap (an earlier version of this list described one
   between `MotorDC` and `ServoMotor`; that was based on the same wrong
   independent-GPIO model and no longer applies).
4. **`SerialCommunication::receive()` is never called.** Nothing forwards
   incoming UART0 bytes to it (the real RX ISR only fills the standard
   `Serial` ring buffer) -- so the framing/checksum parser it feeds can
   never produce a complete frame, `voltage0`/`voltage1` never update
   from real serial input, and the two `MCP4725` DACs are currently
   driven by whatever they were last set to (always 0 today). See
   [lib/Comms/README.md](lib/Comms/README.md) for what wiring this up
   would need.
5. **`lib/StateMachine/StateMachine.hpp`/`.cpp` is dead code that doesn't
   compile** (references an undefined type). Nothing includes it --
   `PWMStateMachine.h` (same folder) is what's actually used. Safe to
   delete.
6. ~~**`BinaryInput`'s `MCUCR |= ~(1<<PUD)`** doesn't do what its comment
   says -- sets unrelated `MCUCR` bits instead of clearing `PUD`.~~ --
   **fixed 2026-08-12**, changed to `MCUCR &= ~(1<<PUD)`.
7. **`MCP4725` dac0/dac1 vs. voltage0/voltage1 naming is crossed** in
   `main.cpp` (`dac1.setVoltage(voltage0,...)`, `dac0.setVoltage(voltage1,...)`)
   -- may be intentional (matching board wiring) but worth a deliberate
   check. See [lib/MCP4725/README.md](lib/MCP4725/README.md).
8. **`lib/Display/SPITFT.cpp`/`GrayOLED.cpp`** are vendored but entirely
   unreferenced -- dead weight from the library import, not part of the
   active display path (`Display`/`GFX`/`mcufriend_shield.h`).
9. **`AnalogInputs::read()` blocks on a polling loop** (`while (ADCSRA &
   (1<<ADSC));`) waiting for each conversion to finish -- ~104us/channel,
   up to ~520us total across all 5 channels every time `MavlinkComms`
   sends `IHM_BOARD_STATE` (~every 100ms). Not a `delay()` call, but the
   same class of problem this project's rule 2 (no blocking waits, a
   timer tick drives scheduling instead -- see "The two strategies
   referenced everywhere" above) exists to avoid. Should become
   interrupt-driven (`ADIE` + `ISR(ADC_vect)`, cycling through the 5
   channels asynchronously rather than blocking the superloop on each
   one) -- not fixed yet, flagged so it isn't mistaken for the
   established pattern. See [lib/AnalogInput/src/AnalogInput.h](lib/AnalogInput/src/AnalogInput.h).

## What's already solid

Worth naming so it isn't lost among the gaps above: `Relay` is fully
wired UI-to-`BinaryOutputs` and was the template for how the
output-wiring pattern should look at the UI/module level -- though per
the multiplexed-bus finding above, `BinaryOutputs` itself likely doesn't
drive the real hardware correctly yet, so "wired" here means the
software plumbing, not confirmed-working-on-a-real-board. `PWM`'s
register math is natively unit-tested and was hardened for a real
register-clobbering bug (see `CHANGELOG.md`, 2026-08-10) -- and per the
bus finding, `PWM`'s timers are confirmed unaffected by any of this,
direct-to-output as designed. `BinaryInputs`/`Ports`/`RegisterIO` give
the whole project a consistent, tested register-access foundation that
every other module builds on without reinventing it.
