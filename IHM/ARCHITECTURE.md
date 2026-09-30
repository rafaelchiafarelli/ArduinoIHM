# ArduinoIHM Architecture

A top-down map of the project, for finding where to make a change. Most
modules also have their own `README.md` (linked below) with more detail;
this document is about how they fit together. See
`docs/architecture.drawio` for a diagram of the module map and
`docs/gui-render-pipeline.drawio` for the on-screen UI's render path.

Target: PlatformIO `megaatmega2560` (ATmega2560). See `IHM/README.md` for
the one-paragraph pitch (PWM function generator + relay I/O behind
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
   handles everything else with no fixed cadence. The tick re-enables
   interrupts as its first statement, so UART RX ISRs can always preempt
   it, and it must always finish inside its ~1 ms slot (there is no
   re-entry guard). **The MAVLink fast handler
   (`mavlinkComms.fast_handler()`) is always the first call after that
   `sei()`** (Rafael, 2026-09-27); new tick work goes after it. See
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
        | sei(); MavlinkComms.fast_handler()  <- always first     |
        | every tick     every 25th tick        every 10th tick   |
        v                v                      (unused slot)     |
  BinaryInputs      MultiOutput.slow_handler                      |
  .fast_handler()     -> Relay.ultra_slow_handler()               |
                     Comms.fast_handler()  (parses; see gap below)|
                     RotaryEncoder.ms_handler()                   |
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
   |   301/302 -> SerialConfig -> mirrorSerialToUi() (on change)     |
   |   janus_render_screen / _widget -> draw_area_sync -> Display    |
   |   315 IHM_DAC_COMMAND -> MCP4725 dac[0/1].setVoltage (on cmd)   |
   +------------------------------------------------------------------+
```

| Layer | Modules | README |
|---|---|---|
| Composition root | `main.cpp`, `Timer2Config` | [src/README.md](src/README.md) |
| Inputs | `BinaryInput`, `RotaryEncoder`, `ButtonMap`, `AnalogInput` | [lib/BinaryInput](lib/BinaryInput/README.md), [lib/RotaryEncoder](lib/RotaryEncoder/README.md) |
| Outputs | `BinaryOutputs`, `Relay`, `PWM` + config/timing/label-format types | [lib/BinaryOutputs](lib/BinaryOutputs/README.md), [lib/MultiOutput](lib/MultiOutput/README.md) |
| UI | `lib/GUI` -- Janus-generated screens + vendored `lib/GUI/runtime`; `src/janus_actions.cpp` + the Janus block of `main.cpp` are the glue | [The UI is generated (`lib/GUI`)](#the-ui-is-generated-libgui) |
| Settings | `BusConfig` -- `SerialConfig` (SERIAL tab settings), its EEPROM image and RE2 step/acceleration helpers | [lib/BusConfig](lib/BusConfig/README.md) |
| Comms/peripherals | `SerialCommunication`, `MavlinkComms`, `MCP4725` | [lib/Comms](lib/Comms/README.md), [lib/MCP4725](lib/MCP4725/README.md) |
| HAL / shared low-level | `Ports`, `HAL/RegisterIO`, `BusIO` (vendored) | [lib/Ports](lib/Ports/README.md), [lib/HAL](lib/HAL/README.md), [lib/BusIO](lib/BusIO/README.md) |
| Vendored, mostly untouched | `Display` (parallel-TFT, in active use), `lib/GUI/runtime` (Janus fixed runtime), `SD`, `TouchScreen` (not instantiated anywhere) | [lib/Display](lib/Display/README.md) |

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
  each generated action id to a hardware effect. Relays:
  `JANUS_ACTION_TOGGLE_RELAY_0..7` -> `relaySet()` ->
  `multiOuput.getRelays()->setRelay()`, with on/off state mirrored in
  `main.cpp`'s `relayState[]` (the `Relay` class has no getter).
  `relaySet()` is also the path for the PC's `IHM_RELAY_COMMAND`. The PWM tab's toggles have generated action ids
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
     `mirrorSerialToUi()` copies the live `SerialConfig` (`lib/BusConfig`)
     into `bus_status_instance` whenever it changes (a 301/302 or a SERIAL
     tab edit) -- and sets the matching `*_dirty` flag so the dirty-aware render path repaints only
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

### The multiplexed output bus (`Relay`)

One 8-bit data bus feeds three `74LS373` transparent latches (relay,
servo, motor), each captured by its own strobe line. Only `Relay` drives
one today: servo control was removed 2026-08-16 and DC/stepper motor
control on 2026-09-28 (`fixes/000013`, the motor was never connected).
The motor latch's outputs are free: `rs485_modbus` means to drive the
RS-485 DE/RE line from one of them. A device keeps its current 8-bit
state in RAM and rewrites the whole byte on every change (the bus is
shared and byte-wide, not individually addressable per bit).

| Signal | AVR pin | Role |
|---|---|---|
| Data bus (8 bits) | `PC2,PC1,PC0,PD7,PG2,PG1,PG0,PL7` | Shared -- holds the byte about to be latched |
| `dig_0` | `PH6` | Strobe -- a third latch, physically present on the board, driven by no firmware |
| `dig_1` | `PG5` | Strobe -- `Relay`'s latch |
| `dig_2` | `PF4` | Strobe -- the motor latch; no driver since `fixes/000013`, `MultiOutput::setup()` latches 0 once |
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
write and an ISR-context write (the deleted motor driver wrote every
~1ms tick; an RS-485 DE/RE line would write from the UART ISRs) can't
interleave and tear a byte. `enableOutputs()` drives `OUTPUT_EN` low once
at setup (`74LS373`'s `OE` is active-low, per the KiCad schematic).
`Relay` assembles all 8 relays into one byte via `refreshBus()` and calls
`bus.write(MUX_RELAY_STROBE, value)`.

### Hardware timers (PWM only -- decoupled from the bus above)

| Timer | Owner |
|---|---|
| Timer2 | System tick (`ISR(TIMER2_COMPA_vect)`) |
| Timer1 | PWM channel 2 (`OC1A/B/C`, complex) |
| Timer3 | PWM channel 0 (`OC3A`, simplex) |
| Timer4 | PWM channel 3 (`OC4A/B/C`, complex) |
| Timer5 | PWM channel 1 (`OC5A`, simplex) |

All 8 timer-compare pins (`OC1A/B/C`, `OC3A`, `OC4A/B/C`, `OC5A`) are
direct-to-output, no buffer, entirely separate from the multiplexed
bus.

### EEPROM

| Bytes | Owner |
|---|---|
| 0-255 | `SerialConfig` image (`lib/BusConfig/src/SerialConfigImage.h`: magic, version, payload length, payload, CRC-16). 83 B today; the rest is room for bus initiatives' appended settings. |
| 256-351 | Relay state at power-off (`lib/MultiOutput/src/RelayStore.h`, `fixes/000014`): 32-slot wear-levelled ring of `[mask, ~mask, seq]`, one slot written per relay change; boot restores the newest. |
| 352-4095 | Free. |

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
2. **`MCP4725` DACs are not bench-verified.** Since `dac_control` they
   are driven by `IHM_DAC_COMMAND` (315), channel i = `dac[i]` = header
   DACi (0x62 / 0x63), with a Wire timeout so a missing DAC can't hang
   `setup()`. The I2C addresses are assumed, not checked against the
   modules fitted. See [lib/MCP4725/README.md](lib/MCP4725/README.md).
3. **Serial2 MAVLink path is not bench-verified.** MAVLink moved to
   Serial2 with interrupt-driven RX/TX (`lib/Uart2`, fast handler in the
   Timer2 tick; see [mavlink/README.md](mavlink/README.md)). The
   RX-overrun margin at 250000 baud and the drop/error counters haven't
   been exercised on hardware, and the counters aren't reported anywhere
   yet.

## What's already solid

`Relay` is fully wired UI-to-`MultiplexedBus`-to-hardware and is the
template for the output-wiring pattern -- software-verified only (no
physical relay click confirmed). `PWM`'s register math is natively
unit-tested and hardened against a real register-clobbering bug; its
timers are direct-to-output, unaffected by the bus work.
`BinaryInputs` / `Ports` / `RegisterIO` give the project a consistent,
tested register-access foundation every other module builds on.
