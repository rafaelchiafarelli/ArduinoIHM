# ArduinoIHM Architecture

A top-down map of the project, for finding where to make a change. Most
modules also have their own `README.md` (linked below) with more detail;
this document is about how they fit together. See
`docs/architecture.drawio` for a diagram of the module map and
`docs/gui-render-pipeline.drawio` for the on-screen UI's render path.

Target: PlatformIO `megaatmega2560` (ATmega2560). See `IHM/README.md` for
the one-paragraph pitch (PWM function generator + relay/motor I/O behind
a parallel-TFT touch-free UI) and `IHM/NEXT-SESSION.md` /
`IHM/CHANGELOG.md` for the current work log.

The on-screen TFT UI is generated code (Janus) -- read
[The UI is generated (`lib/GUI`)](#the-ui-is-generated-libgui) before
touching anything under `lib/GUI/`.

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
   [`src/README.md`](src/README.md) for the exact dispatch table. PWM
   timing runs on its own dedicated hardware timers (Timer1/3/4/5), not
   on delay loops.

The one place these two rules don't apply is the vendored
[`lib/Display`](lib/Display/README.md) parallel-TFT driver, and even
there: it already follows rule 1 (its `PIN_LOW`/`PIN_HIGH` macros compile
to direct `PORTx` bit ops on this target), and its rule-2 exceptions
(`delay()` in the reset/init sequence) are confined to one-time startup
before the scheduler is running -- see that module's README.

## Module map

```
                         ISR(TIMER2_COMPA_vect)  ~1.008ms tick
                                    |
        +---------------+----------+----------+------------------+
        | every tick     every 25th tick        every 10th tick   |
        v                v                      (unused slot)     |
  BinaryInputs      MultiOutput.slow_handler                      |
  .fast_handler()     -> Relay.ultra_slow_handler()               |
  MultiOutput        Comms.fast_handler()  (parses; see gap below)|
  .fast_handler()    RotaryEncoder.ms_handler()                   |
  -> MotorDC.fast_handler()                                       |
        |                                                         |
        v                                                         |
      bMap  ---------------------------------------------------> main()
                                                                 superloop
   +----------------------------------------------------------------+
   | main() while(1) superloop -- no fixed cadence                  |
   |   buildButtonMap(bMap) -> btnMap                                |
   |   RotaryEncoder.getDirection() x3 -> dir[]                      |
   |   rot0  -> janus_switch_screen   (PWM / SERIAL / Output tabs)   |
   |   rot1  -> janus_focus_move ;  rot1 click -> janus_focus_activate|
   |             -> janus_handle_action() [src/janus_actions.cpp]    |
   |                  -> MultiOutput (Relay)                         |
   |   ~100ms: refreshBusStatusInstance() <- MavlinkComms            |
   |   janus_render_screen / _widget -> draw_area_sync -> Display    |
   |   MCP4725 dac0/dac1.setVoltage(voltage0/1)  (fed by Comms /     |
   |                                    MAVLink, once the gap is fixed)|
   +------------------------------------------------------------------+
```

| Layer | Modules | README |
|---|---|---|
| Composition root | `main.cpp`, `Timer2Config` | [src/README.md](src/README.md) |
| Inputs | `BinaryInput`, `RotaryEncoder`, `ButtonMap`, `AnalogInput` | [lib/BinaryInput](lib/BinaryInput/README.md), [lib/RotaryEncoder](lib/RotaryEncoder/README.md) |
| Outputs | `BinaryOutputs`, `Relay`, `MotorDC`, `PWM` + config/timing/label-format types | [lib/BinaryOutputs](lib/BinaryOutputs/README.md), [lib/MultiOutput](lib/MultiOutput/README.md) |
| UI | `lib/GUI` -- Janus-generated screens + vendored `lib/GUI/runtime`; `src/janus_actions.cpp` + the Janus block of `main.cpp` are the glue | [The UI is generated (`lib/GUI`)](#the-ui-is-generated-libgui) |
| Comms/peripherals | `SerialCommunication`, `MavlinkComms`, `MCP4725` | [lib/Comms](lib/Comms/README.md), [lib/MCP4725](lib/MCP4725/README.md) |
| HAL / shared low-level | `Ports`, `HAL/RegisterIO`, `BusIO` (vendored) | [lib/Ports](lib/Ports/README.md), [lib/HAL](lib/HAL/README.md), [lib/BusIO](lib/BusIO/README.md) |
| Vendored, mostly untouched | `Display` (parallel-TFT, in active use), `lib/GUI/runtime` (Janus fixed runtime), `ArduinoLib`, `SD`, `TouchScreen` (not instantiated anywhere) | [lib/Display](lib/Display/README.md) |

`lib/StateMachine/src/PWMStateMachine.h` is not used by the firmware --
only `test_native/test_pwm_state_machine.cpp` includes it.

## The UI is generated (`lib/GUI`)

The on-screen TFT UI is produced by **Janus**, a code generator in a
sibling repo (`../../janus`), from declarative input files. `main.cpp`
supplies the hardware glue.

### Three kinds of file under `lib/GUI/`, three owners

| Kind | Files | Who edits it |
|---|---|---|
| **Authoring input** | `app.yaml`, `*.screen.yaml`, `janus_generated.harpia` (message shapes) | Hand-edited here. This is the contract handed to a Janus re-run. |
| **Generated** | `include/*_screen.gen.h`, `src/*_screen.gen.c`, `src/janus_app.gen.c`, `src/janus_bindings.gen.c`, `include/janus_bindings.gen.h`, `include/janus_actions.gen.h`, `include/janus_display_config.gen.h` | Overwritten wholesale by a Janus re-run -- **never hand-edit**. |
| **Vendored runtime** (`lib/GUI/runtime/`) | `janus_runtime.c`/`.h`, `janus_font.c`/`.h`, `janus_input_focus.c`, `janus_input_{touch,encoder,buttons}.h`, `janus_progmem.h`, ... | Hand-written once, shipped identical with every Janus project. Fix bugs **upstream in Janus** and re-vendor -- `../handle-to-janus.md` (repo root) is the running list of fixes to port back. Its own `lib_deps` entry in `platformio.ini` because it's nested where PlatformIO's LDF won't find it. |
| **Generated scaffold, NOT compiled** | `lib/GUI/janus_actions.c`, `lib/GUI/main.c` | All-TODO stubs Janus emits for a standalone build. Outside `lib/GUI/src/`, so the LDF skips them. The real versions are the glue below. |

### The glue (this repo's code, not Janus's)

- **`src/janus_actions.cpp`** -- the real `janus_handle_action()`. Maps
  each generated action id to a hardware effect. Today: only
  `JANUS_ACTION_TOGGLE_RELAY_0..7` -> `multiOuput.getRelays()->setRelay()`,
  with on/off state mirrored in `main.cpp`'s `relayState[]` (the `Relay`
  class has no getter). The PWM tab's toggles have generated action ids
  but no hardware effect wired yet.
- **The Janus block in `src/main.cpp`** -- four jobs:
  1. **Driver contract** (`display_driver_init`, `draw_area_sync` /
     `draw_area_async`, `display_busy`): bridges Janus's RGB565 tile
     writes onto the `Display` parallel-TFT driver's `setAddrWindow` +
     `pushColors`. No DMA, so `draw_area_async` writes synchronously and
     returns true, and `display_busy()` is always false.
  2. **Input wiring**: reads the 3 rotary encoders + button map once per
     superloop pass, then routes -- **rot0** = `janus_switch_screen`
     (cycle PWM / SERIAL / Output); **rot1** = `janus_focus_move(screen, ±1)`;
     **rot1 click** = `janus_focus_activate`, whose result is dispatched
     to `janus_handle_action` / `janus_switch_screen` / `janus_toggle_box`.
     Janus's `nav: tabs` in `app.yaml` is metadata only (tab-bar
     titles), *not* a wired input path -- the encoder-drives-tabs
     behavior is authored directly in `main.cpp`.
  3. **Feeding the bound structs**: the generated `*_instance` structs
     (`pwm_instance`, `bus_status_instance`, `relay_instance`) are what
     widgets read. Firmware writes real values in --
     `refreshBusStatusInstance()` copies MAVLink-received CAN/RS-485
     config into `bus_status_instance` every ~100ms -- and sets the
     matching `*_dirty` flag so the dirty-aware render path repaints only
     what changed.
  4. **Redraw cadence**: `janus_render_screen` on entry / tab-switch and
     right after an action fires; `janus_render_widget` on the ~100ms
     telemetry tick to refresh just the status bar (widget 0, by
     convention on every screen). The full-screen background clear on
     tab-switch (`tft.fillScreen(0)`) is firmware's job -- the runtime
     only ever fills widget rects, never clears first, and has no notion
     of a canvas background color.

### Render pipeline

`docs/gui-render-pipeline.drawio` diagrams the path (input ->
focus/action resolution -> `janus_render_*` -> `draw_area_sync` ->
`Display`). Rendering is **blocking** (`display.render_mode` defaults to
blocking; a full `pwm` screen render measures ~41-42ms, inside the
~100ms loop cadence). The runtime's non-blocking polled path is compiled
out -- its 6912-byte `g_async_ops` buffer would overflow the
ATmega2560's 8 KiB SRAM.

### Where a UI bug lives

| Symptom | Owner |
|---|---|
| Stale pixels after a tab switch | Firmware (`tft.fillScreen` clear) |
| Widget at wrong geometry / runaway `fill_rect` / garbage after screen load | Janus (bad baked rect, or a PROGMEM-read bug) |
| Toggle flips on screen but hardware doesn't react | Firmware (`janus_handle_action`) |
| Hardware reacts but UI doesn't update | Firmware (missing render call / unset dirty bit) or Janus (dirty traversal) |
| Focus skips a widget / wrong traversal order | Janus (`focus_order` baking) |
| Encoder or button does nothing at all | Firmware input wiring in `main.cpp` |
| Layout / widget-tree / new data binding change | Edit `*.screen.yaml` + `.harpia`, hand off for a Janus re-run |
| Swap enabled/disabled icon from a bound bit | Janus feature gap -- needs a `hidden`/state flag in the generator (`pwm.screen.yaml`'s header notes this); firmware only sets the bit |

### Known follow-ups specific to the UI

- `bus_status_instance`'s CAN/RS-485 fields only repaint on tab-switch or
  box-toggle; the ~100ms tick redraws only the status bar. Live passive
  refresh there needs its own trigger -- e.g. an action fired from the
  MAVLink receive path. See the comment in `main.cpp`'s ~100ms block.
- The PWM tab's `*_disabled.jpg` icon art sits in `lib/GUI` unused,
  waiting on the Janus `hidden`/state-flag feature above.

## Shared hardware resource allocation

### The multiplexed output bus (`Relay` / `MotorDC`)

`Relay` and `MotorDC` share one 8-bit data bus feeding separate `74LS373`
transparent latches, one per device, each captured by its own strobe
line. Each device keeps its own current 8-bit state in RAM and rewrites
the whole byte on every change (the bus is shared and byte-wide, not
individually addressable per bit).

| Signal | AVR pin | Role |
|---|---|---|
| Data bus (8 bits) | `PC2,PC1,PC0,PD7,PG2,PG1,PG0,PL7` | Shared -- holds the byte about to be latched |
| `dig_0` | `PH6` | Strobe -- a third latch, physically present on the board, driven by no firmware |
| `dig_1` | `PG5` | Strobe -- `Relay`'s latch |
| `dig_2` | `PF4` | Strobe -- `MotorDC`'s latch |
| `OUTPUT_EN` | `PB4` | Shared tri-state control, not part of the write sequence |

A `74LS373` is *transparent*, not edge-triggered: its outputs follow the
inputs while its enable line is high, and hold whatever value was present
the instant that line falls. So a correct write is: **settle the data
bus -> raise the target device's strobe -> drop it again** -- the
*falling* edge of the strobe captures the byte into that device's latch,
leaving the other latches untouched.

`MultiplexedBus` (`lib/MultiOutput/src/MultiplexedBus.h`) is the driver,
built on top of `BinaryOutputs::SetOutput()`. `write(strobeIndex, byte)`
settles all 8 data-bus bits, raises the target strobe, then drops it, all
inside `ATOMIC_BLOCK(ATOMIC_RESTORESTATE)` -- so a `Relay` foreground
write and a `MotorDC` ISR-context write (every ~1ms tick) can't
interleave and tear a byte. `enableOutputs()` drives `OUTPUT_EN` low once
at setup (`74LS373`'s `OE` is active-low, per the KiCad schematic).
`Relay` assembles all 8 relays into one byte via `refreshBus()` and calls
`bus.write(MUX_RELAY_STROBE, value)`; `MotorDC` uses `MUX_MOTOR_STROBE`
the same way, with coarse software-PWM speed control on the ~1.008ms tick
(~99Hz carrier, 10% duty steps) -- no dedicated fast hardware timer is
free (Timer1/3/4/5 are all committed to `PWM`).

**Open:** `MotorDC`'s bit positions within its one byte (`enA`=bit0,
`dirA`=bit1, `enB`=bit2, `dirB`=bit3) are a placeholder, unconfirmed
against `IOs IHM.xlsx` / the KiCad schematic. `MotorDC` also has no
on-screen UI tab yet.

### Hardware timers (PWM only -- decoupled from the bus above)

| Timer | Owner |
|---|---|
| Timer2 | System tick (`ISR(TIMER2_COMPA_vect)`) |
| Timer1 | PWM channel 2 (`OC1A/B/C`, complex) |
| Timer3 | PWM channel 0 (`OC3A`, simplex) |
| Timer4 | PWM channel 3 (`OC4A/B/C`, complex) |
| Timer5 | PWM channel 1 (`OC5A`, simplex) |

All 8 timer-compare pins (`OC1A/B/C`, `OC3A`, `OC4A/B/C`, `OC5A`) are
direct-to-output, no buffer, entirely separate from the `Relay` /
`MotorDC` bus.

## Known gaps

Pre-existing gaps, listed so they stay visible -- not implying any need
fixing today.

1. **`SerialCommunication::receive()` is never called.** Nothing forwards
   incoming UART0 bytes to it (the real RX ISR only fills the standard
   `Serial` ring buffer), so the framing/checksum parser it feeds never
   produces a complete frame and `voltage0`/`voltage1` never update from
   real serial input. Needs either a `USART0_RX_vect` override calling
   `comms.receive()`, or main-loop polling of `Serial.available()` /
   `Serial.read()` feeding it. See [lib/Comms/README.md](lib/Comms/README.md).
2. **`MCP4725` dac0/dac1 vs. voltage0/voltage1 naming is crossed** in
   `main.cpp` (`dac1.setVoltage(voltage0,...)`, `dac0.setVoltage(voltage1,...)`)
   -- may be intentional (matching board wiring), worth a deliberate
   check. The `dac*.begin()` / `setVoltage()` calls are commented out for
   now (a missing DAC hangs `twi.c`'s unbounded wait loop). See
   [lib/MCP4725/README.md](lib/MCP4725/README.md).
3. **`MavlinkComms::poll()` drains the whole RX ring buffer in one
   `while (serial->available())` loop** (`lib/MavlinkComms/src/MavlinkComms.h`)
   -- not bounded by a fixed iteration count, so a burst of buffered
   bytes can hold up the rest of the superloop until they're all parsed.

## What's already solid

`Relay` is fully wired UI-to-`MultiplexedBus`-to-hardware and is the
template for the output-wiring pattern -- software-verified only (no
physical relay click confirmed). `PWM`'s register math is natively
unit-tested and hardened against a real register-clobbering bug; its
timers are direct-to-output, unaffected by the bus work.
`BinaryInputs` / `Ports` / `RegisterIO` give the project a consistent,
tested register-access foundation every other module builds on.
