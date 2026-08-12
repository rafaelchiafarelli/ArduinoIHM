# Elements

The widget library [GUI](../GUI/README.md) is built from. Everything here
runs synchronously from `GUI::update()`/`PWMScreen`/`RelayScreen` in the
foreground superloop -- no ISR/handler involvement, no hardware ownership
of its own beyond the injected `Display*`.

## Files

- **`Element.h`** -- base class for all widgets (position/size/state/
  label + `Display*`). `setLabel/setLocation/setPosition/setSize/
  setState/update()` all return `Element` *by value* -- a fluent-ish
  pattern, but note this means slicing, not real polymorphism, if a
  caller ever treats the return as a derived type. `Element.cpp` is
  empty; everything is header-inline.
- **`Label.h`** -- text widget, 7 blink states (off/on/blink slow/fast/
  very-fast/inverted-blink/highlighted).
- **`LED.h`** -- circular status indicator, blink states + optional
  side-label (top/bottom/left/right).
- **`Icon.h`** (`.cpp` empty) -- generic bitmap-icon element over an
  externally-owned `IconType_t[]` (bitmap ptr + w/h/x/y + visibility).
  `BattIcons.h` and `EdgeSelectionIcons.h` subclass it for specific icon
  sets, pulling raw bitmap data from `Icons/` (`BattCharging.h`,
  `BattEmpty.h`, `BattFull.h`, `EdgeFalling.h`, `EdgeRising.h`).
- **`TabSelector.h`** -- the 3-tab header (PWM/SERIAL/Output); pure
  cursor + draw.
- **`StatusBar.h`** -- top strip (serial-LED placeholder, time-slice
  indicator, battery icon). Only `setBattLevel`/`setTimeSlice` are fed
  live data (from `GUI`).
- **`PWMScreen.h`** -- the PWM tab: 2x `PWMSimplex` + 2x `PWMComplex` +
  a `PWMStateMachine` ([StateMachine](../StateMachine/README.md)) for
  channel/field cursor state. See [MultiOutput/README.md](../MultiOutput/README.md)
  for the channel -> timer mapping.
- **`PWMSimplex.h` / `PWMComplex.h`** -- delegate config math to
  `PWMChannelConfig`/`PWMLabelFormat` (`lib/MultiOutput`).
  `editField()` mutates the config, then *immediately* calls
  `applyToHardware()` (real register writes via `PWM::setupPWMChannel0..3`)
  and `refreshDisplay()` -- there's no separate confirm step; every
  encoder tick writes live to the timer.
- **`RelayScreen.h`** -- the Output tab: 8x `RelayElement` rows + a
  single selection index.
- **`RelayElement.h`** -- one relay row, owns a `RelayConfig`
  (`lib/MultiOutput`); `toggle()` applies immediately via
  `Relay::setRelay(index, state)`.
- **`Widgets.h`** -- umbrella include of LED/TabSelector/Label/Icon/
  StatusBar.
- **`standardDefinitions.h`** -- shared RGB565 color constants, plus
  touchscreen calibration pin `#define`s (`YP/XM/YM/XP`,
  `TS_MINX/MAXX/MINY/MAXY`) that look unused -- no `TouchScreen`
  instance exists anywhere in `GUI`/`main.cpp`.

## Coupling

Depends on `lib/MultiOutput` (`PWM.h`, `Relay.h`, `RelayConfig.h`,
`PWMChannelConfig.h`, `PWMLabelFormat.h`), `lib/StateMachine`
(`PWMStateMachine.h`), `lib/RotaryEncoder` (`DIRECTION_TYPE` enum only),
and `lib/Display`.
