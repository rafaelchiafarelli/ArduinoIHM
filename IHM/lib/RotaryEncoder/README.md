# RotaryEncoder

Two unrelated small pieces share this folder (PlatformIO adds the whole
directory as one include path, hence `main.cpp` can `#include
<ButtonMap.h>` even though there's no separate `lib/ButtonMap`).

## `RotaryEncoder.h` (`.cpp` empty, all logic header-inline)

Quadrature decoder for up to `MAX_NUMBER_EMCODERS` (3) encoders.
`ms_handler(uint16_t bMap)` samples 2 bits per encoder out of the packed
input bitmap and runs a 4-bit gray-code lookup table (`cDirection[16]`)
to detect CW/CCW, latched per-encoder. `getDirection(uint8_t enc)` reads
and clears that latch (edge-triggered, single read per call).

Each encoder's bit positions within `bMap` are hardcoded (`Pin0`/`Pin1` =
6/7, 9/10, 12/13), matching [BinaryInput](../BinaryInput/README.md)'s
pin-table indices. The constructor takes a `BinaryInputs*` but never
dereferences it -- decoding works purely off the `bMap` value passed into
`ms_handler`.

**Invocation:** `rotaryEncoders.ms_handler(bMap)` runs in
`ISR(TIMER2_COMPA_vect)`'s ~25ms branch (same cadence as `Comms`, despite
the `ms_handler` name suggesting per-ms). `getDirection()` is called from
`main()`'s superloop, once per encoder per pass.

## `ButtonMap.h/.cpp`

One pure function, `buildButtonMap(uint16_t bMap) -> uint8_t`. Repacks
`BinaryInputs`' raw bit layout into [GUI](../GUI/README.md)'s compact
button-map format: bits 0-3 = buttons 0-3 unchanged, bit 4 = rot2 button,
bit 5 = rot1 button, bit 6 = rot0 button, bit 7 = hardcoded sentinel `1`.
No hardware, no state -- deliberately AVR-independent and unit-testable.
Called directly from `main()`'s superloop; its output feeds straight into
`gui.update(...)`.

## Coupling

Both files hardcode bit positions against `BinaryInputs`' pin table (see
[BinaryInput/README.md](../BinaryInput/README.md) -- that's the single
source of truth three modules silently depend on). `ButtonMap`'s output
format is in turn hardcoded into `GUI::update()`'s bit-mask checks.
