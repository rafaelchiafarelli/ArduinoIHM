# Next session

Picking up from 2026-08-23. Full detail on what was done is in
`IHM/CHANGELOG.md` -- this file is just the "where things stand / what's
next" handoff.

**2026-08-23 session:** first real hardware bring-up of the Janus-
generated UI (`lib/GUI`/`lib/Elements` replacement -- the migration itself
predates this session, done but never committed before now). Found and
fixed two AVR `PROGMEM` bugs in Janus's fixed runtime library (not the
generated `.gen.c` files) -- see `CHANGELOG.md`'s 2026-08-23 entry for the
full writeup, and `../handle-to-janus.md` (repo root, one level up from
`IHM/`) for the version meant to be ported back into the Janus project
itself. `pwm_screen` now renders correctly and was measured at ~41-42ms
per full blocking render, well inside the ~100ms redraw cadence. RAM/Flash
dropped sharply (43.9%/3595 B, 16.1%/40992 B) since Janus's generated data
is `PROGMEM`-resident by design, unlike the old hand-written UI it
replaces.

**Open, top priority next session:** `dac0.begin(0x62)`/`dac1.begin(0x63)`
(`MCP4725`) hangs forever in `twi.c`'s unbounded TWI wait loops when the
DAC doesn't ack -- both `.begin()` calls and the two `dac*.setVoltage()`
calls in `main.cpp` are commented out as a bypass, not a real fix. Needs
either confirming the DAC wiring/I2C address against real hardware, or a
timeout around the TWI waits so a missing/unresponsive DAC can't hang
`setup()` again. This is now the only thing standing between the current
build and full functionality (display works, DAC output doesn't).

**2026-08-16 session:** `MotorDC` rewritten onto `MultiplexedBus` (same
pattern as `Relay`), with coarse software-PWM speed control on the
existing ~1.008ms system tick (no dedicated fast timer was free -- see
`CHANGELOG.md`'s 2026-08-16 entry for the full reasoning). `MultiOutput::
fast_handler()` is now actually wired into `TIMER2_COMPA_vect`. **One
open item carried forward, not resolved this session:** the bit layout
within `MotorDC`'s one latched byte (`enA`=bit0, `dirA`=bit1, `enB`=bit2,
`dirB`=bit3) is a placeholder -- needs checking against `IOs IHM.xlsx` or
the KiCad schematic before this is trusted against real hardware. DC/
stepper motor UI wiring (the on-screen TFT tab) is still not started --
deliberately out of scope this session, backend driver only.

**Same session, second decision: `ServoMotor` deleted entirely.**
User's call: servo control is **not part of the IHM solution** -- a
separate, dedicated servo controller is planned for the future, not a
rewrite of this driver. `lib/MultiOutput/src/ServoMotor.h/.cpp` are
gone, along with `MultiOutput`'s `timer_handler()` method and the empty
`ISR(TIMER1_COMPA_vect)` in `main.cpp` that only existed to call it
(`TIMSK1` was never configured, so that vector never actually fired).
The bus's third latch (`dig_0`/`MUX_SERVO_STROBE`) is still documented
in `MultiplexedBus.h` as a real, physically-wired hardware fact, just
unused by any firmware here now. `ARCHITECTURE.md`, `MultiOutput/
README.md`, `BinaryOutputs/README.md`, and `IHM/README.md` are updated;
`docs/architecture.drawio` still shows the old `ServoMotor` nodes and
was not regenerated -- treat servo mentions there as stale. RAM dropped
to 75.7% (6202/8192 B) as a result.

**Same session, third item: `AnalogInputs::read()` made non-blocking.**
Was a `while (ADCSRA & (1<<ADSC));` polling loop, ~104-520us blocking the
superloop every ~100ms (`ARCHITECTURE.md`'s former gap 9). Now a
continuous interrupt-driven round-robin scan -- `AnalogInputs::setup()`
starts it, new `ISR(ADC_vect)` in `main.cpp` (calling
`AnalogInputs::isr_handler()`) keeps it running forever, `read()` just
returns the cached last value. See `CHANGELOG.md`'s 2026-08-16 entry.
RAM: 75.8% (6213/8192 B), +11 bytes for the new cache array.

**2026-08-15 session:** two things happened, in a separate sibling repo and
in this one.

1. A new **standalone sibling repo, `workspace/IHM-PCApp`**, was created --
   the PC-side counterpart to this firmware. Win32/DirectX11/ImGui/ImPlot
   app that speaks the board's MAVLink dialect over its debug/programming
   COM port (verified live against this repo's board, COM7): displays
   `IHM_BOARD_STATE` telemetry, and can send `CAN_SIGNAL_CONFIG`/
   `RS485_SIGNAL_CONFIG`. Also scaffolded, not yet generated: a
   [Harpia](../harpia)-based bridge that would re-publish board telemetry
   onto a separate data-exchange fabric over ZMQ -- blocked on nothing now
   (Docker is installed), just not done yet. See that repo's own
   `NEXT-SESSION.md` for its handoff detail; not duplicated here since it's
   a separate repo/history.
2. In this repo: the on-board TFT UI (`lib/GUI`, `lib/Elements`) was
   simplified -- `Element`'s broken-by-value virtual pattern removed
   entirely (de-virtualized, not just fixed -- nothing anywhere used
   polymorphism through it), duplicated blink-counter logic in `Label`/`LED`
   factored into a shared `BlinkAnimator`, magic-number button masks and
   tab-layout constants named/composed properly, several confirmed-dead
   methods/fields deleted. The previously-placeholder **SERIAL tab is now a
   real read-only monitor** (`BusStatusScreen`) of the CAN0/CAN1/RS-485
   config the board receives from the new PC app -- closes the loop between
   the two pieces of this session's work. Full detail, including two
   pre-existing bugs found but deliberately not touched (a frozen status-bar
   `LED` label, `TabSelector`'s outer-border pixel oddities), in
   `CHANGELOG.md`'s 2026-08-15 entry.
   - **Not verified: the SERIAL tab's actual on-screen rendering** --
     confirming it needs turning the physical tab-select encoder, which
     needs the user's own hands/eyes on the device, not checkable remotely.
     Do this first next session if the same board is still connected.
   - RAM is now **80.6% (6604/8192 B)**, up from 79.4% -- re-run
     `platformio run` for a current number rather than trusting this, per
     this doc's own repeated caution below.
3. **New, not started: `IHM/ui-rotation.md`** -- portrait/landscape UI
   support driven by the housekeeping rotation pin (bit 4 of `bMap`, PA0),
   switchable live while the board is running, via a non-blocking
   incremental redraw (no full-screen redraw in one shot -- see
   `ARCHITECTURE.md`'s no-`delay()`/no-blocking-work rule). Deliberately left
   for a fresh session to pick up the key open decision (where the
   redraw-scheduler logic lives) rather than settling it in the same
   conversation it was scoped in. `IHM/ui-rotation.md` has full context,
   open questions (notably: how a single sensor bit is meant to select among
   4 rotation angles), and a new supporting diagram,
   `docs/gui-render-pipeline.drawio` (current-state only, not a proposal).

**2026-08-13 session:** item 0 below (the multiplexed-output-bus driver)
is done -- `MultiplexedBus` exists and `Relay` uses it; see
`CHANGELOG.md`'s 2026-08-13 entries for full detail. Items 1/2 (`ServoMotor`/
`MotorDC` UI wiring) are no longer *blocked*, but neither is done -- see
"Immediate next steps" below.

**Hardware verification attempted and blocked -- don't retry against the
same board.** This session's firmware (with the new `MultiplexedBus`
driver) was built and uploaded via `platformio run -t upload` to a Mega
2560 clone on COM7 (CH340 USB-serial) -- upload succeeded, verified,
56708 bytes flashed. But the user confirmed **the physical hardware
connected is still an old revision** -- not the board this session's
multiplexed-bus/`74LS373`-latch model was derived from. So even though
the upload worked, this did *not* actually verify `Relay`'s new write
sequence (or anything else in `MultiplexedBus`) against real hardware --
the "unverified against a physical board" caveat throughout this doc,
`CHANGELOG.md`, and `ARCHITECTURE.md` still stands. **Don't attempt
hardware verification again against whatever's connected until the user
says the current/matching-revision board is in place** -- ask first
rather than assuming a connected board is the right one to test against.

Same session, second pass: cleared out the safe-to-delete dead code found
while writing the driver. Deleted `lib/StateMachine/StateMachine.hpp`/
`.cpp` (didn't compile, unreferenced), `lib/Display/SPITFT.cpp`/`.h`/
`SPITFT_Macros.h`/`GrayOLED.cpp`/`.h` (vendored, unreferenced), and
`Stream`'s dead `millis()`-based timeout/parsing methods (`readBytes`,
`parseInt`, `find`, etc. -- confirmed zero callers outside `ArduinoLib`
before deleting; `Stream.cpp` is gone entirely, nothing was left to
implement). Removed the dead `#include "SD.h"` from `main.cpp`, but kept
`lib/SD` itself vendored (including its own `millis()` wait loops) since
`mavlink/README.md` reserves room for a future SD-card message -- see
"Known follow-ups" below for why that one's different from the others.
**Deliberately not touched** (need a real decision, not a deletion, so
left for later): `MavlinkComms::poll()`'s unbounded RX-drain loop,
`SerialCommunication::receive()` never being called, the `MCP4725`
dac/voltage naming question, and `AnalogInputs::read()`'s blocking ADC
poll -- all four are still open below.

AVR RAM is now **79.2% (6491/8192 B)**, down from 81.3% at the start of
this session (mostly `Relay`'s new single-assembled-byte write compiling
smaller than its old per-relay write loop; the dead-code deletions barely
moved it, since the linker was already garbage-collecting their unused
symbols -- the value there was source-tree cleanliness, not RAM/flash).

Also: this file and `CHANGELOG.md` had silently fallen a day behind
actual `dev` history before this session started -- the 2026-08-12
`cc334ea` commit (MAVLink dialect + wiring, `AnalogInput` driver) was
real, shipped work but was never appended to either doc. `CHANGELOG.md`
now has a catch-up entry for it. Worth checking `git log` against these
docs periodically, not just trusting them, if a gap like this is
suspected again.

## Where things stand

Everything is merged into `dev` and pushed to `origin`. The 2026-08-10 PWM
stack (`fix/pwm-state-machine` through `demo/pwm-outputs-walkthrough`) was
fast-forwarded into `dev`; `chore/restore-kicad-pcb-placement` was merged
in with a merge commit (it had diverged independently). All 7 feature
branches were deleted, local and remote, after merging -- `dev` is the only
branch with any of this work on it now. The 2026-08-11
`SerialCommunication` fix (see "Known follow-ups" below) was committed
directly to `dev`, no feature branch.

No PRs were opened for any of this -- it was merged directly by fast-
forward/merge commit with the user's confirmation, since everything landed
without conflicts.

After item 1 (fixed) and scoping item 2 (killed, see below), the session
continued into item 3: Relay is now wired into the Output tab (see
`CHANGELOG.md`'s 2026-08-11 "Wire Relay outputs into the UI" entry). Servo
and motor outputs are still not wired -- same pattern would apply, but each
peripheral has different hardware behavior (Timer4 single-compare for
servos, Timer1 fast-PWM + a stepper mode for DC motors) so it's not a
copy-paste of the Relay work. Nothing is mid-flight; `dev` is clean and
everything above is pushed.

**Worth knowing before continuing:** RAM is now **79.2% (6491/8192 B,
~20.8%/1701 B headroom)** as of the 2026-08-13 session -- re-run
`platformio run` for a current number rather than trusting any figure in
this doc, it has drifted before (see the 2026-08-13 session note above).
Servo (8 channels, per the updated hardware model below, not 10) and
motor screens will eat into that headroom further -- check the RAM number
after each one, don't assume there's room for both.

**2026-08-12 session:** no code changes. Two things happened: (1) a full
documentation pass -- `IHM/ARCHITECTURE.md`, a `README.md` in every
`lib/*` module and `src/`, and `IHM/docs/architecture.drawio` -- covering
the whole codebase's module map, scheduling model, and known gaps. (2)
While investigating a suspected Timer4 register conflict for item 1 below,
cross-checking against the user's `IOs IHM.xlsx` and the KiCad schematic
uncovered that `Relay`/`ServoMotor`/`MotorDC` don't drive independent GPIO
pins at all -- they share an addressed 8-bit bus behind three `74LS373`
latches. This is a bigger, more foundational finding than the Timer4
question it started as; see item 0 below for the full protocol. **Also
worth checking:** since `BinaryOutputs::SetOutput()` doesn't implement
this bus protocol, `Relay`'s existing UI wiring (done 2026-08-11, believed
complete) may not actually drive real hardware correctly -- verify against
a real board before trusting it.

## Immediate next steps (pick up here)

0. ~~**Write the multiplexed-output-bus driver -- blocks items 1 and 2, do
   this first.**~~ -- **Done 2026-08-13.** `MultiplexedBus`
   (`lib/MultiOutput/src/MultiplexedBus.h`) implements the settle-bus ->
   raise-strobe -> drop-strobe protocol for `Relay`/`ServoMotor`/`MotorDC`'s
   shared 8-bit bus (see `ARCHITECTURE.md`'s "multiplexed output bus"
   section for the full hardware writeup: pin map, `74LS373` transparent-
   latch behavior, `OUTPUT_EN` polarity). `Relay` now uses it and is
   believed to actually drive real hardware correctly for the first time
   (still unverified against a physical board). The atomicity open
   question below is resolved -- `write()` brackets its whole sequence in
   `ATOMIC_BLOCK(ATOMIC_RESTORESTATE)`. `MotorDC`'s bit-layout question
   is **not** resolved -- still open, see item 2.

1. ~~**Servo motor UI wiring**~~ -- **withdrawn 2026-08-16, not just
   deferred.** Servo control is not part of the IHM solution; `ServoMotor`
   was deleted rather than wired up. A dedicated servo controller is
   planned as separate future work. See `CHANGELOG.md`'s 2026-08-16 entry.
2. ~~**DC/stepper motor driver wiring**~~ -- **driver done 2026-08-16**,
   see `CHANGELOG.md`'s 2026-08-16 entry: `MotorDC` now goes through
   `MultiplexedBus` (`MUX_MOTOR_STROBE`), `fast_handler()` is actually
   called from `TIMER2_COMPA_vect`, and DC speed control works via coarse
   software PWM (~99Hz, 10% duty steps) since no dedicated fast timer is
   free. **Still open:** `MotorDC`'s exact bit layout within its one byte
   is a **placeholder** (`enA`=bit0, `dirA`=bit1, `enB`=bit2, `dirB`=bit3)
   -- needs checking against `IOs IHM.xlsx`/the KiCad schematic before
   trusting it against real hardware. **Still not started:** the on-screen
   TFT UI tab for motor output (deliberately out of scope for the driver
   session).

Item 2's on-screen TFT UI tab (still not started) is a real effort of its
own -- don't assume it's quick just because the driver work is done.

(Display-glue native tests, formerly #2, was killed -- see follow-up
below.)

## Known follow-ups

- **`ARCHITECTURE.md` and its module map are stale re: the UI** (still
  describe `GUI`/`Elements`/`PWMScreen`/`RelayScreen`, deleted 2026-08-23
  in favor of Janus-generated code -- see `CHANGELOG.md`'s 2026-08-23
  entry). Not rewritten this session -- a full module-map/diagram pass is
  its own effort, deliberately not done alongside the hardware bring-up
  and bugfixing this session was already about.

From `CHANGELOG.md`'s "Known follow-ups" section:

- Relay and DC/stepper-motor outputs (`lib/MultiOutput/src/Relay.*`,
  `MotorDC.*`) are still not wired into the on-screen TFT UI. **Relay
  done 2026-08-11** (see `CHANGELOG.md`); **motor's backend driver done
  2026-08-16** (bus wiring + software-PWM speed control, see that date's
  `CHANGELOG.md` entry) **but its UI tab still not started.** (Servo was
  a third item here -- **removed from scope entirely 2026-08-16**, not
  just deferred; `ServoMotor` is deleted, see that date's `CHANGELOG.md`
  entry.)
  ~~**Pin-index gotcha found while investigating:** `BinaryOutputs`'s pin
  table is one hardcoded 20-slot array, indexed by position, shared across
  `Relay`/`MotorDC`/`ServoMotor`/`MultiOutput`, with `MotorDC` (8-13) and
  `ServoMotor` (8-17) overlapping.~~ -- **superseded 2026-08-12.** That
  analysis assumed `Relay`/`MotorDC`/`ServoMotor` each drive independent
  GPIO pins through `BinaryOutputs`. The real hardware is a shared 8-bit
  bus behind three `74LS373` latches (see item 0 above for the full
  protocol) -- there's no per-pin index overlap to resolve because none of
  these three devices own individual pins at all. The actual blocker is
  writing a driver for the bus/strobe protocol, not repartitioning an
  index range.
- ~~`SerialCommunication::receive()` has an unbounded `rcv_counter`~~ --
  **fixed 2026-08-11**, committed directly to `dev`. See `CHANGELOG.md`.
- ~~`PWMSimplex`/`PWMComplex`/`GUI.cpp`'s Display-facing glue is verified
  by the AVR build and by inspection only, not by native unit tests~~ --
  **killed 2026-08-11, won't do.** Looked into it first: `Element` (base
  class of `PWMSimplex`/`PWMComplex`) holds a concrete `Display*`, not an
  interface, and `Display`/`GFX.h` pull in
  `Arduino.h`/`Print.h`/`I2CDevice.h`/`SPIDevice.h` -> `avr/io.h`, same as
  `SerialCommunication.h` before it, with `Display.cpp`+`GFX.cpp` alone
  ~6,371 lines (Adafruit-GFX-style parallel-TFT driver). Testing it
  natively would need a full Arduino/Display compatibility shim, or a
  production-code interface refactor with real regression risk -- neither
  is "add a test file." Decision: not worth it, because the plan is to
  switch to an SPI display next hardware revision, at which point this
  driver gets replaced anyway and any test investment here would be
  thrown away. Revisit test coverage for the *new* display's glue code
  once that swap happens, not this one.
  Note: the pure logic `PWMSimplex`/`PWMComplex` delegate to
  (`PWMChannelConfig`, `PWMLabelFormat`, `PWMTiming`) already has native
  coverage from the 2026-08-10 stack -- it was only the
  `tft->drawRect(...)`/`tft->print(...)` call sites themselves that were
  ever untested, and that gap is now accepted, not fixed.

- ~~**Write the multiplexed-output-bus driver**~~ -- **done 2026-08-13**,
  see item 0 above and `CHANGELOG.md`'s 2026-08-13 entry. `Relay` uses it;
  `ServoMotor`/`MotorDC` still don't (items 1/2).
- **`CHANGELOG.md`/`NEXT-SESSION.md` had silently drifted a day behind
  `dev`** -- the 2026-08-12 MAVLink/AnalogInput commit (`cc334ea`) was
  real, shipped work never recorded in either doc until this session's
  catch-up entry. Caught by comparing `git log` against these docs
  directly, not by anything in the docs themselves -- worth doing that
  comparison again if a gap like this is suspected.

Found during the 2026-08-13 session (not asked for, not fixed, pinned so
they aren't lost):

- **`MavlinkComms::poll()` drains its whole RX ring buffer in one
  unbounded `while (serial->available())` loop**
  (`lib/MavlinkComms/src/MavlinkComms.h:72`) -- same class of problem as
  the `AnalogInputs::read()` polling gap below (blocking the superloop
  for a data-dependent amount of time), just not yet measured.
- ~~**Dead `millis()`-based blocking-wait code in two vendored
  libraries**~~ -- **`Stream` side deleted 2026-08-13.** `timedRead()`/
  `timedPeek()` and everything that only existed to support them
  (`readBytes`/`readBytesUntil`/`readString`/`readStringUntil`/
  `parseInt`/`parseFloat`/`find`/`findUntil`/`findMulti`/`peekNextDigit`,
  `setTimeout`/`getTimeout`) are gone from `Stream.h`; `Stream.cpp` was
  deleted entirely (nothing left to implement). Checked first that no
  subclass (`HardwareSerial`/`Client`/`Udp`/`USBAPI`/`Wire`/`SD`) relied
  on the removed methods -- they only ever inherited the trimmed
  `available()`/`read()`/`peek()` interface. **`SD` side: only the dead
  `#include "SD.h"` in `main.cpp` was removed** -- `lib/SD` itself
  (`Sd2Card.cpp`'s `millis()` loops included) stays vendored, not
  deleted, since `mavlink/README.md` already reserves payload headroom
  for a future "SD-card-status" message. Revisit deleting `lib/SD` for
  real if that message never materializes.

Found during the 2026-08-12 documentation pass (`IHM/ARCHITECTURE.md` has
full detail on each; not asked for, not fixed, listed so they aren't lost):

- **`SerialCommunication::receive()` is never called by anything.** It's
  the only entry point that feeds bytes into the framing/checksum state
  machine, but the real `ISR(USART0_RX_vect)`
  (`lib/ArduinoLib/src/HardwareSerial0.cpp`) only fills the standard
  Arduino `Serial` ring buffer -- it doesn't forward to `receive()`.
  Result: `able_to_parse` can never become true, `fast_handler()` always
  returns false, and `voltage0`/`voltage1` (fed to the two `MCP4725`
  DACs) never update from real serial input today. Needs either a
  `USART0_RX_vect` override calling `comms.receive()`, or main-loop
  polling of `Serial.available()`/`Serial.read()` feeding it.
- ~~**`lib/StateMachine/StateMachine.hpp`/`.cpp` doesn't compile and isn't
  used.**~~ -- **deleted 2026-08-13.** Referenced an undefined type,
  nothing included it; `PWMStateMachine.h` (same folder, actually used)
  is untouched.
- ~~**`BinaryInput`'s `MCUCR |= ~(1<<PUD);`** doesn't do what its comment
  ("ensure pull-ups aren't globally disabled") says~~ -- **already fixed**,
  turns out (found 2026-08-18 while looking for a quick task -- this list
  had gone stale). `BinaryInput.h` already reads `MCUCR &= ~(1<<PUD);`,
  fixed in `39bf816` ("Add MAVLink telemetry/command protocol and
  analog-input driver") without this doc being updated. `BinaryInput/
  README.md` had the same stale bug writeup, corrected alongside this.
- **`MCP4725` dac0/dac1 vs. voltage0/voltage1 naming looks crossed** in
  `main.cpp`: `dac1.setVoltage(voltage0,...)`, `dac0.setVoltage(voltage1,...)`.
  May be intentional (matching board wiring) -- worth a deliberate check
  against the actual hardware before assuming "channel 0 = voltage0."
- ~~**`lib/Display/SPITFT.cpp`/`GrayOLED.cpp`** are vendored but entirely
  unreferenced~~ -- **deleted 2026-08-13** (`SPITFT.cpp`/`.h`,
  `SPITFT_Macros.h`, `GrayOLED.cpp`/`.h`); confirmed nothing else included
  them. The active display path (`Display`/`GFX`/`mcufriend_shield.h`) is
  untouched.

Full architecture writeup, per-module `README.md`s, and a diagram now
exist: `IHM/ARCHITECTURE.md`, `IHM/lib/*/README.md`,
`IHM/docs/architecture.drawio`. Check those before re-deriving module
structure from scratch.

None of these were asked for beyond the `SerialCommunication` fix -- listed
here so they don't get mistaken for "already done" or lost track of.

## How to pick up dev work

- Native unit tests: `IHM/test_native/run_tests.ps1` (PowerShell; drives
  MSVC directly since no gcc is installed on this machine -- see that
  script's header comment). 96 tests, all passing as of the last commit.
- AVR build: `platformio run` from `IHM/` (or `-d` pointed at it). Verified
  after every commit; last known state RAM 79.2% (6491/8192 B), Flash 22.3%
  (56708/253952 B) -- RAM headroom is getting less comfortable than it
  looks (see the note above on servo/motor RAM cost). This number has
  drifted out of sync with what this doc says before (see the 2026-08-13
  session note at the top) -- re-run rather than trust it blindly.
- Demo: `IHM/demo/run_demo.ps1` -- prints the register-level walkthrough.
  `IHM/demo/HARDWARE_RUNBOOK.md` has the equivalent checks for real
  hardware.
- Adding a new native-testable file: it needs adding to the allowlists near
  the top of `run_tests.ps1` (`$libAllowlist` for include dirs,
  `$prodSourceAllowlist` for production `.cpp` files to link in) -- these
  are explicit allowlists, not globs, because blindly including everything
  under `lib/` pulls in AVR-only headers that don't compile natively (see
  the comment in `run_tests.ps1` for the specific `lib/ArduinoLib/src/new`
  collision that motivated this).

## Notes for whoever/whatever resumes this

- **2026-08-13: `dev`'s history was rewritten and force-pushed** to strip
  `ElectricalProject/ArduinoIHM-backups/*.zip` out of every commit (only
  the working-tree copy is gitignored now; the 18 already-committed zips
  are gone from history too). Every commit hash from `5308ab5` ("Electrical
  project update - fix page size") onward changed as a result -- if you're
  matching this doc or `CHANGELOG.md` against a commit hash from before
  2026-08-13 and it doesn't resolve, that's why. A full pre-rewrite backup
  bundle (`git bundle`, all refs) was taken first and is not part of the
  repo itself -- ask the user if a pre-rewrite commit is ever actually
  needed.
- GitHub push access needed a key added to the `rafaelchiafarelli` account
  mid-session; that's resolved now (push succeeded), no action needed
  unless it regresses.
- Early in this session, a `git reset --hard` run while re-basing a branch
  discarded unrelated uncommitted changes (a KiCad PCB edit and a
  `.vscode/extensions.json` edit) that predated the session. The PCB edit
  was recovered from a backup zip and is now `chore/restore-kicad-pcb-
  placement`; the `extensions.json` edit was not recoverable. Be careful
  with `git reset --hard` / `git checkout` across branches with uncommitted
  changes present -- stash *everything* first, not just the files you think
  are relevant.
