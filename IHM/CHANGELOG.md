# Changelog

## 2026-08-16 -- Make AnalogInputs::read() non-blocking (interrupt-driven ADC scan)

Third item this session (after the `MotorDC` rewrite and `ServoMotor`
deletion below). `AnalogInputs::read()` used to block the caller on a
`while (ADCSRA & (1<<ADSC));` polling loop -- ~104us/channel, up to
~520us total across all 5 channels every ~100ms when `MavlinkComms`
builds `IHM_BOARD_STATE`. Flagged in `ARCHITECTURE.md` as the same class
of problem as a `delay()` call (this project's rule 2: no blocking
waits, a timer tick drives scheduling instead), just polling a hardware
flag instead of counting cycles.

Rewritten to a continuous, self-sustaining round-robin scan:
`AnalogInputs::setup()` enables `ADIE` and starts channel 0's conversion;
`ISR(ADC_vect)` (new, in `main.cpp`) calls `AnalogInputs::isr_handler()`,
which caches the completed conversion into a `volatile results[]` array,
advances to the next channel, and immediately restarts the ADC -- the
scan keeps itself going forever with no further external trigger needed.
`read(index)` now just returns the cached value: non-blocking, and as a
side effect *fresher* than before (bounded by one ~1ms full-scan cycle
instead of the caller's own ~100ms read cadence).

Follows the project's established single-ISR-owner convention (`main.cpp`
is the only place that defines `ISR(...)` vectors; modules expose handler
methods called from there) -- same pattern `BinaryInputs`/`MultiOutput`/
`RotaryEncoder` already use with `TIMER2_COMPA_vect`.

RAM: 75.8% (6213/8192 B), up 11 bytes from the `ServoMotor` deletion
baseline (75.7%/6202 B) -- the new `results[]` cache. Native tests (96)
unaffected -- `AnalogInputs` isn't natively tested (AVR-only headers,
same reason as `Relay`/`MotorDC`).

## 2026-08-16 -- Delete ServoMotor: servo control is not part of the IHM solution

Same session as the `MotorDC` rewrite below. While scoping servo work as
the natural next item, found the problem is meaningfully harder than
`MotorDC`'s was: `ServoMotor::timer_handler()` wrote directly to
`OCR4A`, which belongs to `PWM` channel 3 (`Timer4`, complex PWM,
actively used) -- a live corruption risk, same "landmine, never
triggered because never called" pattern the old `MotorDC` had.
`engines[i].index = i+8` and `NUMBER_OF_ENGINES = 10` were also from the
pre-multiplexed-bus model (collided with `PWM`'s pins/the bus's control
lines; real hardware has 8 servo channels, not 10), and there was a real
clamp bug in `load(uint16_t, uint8_t)` (checked `ar <= MIN_POSITION`
where the array overload right above it correctly checks `ar[i] <=
MAX_POSITION`).

Beyond the bugs, the deeper issue: servos need real pulse-width
resolution (a ~1-2ms pulse repeated every ~20ms, with fine-grained
timing within that window), and -- same finding as `MotorDC` -- no
dedicated fast timer is free (Timer1/3/4/5 are fully committed to
`PWM`'s 4-channel generator). `MotorDC` could get away with riding the
existing ~1ms system tick because on/off duty cycling at ~100Hz only
needed ~10 steps of resolution; servos need far more than that tick can
give, and borrowing a PWM channel's spare compare unit would couple
servo timing to that channel's user-editable frequency.

**User's decision: kill servo support entirely rather than solve that
timing problem here.** Servo control is not part of the IHM solution --
a separate, dedicated servo controller is planned for the future, not a
rewrite of this driver.

**Removed:** `lib/MultiOutput/src/ServoMotor.h`/`.cpp`; `MultiOutput`'s
`ServoMotor` member, include, and `timer_handler()` method; the empty
`ISR(TIMER1_COMPA_vect)` in `main.cpp` (its only purpose was calling
`multiOutput.timer_handler()` -- already commented out -- and `TIMSK1`
was never configured, so that vector never actually fired regardless).

**Kept:** `MultiplexedBus.h`'s `MUX_SERVO_STROBE` constant and the
bus/latch hardware description in `ARCHITECTURE.md` -- the third
`74LS373` latch (`dig_0`) is a real, physically-wired hardware fact
regardless of firmware support; it's just undriven now. Documented as
such (with a pointer to this entry) in `ARCHITECTURE.md`,
`NEXT-SESSION.md`, `lib/MultiOutput/README.md`,
`lib/BinaryOutputs/README.md`, and `IHM/README.md`'s pitch paragraph.
`docs/architecture.drawio` still shows the old `ServoMotor` nodes and
was not regenerated -- treat servo mentions there as stale.

RAM: 75.7% (6202/8192 B), down from 78.7% after the `MotorDC` rewrite
below. Native tests (96, `test_native/run_tests.ps1`) unaffected --
`ServoMotor` was never natively tested (same AVR-only-header reason as
`Relay`/`MotorDC`).

## 2026-08-16 -- Rewrite MotorDC onto the multiplexed bus, wire its tick handler

`MotorDC` (`lib/MultiOutput/src/MotorDC.h`) predated the 2026-08-13
multiplexed-bus finding and still used raw `BinaryOutputs` indices
(`PWMA_INDEX 8`, `DIRA_INDEX 10`, `ENA_INDEX 11`, etc.) that collide with
`PWM`'s exclusive hardware-PWM pins and the bus's own strobe/`OUTPUT_EN`
lines -- never triggered only because nothing called into `MotorDC` from
`main.cpp`. Rewritten to hold a `MultiplexedBus` and go through
`bus.write(MUX_MOTOR_STROBE, ...)`, same pattern as `Relay`.

**DC speed control:** no dedicated fast timer is free for real hardware
PWM -- Timer1/3/4/5 are fully committed to the 4-channel `PWM`
generator's 8 compare-output pins, and borrowing one of Timer3/5's spare
compare units would couple motor carrier frequency to that PWM channel's
user-editable frequency. Decided instead to drive coarse software PWM off
the existing ~1.008ms system tick: 10 ticks/period (~99.2Hz, the floor of
the confirmed-acceptable 100Hz-1kHz range), duty quantized to 10% steps.
`MultiOutput::fast_handler() -> MotorDC::fast_handler()` is now actually
called, from `TIMER2_COMPA_vect`'s every-tick branch in `main.cpp` (the
call site existed but was commented out); the stepper commutation
sequence was also rewritten to go through the bus instead of raw
per-pin `SetOutput()`, which never latched correctly under the real
protocol.

Also deleted `MotorDC::setup()`'s `TCCR1A`/`TCCR1B` writes -- Timer1 is
exclusively owned by `PWM` channel 2; this was a live corruption risk
that only never fired because `motors.setup()` was never called.

**Bit layout within `MotorDC`'s one latched byte is a placeholder**
(bit0=`enA`, bit1=`dirA`, bit2=`enB`, bit3=`dirB`) -- not confirmed
against `IOs IHM.xlsx`/the KiCad schematic, both binary/graphical files
this session couldn't parse. Flagged in code and in `NEXT-SESSION.md`.

Scope deliberately excludes on-screen TFT UI wiring (`MotorScreen`/output
tab) and any hardware verification -- the connected board is a confirmed
mismatched hardware revision.

RAM: 78.7% (6450/8192 B), down slightly from 79.2% -- native tests (96,
`test_native/run_tests.ps1`) unaffected, `MotorDC` isn't natively tested
(same as `Relay`, both pull in AVR-only headers via `MultiplexedBus`/
`BinaryOutputs`).

## 2026-08-15 -- Simplify the on-board TFT UI; build a real SERIAL/bus-status screen

User called the on-board UI (`lib/GUI`, `lib/Elements`) "off-putting" in both
code and on-screen result, and asked for both simplified together, while
explicitly preserving partial/selective redraws and blink/counter-driven
animation -- the two things that make a slow parallel-TFT UI feel more
responsive than its actual superloop-cadence update rate.

**`Element` (`lib/Elements/src/Element.h`) de-virtualized, not just
fixed.** Every setter (`setLabel`/`setLocation`/`setPosition`/`setSize`/
`setState`/`update`) was `virtual` and returned `Element` *by value* -- a
slicing trap on a class meant to be subclassed, plus a pointless copy every
call. Grepped the whole tree first: nothing anywhere holds an `Element*`/
`Element&` or calls through one -- every call site uses its widget's own
concrete type directly, so polymorphism was never actually exercised.
Removed `virtual` entirely (no dispatch needed), changed returns to `void`,
and dropped the unused `elementType`/`ElementType` field (stored, never
read -- no getter existed). `setPosition`/`setSize` were called nowhere on
any class -- deleted from `Element` and every subclass rather than "fixed."
`setLocation` kept only where it does something real (`LED`, the optional
side-label); the no-op overrides on `Label`/`Icon`/`BattIcons`/
`TabSelector`/`StatusBar` are gone.

**Duplicated blink logic factored into `BlinkAnimator.h`** (new, small):
`Label`/`LED` each hand-rolled the same counter/threshold/toggle shape
across 5-6 near-identical `switch` cases (with copy-pasted comments -- two
different states both said "turned blinking very fast"). Both now share one
`tick(threshold)` helper. Fixed along the way: `Label`'s blink-speed
thresholds were mis-ordered (`BLINK_FAST` slower than `BLINK_SLOW`,
`BLINK_VERY_FAST` identical to `BLINK_SLOW`) -- not intentional timing, a
bug; now three genuinely distinct speeds, matching the ordering `LED`
already had right. `LED`'s solid on/off states redraw every single call
(no dirty-flag guard, unlike `Label`) -- now gated the same way.
Preserved exactly: `TabSelector`'s default-selected tab still starts on
`LABEL_BLINK_BACKGROUND` (was numeric `5`) before settling once encoder
input happens -- caught and fixed an accidental substitution to
`LABEL_HIGHLIGHTED` while converting that call site to a named constant,
which would have silently dropped this startup animation.

**Named constants replacing magic numbers**: `ButtonMap.h` gained
`BTN_MASK_ROT0`/`BTN_MASK_ROT1`/etc. (matching its own pre-existing
documentation comment) -- `GUI::update()` no longer spells these as raw
`0b...` literals. `TabSelector`'s tab-width constants
(`TABSELECTOR_PWM_WIDTH`/`_SERIAL_WIDTH`/`_OUTPUT_WIDTH`) now compose into
`_SERIAL_OFFSET`/`_OUTPUT_OFFSET`/`_END_OFFSET` once, instead of being
re-summed by hand at every label-position and divider-line call site --
same pixel values, just one source of truth. Also fixed a real bug while
there: `TabSelector::selectCurrTab()`'s `OUPTUT_SELECTED` case was missing
a `break` (harmless today only because `default` is empty).

**Confirmed-dead code deleted** (grepped: zero live call sites; one of the
stale comments referenced a `screenMachine()` method that doesn't even
exist): `GUI`'s `ScreenType`/`currentWork` (a second, disconnected
"current screen" concept `TabSelector`'s own `SelectedOption` already
replaced) and the six unwired `rotateGUI`/`receiveData`/`showWarning`/
`showEmergency`/`showNormalOperation`/`showRegularLoop` methods.
`BattIcons` was duplicating `Icon`'s entire array-loop-drawRGBBitmap logic
independently instead of subclassing it (unlike `EdgeSelectionIcons`,
which already did this correctly) -- now subclasses `Icon` like it should.

**The SERIAL tab is now real**, not six hardcoded `drawFastHLine` calls
forming a meaningless staircase. New `BusStatusScreen`/`BusStatusRow`
(`lib/Elements/src/`), modeled on `RelayScreen`'s row-list shape (not
`PWMScreen`'s two-level edit cursor -- nothing here is editable from the
board, the PC app (`IHM-PCApp`, built earlier this session) is the intended
editor for `CAN_SIGNAL_CONFIG`/`RS485_SIGNAL_CONFIG` now that it exists).
Three read-only rows -- CAN0, CAN1, RS-485 -- showing what
`MavlinkComms::getCanSignalConfig()`/`getRs485SignalConfig()` has received
and stored, previously displayed nowhere at all. Each row keeps its own
last-seen snapshot and diffs it every tick to pulse its LED briefly on
change (`BlinkAnimator` again) before settling to solid on/off -- entirely
UI-side, `MavlinkComms` itself untouched. Wired in via `GUI` gaining a
`MavlinkComms*` (`main.cpp`'s `mavlinkComms` declaration had to move above
`gui`'s so the name exists when `GUI`'s constructor references it --
construction order itself doesn't matter, `GUI` just stores the pointer).

**Verified:** native test suite still 96/96. Clean AVR build, no new
compiler warnings (one `-Wreorder` self-introduced and fixed before
landing -- `BusStatusRow`'s constructor parameter `h` shadows the member of
the same name, same pattern `RelayElement` already relies on, just needed
the member set in the constructor body rather than the init-list to avoid
the warning). RAM **80.6% (6604/8192 B)**, up from 79.4% -- the new
`BusStatusScreen`'s widgets cost more than the deleted dead code saved.
Flash **21.8% (55452/253952 B)**, down from 22.3%. Flashed to the connected
board (COM7, confirmed old hardware revision -- see
`arduinoihm_hardware_revision_mismatch` note, asked before reflashing):
firmware boots and keeps producing `IHM_BOARD_STATE` telemetry at the
normal ~100ms cadence both before and after receiving a live
`CAN_SIGNAL_CONFIG` sent from a throwaway probe, confirming the new code
doesn't destabilize the board. **Not verified: the SERIAL tab's actual
on-screen rendering** -- that needs turning the physical tab-select
encoder, which requires the user's own hands/eyes, not something checkable
remotely.

**Found, not fixed** (flagged so they aren't lost, matching this project's
own "known gaps" convention):
- `LED::setLabel()` never re-arms `visibilityControl` back to `VISIBLE`
  after the first draw, so `StatusBar::setTimeSlice()`'s repeated
  `tSlice.setLabel(...)` calls update the buffer but the on-screen "TS:NN"
  text never visibly refreshes after the very first render -- pre-existing,
  not introduced by this session. A real fix needs the redraw path to clear
  the old text before printing new text of possibly-different width (`LED`
  doesn't do this today, unlike `Label`'s one-shot states), so wasn't
  attempted without being able to visually verify the result.
- `TabSelector`'s outer border has two smaller pre-existing oddities not
  touched: the right vertical line uses the absolute constant
  `TABSELECTOR_WIDTH` (318) instead of `x + TABSELECTOR_WIDTH`, ignoring
  `x0` (currently 2, so the border likely lands 2px short of the true right
  edge); the right border's height (`TABSELECTOR_HEIGHT - y`) doesn't match
  the left border's (`TABSELECTOR_HEIGHT`), a ~21px mismatch. Left
  untouched deliberately -- fixing pixel geometry without being able to see
  the physical screen risks trading a known-harmless oddity for an
  unverified new one.

## 2026-08-13 -- Delete confirmed-dead code (StateMachine, SPITFT/GrayOLED, Stream's timeout methods)

Second pass of the same session, clearing out dead code found while
writing the multiplexed-bus driver (see the entry below) and while
investigating two things the user spotted mid-session: a `while(millis())`
loop in `Stream.cpp` and a `while(Serial.available())` loop in
`MavlinkComms.h`. The latter is a real, live unbounded loop -- pinned as
a known follow-up, not touched. The former led to confirming several
whole files are dead weight, which *were* safe to just delete:

- `lib/StateMachine/StateMachine.hpp`/`.cpp` -- didn't compile
  (referenced an undefined type), nothing included it.
  `PWMStateMachine.h` (same folder, actually used) is untouched.
- `lib/Display/SPITFT.cpp`/`.h`, `SPITFT_Macros.h`, `GrayOLED.cpp`/`.h`
  -- vendored, confirmed nothing else in the tree included them. The
  active display path (`Display`/`GFX`/`mcufriend_shield.h`) doesn't
  touch these.
- `Stream`'s `millis()`-based timeout/parsing methods (`timedRead`,
  `timedPeek`, `readBytes`, `readBytesUntil`, `readString`,
  `readStringUntil`, `parseInt`, `parseFloat`, `find`, `findUntil`,
  `findMulti`, `peekNextDigit`, plus `setTimeout`/`getTimeout` and the
  `_timeout`/`_startMillis` members they existed for) -- grepped the
  whole tree outside `ArduinoLib`: zero callers. `Stream.h` now only
  declares the pure-virtual `available()`/`read()`/`peek()` interface;
  `Stream.cpp` is deleted entirely, nothing was left to implement.
  Checked first that no subclass (`HardwareSerial`, `Client`, `Udp`,
  `USBAPI`, `Wire`, `SD`) relied on the removed methods -- they only ever
  inherited the base interface.
- `main.cpp`'s dead `#include "SD.h"` -- removed. `lib/SD` itself
  (`Sd2Card.cpp`'s own `millis()` wait loops included) was **not**
  deleted, unlike the others above -- `mavlink/README.md` already
  reserves payload headroom for a future "SD-card-status" MAVLink
  message, so this one looked plausibly wanted later, not dead weight
  from an abandoned import. Revisit for real deletion if that message
  never materializes.

Left alone, on purpose -- these need a design decision or hardware
verification, not a deletion, so they stay as pinned follow-ups:
`MavlinkComms::poll()`'s unbounded RX-drain loop, `SerialCommunication::
receive()` never being called, and the `MCP4725` dac/voltage naming
question (might be intentional, matches board wiring).

**96 native tests still pass.** AVR build: RAM **79.2% (6491/8192 B)**,
down slightly from 79.4%; Flash 22.3% (56708/253952 B), also down
slightly. Both moved less than the deleted line count might suggest --
`-ffunction-sections`/`-fdata-sections` plus linker GC were already
dropping these unused symbols from the final binary before deletion. The
real benefit here is source-tree clarity, not footprint.

## 2026-08-13 -- Write the multiplexed-output-bus driver; fix Relay's write sequence

`NEXT-SESSION.md` item 0, the blocker for `ServoMotor`/`MotorDC` UI wiring
(items 1/2) and for `Relay`'s existing wiring actually being correct. New
`MultiplexedBus` (`lib/MultiOutput/src/MultiplexedBus.h`) implements the
settle-bus -> raise-strobe -> drop-strobe protocol the three `74LS373`
latches need (see `ARCHITECTURE.md`'s "multiplexed output bus" section for
the full hardware writeup): `write(strobeIndex, byte)` settles all 8
data-bus bits via `BinaryOutputs::SetOutput()`, then strobes the target
device's latch, wrapped in `ATOMIC_BLOCK(ATOMIC_RESTORESTATE)` so a
`ServoMotor` ISR-context write can't interleave with a `Relay`/`MotorDC`
foreground write and tear a byte mid-sequence -- resolves the atomicity
open question from the 2026-08-12 doc pass. `enableOutputs()` drives
`OUTPUT_EN` low once at setup; confirmed against the KiCad schematic that
`74LS373`'s `OE` pin is the part's only electrically-inverted pin on the
symbol (active-low), so low is what actually enables the latches --
easy to get backwards since `false`/low reads less intuitively as "on."
Holds a `const BinaryOutputs&`, not a copy -- the table is 160 bytes and
every device already keeps its own copy; a fourth copy here would have
cost another 160 bytes of already-scarce RAM for nothing.

`Relay.h` rewired to use it: `refreshBus()` (new, private) assembles all
8 relays' current `enabled && state` into one byte and calls
`bus.write(MUX_RELAY_STROBE, value)`; `setRelay`/`disableRelay`/
`ultra_slow_handler` all call it instead of the old one-`SetOutput()`-
per-relay immediate-write model. `Relay`'s public API (`setRelay`,
`enableRelay`, `disableRelay`, `ultra_slow_handler`) is unchanged, so
`RelayElement`/`RelayScreen`/`GUI` needed no changes. `MultiOutput.h`
gained a `MultiplexedBus bus` member (constructed from `bnOuts`, ahead of
`relays` in declaration order) and calls `bus.enableOutputs()` alongside
`bnOuts.setup()`.

`ServoMotor`/`MotorDC` are untouched -- still take a raw `BinaryOutputs`,
still not called from `main.cpp`'s live handlers. Item 0 only unblocks
items 1/2; it doesn't do them. `MotorDC`'s exact bit layout within its one
byte is still an open question for whoever picks that up next.

**96 native tests, all still passing** (this change isn't reachable from
any native-testable file -- `MultiplexedBus`/`Relay` both depend on
`BinaryOutputs` -> `avr/io.h`, same AVR-only category as `BinaryOutputs`
itself). AVR build (`platformio run`, `megaatmega2560` env): RAM
**79.4% (6507/8192 B)**, actually *down* from the pre-existing 81.3%
baseline (the MAVLink/AnalogInput commit two days prior) despite adding a
whole new class -- the single assembled-byte `write()` call compiles
smaller than the old per-relay immediate-write loop did. Flash 22.3%
(56748/253952 B), essentially unchanged.

No physical hardware was available this session to verify an actual
relay click or confirm the `OE` polarity read from the schematic --
same caveat as the 2026-08-11 Relay-wiring entry.

## 2026-08-12 -- MAVLink telemetry/command protocol + analog-input driver (catch-up entry)

Not recorded here at the time (commit `cc334ea` landed same-day as the
architecture-documentation pass below, but this changelog and
`NEXT-SESSION.md` weren't updated for it until now). New
`IHM_BOARD_STATE`/`CAN_SIGNAL_CONFIG`/`RS485_SIGNAL_CONFIG` MAVLink
dialect (`IHM/mavlink/`), wired into `main.cpp`'s superloop: the board
pushes user-input + analog telemetry roughly every 100ms and parses
incoming CAN/RS-485 signal-generator commands (stored only -- no bus
driver exists yet to act on them). Replaced the old raw `Serial.print()`
debug lines, which would otherwise corrupt the binary MAVLink stream on
the same UART.

Also new: a register-direct ADC driver (`lib/AnalogInput`) for the 4
analog inputs + `BattVoltage`, none of which were read anywhere in this
codebase before -- see `ARCHITECTURE.md` gap 9 for its accepted
polling-loop caveat. Also includes the `BinaryInput` `MCUCR` pull-up fix
from the same session (`ARCHITECTURE.md` gap 6).

One real RAM regression was caught and fixed before landing:
`mavlink_parse_char()`'s error path pulls in the library's internal
per-channel buffers regardless of which buffers you pass it, costing
~400 bytes for buffers this code never uses. Switched to
`mavlink_frame_char_buffer()`, the library's own "no global variables"
variant -- see `mavlink/README.md` for the measured before/after. Net RAM
cost for the whole feature: 519 bytes (75.0% -> 81.3%).

## 2026-08-11 -- Wire Relay outputs into the UI (Output tab)

First of the three items in "Relay, servo, DC/stepper-motor outputs... not
wired into the UI" (see 2026-08-10's Known follow-ups) -- scoped to Relay
only; servo/motor are separate future work, see `NEXT-SESSION.md`. Followed
the same pattern already proven for PWM: a pure/testable config class, thin
Display-facing glue, a screen container, then GUI wiring.

Investigated first and confirmed pin safety: `BinaryOutputs`'s hardcoded
20-slot pin table (`lib/BinaryOutputs/src/BinaryOutputs.h`) has `Relay` using
indices 0-7 exclusively; `MotorDC` (8-13) and `ServoMotor` (8-17) overlap
*each other* (pre-existing, untouched, out of scope) but neither touches
Relay's range, so wiring Relay in doesn't risk double-driving any pin.

- `lib/MultiOutput/src/RelayConfig.h` -- new, pure on/off struct (mirrors
  `PWMChannelConfig`'s shape, trivial: one bool + `toggle()`). Natively
  tested (`test_native/test_relay_config.cpp`, 2 tests).
- `lib/Elements/src/RelayElement.h` / `RelayScreen.h` -- new, Display-facing
  glue: one row per relay (an `LED` on/off indicator + a `Label` reusing
  `PWMLabelFormat::formatShortStatusLabel`'s "Ativo"/"Desl." text -- that
  function is generic on/off text, not PWM-specific, so no duplicate
  formatter was needed). `RelayScreen` mirrors `PWMScreen` but needs no
  `PWMStateMachine`-style field-cursor, since a relay is only ever on/off.
  Verified by AVR build + inspection only, same accepted gap as PWM's glue
  (see the killed Display-glue-testing follow-up above).
- `lib/MultiOutput/src/MultiOutput.h` -- added `getRelays()` accessor and
  made `setup()` enable all 8 relays (still start off). Deliberately *not*
  a second standalone `Relay` instance the way PWM has two disconnected
  `PWM` objects (`main.cpp`'s standalone one wired to `GUI`, plus an unused
  one inside `MultiOutput`) -- `MultiOutput::slow_handler()` already calls
  `relays.ultra_slow_handler()` every ~25ms to re-assert relay state, so a
  duplicate would mean the UI-controlled relays never get that refresh.
  `GUI` now takes the same `Relay*` `MultiOutput` owns.
- `lib/GUI/GUI.h`/`.cpp`, `src/main.cpp` -- Output tab wired: rot1 rotates
  through the 8 relays, rot1's button (same bit `PWM_SELECTED` uses for its
  own tab-local meaning, safe to reuse since it's scoped to this `case`)
  toggles the highlighted one -- applied to the real relay immediately, no
  separate confirm step, same UX as PWM fields.
- `Relay.h`/`Relay.cpp` themselves: **untouched**. `setRelay`/`enableRelay`
  were already public and exactly right.

**96 native tests total, all passing.** AVR build verified
(`platformio run`, `megaatmega2560` env) -- RAM jumped from 61.9% to **75.0%**
(5073 -> 6144 / 8192 B, +1071 B) for 8 new UI rows, each an `LED`+`Label`
pair; Flash barely moved (18.8% -> 19.5%). Flagged to the user as a bigger
jump than expected for "8 toggles" -- 25% RAM headroom remains, not
dangerous, but worth knowing before adding servo/motor screens on top.

No physical hardware was available this session to verify an actual relay
click -- verified by AVR build + native tests + manual trace only, same
caveat as prior AVR-build-only verifications.

## 2026-08-11 -- Fix unbounded `rcv_counter` in `SerialCommunication::receive()`

Follow-up from the 2026-08-10 pass below, which flagged this bug but left it
unfixed. `receive()` wrote every incoming byte to `ReceivedBytes[rcv_counter]`
after unconditionally incrementing `rcv_counter`, with no bound tied to the
array's size (256 bytes). If a frame's `HEADER`/`FOOTER`/`TERMINATOR` never
showed up in the stream (desync, noise, or a `Found` state with an
unusually long/malformed payload), `rcv_counter` grew past the array end,
corrupting adjacent class members (`ParseBuffer`, `OutBuffer`, etc.).

Fixed by checking `rcv_counter` against `sizeof(ReceivedBytes) - 1` *before*
each increment/write; on overflow, resets to `Searching` (drops the
buffered bytes and starts looking for a fresh `HEADER`) instead of writing
past the buffer. `lib/Comms/src/SerialCommunication.cpp`.

Not covered by a native unit test: `SerialCommunication.h` pulls in
`HardwareSerial.h`/`avr/io.h`/`avr/interrupt.h` directly, so it can't compile
for the host target without mocking the whole AVR serial stack -- the same
disproportionate-effort call made for the Display glue in the entry below.
Verified instead by AVR build (`platformio run`, unchanged RAM/Flash: 61.9%
/ 18.8%) and by tracing the state machine by hand for both the `Searching`
and `Found` overflow paths.

## 2026-08-10 -- PWM subsystem: state machine, register bugs, UI wiring, input fixes

Follow-up to a full-codebase review (see conversation history / PR
descriptions for the original findings). Delivered as 5 stacked branches off
`dev` (each branches from the previous, sharing the native test harness
added in the first one -- review/merge in order), one independent branch,
and a final demo branch stacked on the tip. All branches build the real AVR
firmware (`platformio run`) and pass the full native test suite at every
step; see `IHM/test_native/` and `IHM/demo/`.

### Branch stack (merge in this order)

| # | Branch | Commit | Summary |
|---|--------|--------|---------|
| 1 | `fix/pwm-state-machine` | `5e00764` | Fixed `PWMStateMachine.h`'s broken `#include <events.h` (unterminated, file doesn't exist) and completed the class -- it was a stub with no transition logic. Added channel selection and field-cursor cycling for both simplex and complex channel UIs. Added `test_native/`, a host-native unit test harness (MSVC-driven; no gcc on this machine, PlatformIO's native platform needs one). **15 tests.** |
| 2 | `fix/pwm-com-bits` | `8f62ecd` | `PWM.cpp` (1109 lines, hand-duplicated across 4 channels x 14 frequencies) had the missing-`COMnX1`-bit bug in every 9/10-bit mode, a `=`-instead-of-`\|=` clobbering bug in both 3-output channels, a stray extra bit copy-pasted into two cases, a clock-select macro typo, and -- found while fixing the rest -- channel 1 configured the wrong compare register entirely (`COM5C*`/writes to `OCR5A`) and could never have produced output at all. Rewrote the register math as a small, pure, fully-tested core (`PWMTiming.h/.cpp`, `PWMConfig.cpp`) behind a zero-overhead `Reg8`/`Reg16` register-handle HAL (`lib/HAL`), reducing `PWM.cpp` itself to four one-line AVR-bound wrappers. **31 tests.** |
| 3 | `feat/wire-pwm-screen-to-hw` | `3328d2a` | Nothing tracked "what is this PWM channel currently configured to do" anywhere in the firmware -- the UI showed static placeholder text and `PWM pwm;` was never called. Added `PWMChannelConfig`/`PWMComplexChannelConfig` (UI-editable state + pure conversion to the exact register-call arguments) and `PWMLabelFormat` (on-screen text, no float-printf). Wired `PWMSimplex`/`PWMComplex`/`PWMScreen`/`GUI.cpp` so every field edit applies live to the real timer -- no separate confirm step. Renamed `frequency_16_625HZ` to `frequency_15_625HZ` (its real output is 15625Hz) since the whole point of the new frequency label is to show the user the truth. All of this runs from the main loop via `GUI::update()`, same as before -- nothing added here runs from an ISR or blocks. **37 tests.** |
| 4 | `fix/encoder-bitmap-volatile` | `630ec87` | Rot1's encoder/button pins were swapped: `RotaryEncoder.h` read its own push-button pin (index 11) as a quadrature signal, while `main.cpp`'s `rot1btn` extraction read what should have been that signal pin (index 10) as the button. Fixed both, extracted the button-bitmap construction into a tested pure function (`ButtonMap.h/.cpp`), and marked `bMap`/`newDataAvailable`/`timeStatistics`/`timeCounter` `volatile` (written in `TIMER2_COMPA_vect`, read from `main()`). **10 tests.** |
| 5 | `fix/timer2-isr-cleanup` | `c2ec0c7` | `setup()` enabled all three Timer2 interrupts but only Compare-A had a handler; Compare-B/Overflow firing would vector to the reset vector, so two empty ISRs existed just to catch that. Root-caused it: only enable Compare-A, delete both empty ISRs. Also corrected two stale comments in the same block (prescaler is /128 not /256; tick period is ~1.008ms not 256us) found while verifying the fix. **2 tests.** |

**94 native tests total, all passing.** AVR build verified after every
branch (`platformio run`, `megaatmega2560` env) -- final state RAM 61.9%
(5073/8192 B), Flash 18.8% (47840/253952 B).

### Independent branch

| Branch | Commit | Summary |
|--------|--------|---------|
| `chore/restore-kicad-pcb-placement` | `43db406` | Restores `ElectricalProject/ArduinoIHM.kicad_pcb`'s 76-footprint placement, which was uncommitted work-in-progress from around 2026-01-17 that a `git reset --hard` (run while re-basing branch 2 onto branch 1, without first stashing *unrelated* uncommitted changes) accidentally discarded mid-session. Recovered from a KiCad backup zip still on disk. Branched from `dev` directly -- unrelated to the firmware stack above. |

### Demo

| Branch | Commit | Summary |
|--------|--------|---------|
| `demo/pwm-outputs-walkthrough` | `422b0bc` | `demo/demo_pwm_walkthrough.cpp` drives the real production code (the same functions `PWMSimplex`/`PWMComplex` call on-device) against fake registers and prints the resulting register bytes for every channel x frequency x edge combination -- readable proof the fixes above do what they claim (e.g. the 9-bit inverting case now produces `TCCRxA=0xC2`, not the old `0x42`). `demo/HARDWARE_RUNBOOK.md` is the same checks for real hardware, plus an explicit note on what's still out of scope: relay/servo/motor outputs and the serial protocol were never wired into the UI and remain that way -- untouched by any of the 5 branches above. |

### Known follow-ups (not done in this pass)

- Relay, servo, DC/stepper-motor outputs (`lib/MultiOutput/src/Relay.*`,
  `ServoMotor.*`, `MotorDC.*`) and the serial command protocol
  (`lib/Comms/src/SerialCommunication.*`) are still not wired into the UI.
- `SerialCommunication::receive()`'s unbounded `rcv_counter` growth (no
  header found within the buffer) is unfixed -- noted in the original
  review, out of scope for this pass. **Fixed 2026-08-11, see entry above.**
- The demo's `PWMSimplex`/`PWMComplex`/`GUI.cpp` glue is verified by the AVR
  build and by inspection, not by native unit tests -- it's Display-coupled
  and mocking the display stack was judged out of proportion to this fix
  (same reasoning applied to `PWM.cpp`'s thin AVR wrappers in branch 2).
  **Won't fix, decided 2026-08-11** -- the plan is to switch to an SPI
  display next hardware revision, which replaces this driver outright, so
  test investment in the current parallel-TFT glue would be thrown away.
  See `NEXT-SESSION.md` for the full reasoning.
