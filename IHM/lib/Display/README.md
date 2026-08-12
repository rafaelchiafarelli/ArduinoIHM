# Display

Vendored, `mcufriend_kbv`-style parallel-TFT driver. `Display`
(`Display.h/.cpp`) extends `GFX` (`GFX.h/.cpp`) which does the actual
pixel/text drawing; the parallel data-bus and control-line bit-banging
lives in `mcufriend_shield.h`.

See the project's memory note / `IHM/NEXT-SESSION.md` for the bigger
picture: this driver is planned to be replaced by an SPI display on a
**far-future** hardware revision (confirmed 2026-08-12, not imminent) --
so it's still the active investment target, not a dead end.

## Dead code in this folder

`SPITFT.cpp/.h`, `SPITFT_Macros.h`, and `GrayOLED.cpp/.h` are vendored
Adafruit-GFX-family drivers (generic SPI-TFT base class and a grayscale-
OLED driver respectively) that **nothing in this codebase references**
(confirmed by grep -- only `GFX.h`/`Display.h` are ever included). They
came along with the vendor drop but aren't part of the actual display
path. Safe to ignore, or delete when convenient; don't spend refactor
effort on them.

## digitalWrite / delay status (investigated 2026-08-12)

`mcufriend_shield.h` is one large per-target-platform `#elif` chain. The
branch that actually compiles for this board (`__AVR_ATmega2560__` /
`__AVR_ATmega1280__`, lines 56-118) already defines `PIN_LOW`/`PIN_HIGH`
as direct `PORTx &=/|= (1<<bit)` operations -- the same "port + bitmask"
strategy as [BinaryOutputs](../BinaryOutputs/README.md), just expressed
as macros. The `digitalWrite`-based `PIN_LOW`/`PIN_HIGH` variant that a
plain grep for `digitalWrite` turns up (near the end of the file) lives
in an unrelated fallback `#elif` branch that never compiles for this
target. **There is no digitalWrite in the compiled parallel-TFT driver.**

`delay()` calls are real but confined to one-time display bring-up:

- `Display::reset()` -- the RESET-pin toggle sequence (`delay(50/100/100)`).
- `Display::begin()`'s replay of a PROGMEM controller-init command table
  (`init_table`/`init_table16` in `Display.cpp`, `delay(len)`/`delay(d)`,
  driver-model-specific).
- `Display::readReg()`/`read16bits()` -- rare, only exercised by
  `readID()` during `begin()`.

`GFX.cpp` -- which handles every `drawPixel`/`fillRect`/text call during
normal operation -- has **zero** delay calls. Since AVR `delay()` polls
`millis()` with interrupts enabled, Timer2's tick ISR (and everything it
dispatches: relay/input/servo/comms handlers) keeps running the whole
time regardless. Net effect: these delays add a few hundred ms to boot
time, once, and don't block any real-time behavior. Decision (2026-08-12):
leave as-is rather than build a non-blocking boot sequencer for it.
