# Handoff: PROGMEM bugs found in Janus's AVR embedded-C runtime

Found 2026-08-23 while bringing up the Janus-generated UI (`IHM/lib/GUI/`)
on real ArduinoIHM hardware (Mega2560) for the first time. Two real bugs in
Janus's **fixed runtime library** (the hand-written, shipped-with-every-
project files — `janus_runtime.c`/`.h`, `janus_input_focus.c` — not
anything generated from YAML), both the same root mistake in two different
places. Worth porting back into the Janus repo itself so every future
AVR-target project doesn't hit this same first-boot dead end.

## The mistake

On AVR, `const` data is not automatically flash-resident — the CPU can
only read ordinary (RAM/IO) memory with `LD`; flash needs the separate
`LPM` instruction. Janus's own `JANUS_PROGMEM` attribute (`= PROGMEM` on
AVR, blank elsewhere) marks generated data as flash-resident specifically
*so it doesn't eat the target's tiny RAM budget* (see `janus_runtime.h`'s
own doc comment on this — an ATmega2560 has 8 KiB total). But marking data
`JANUS_PROGMEM` only changes *where the linker puts it* — every C-level
read of it still needs to go through `pgm_read_byte`/`pgm_read_ptr`/
`memcpy_P`, or you get garbage: an ordinary load from what's actually a
flash byte offset, reinterpreted as if it were a RAM address.

The runtime already had the right idiom for one case
(`janus_widget_load()`, a `memcpy_P` of a whole `janus_widget_desc_t` into
a RAM-resident local before reading any field) — but it wasn't applied
consistently. Two more PROGMEM-resident shapes were being read with plain
`->`/`[]` access instead:

1. **`janus_app_t.screens`** is `const janus_screen_desc_t *const *` — an
   *array of pointers* that's itself `JANUS_PROGMEM` in generated
   `*_app.gen.c` (see `janus_app_screens[]`). Reading `app->screens[i]`
   with ordinary indexing fetches a pointer *value* out of flash with a
   plain load — garbage, not the real address of the target screen.
2. **Every generated `janus_screen_desc_t` instance** (`pwm_screen`, etc.
   in `*_screen.gen.c`) is *itself* declared `JANUS_PROGMEM` — the whole
   struct lives in flash, not just something pointing at it. Reading
   `screen->widget_count`/`screen->widgets`/`screen->bound_struct` via
   plain `->` access has the identical problem, one level deeper: even
   once you have a *correct* `screen` pointer (fix #1 applied), the first
   thing done with it was already broken.

Both silently "half work" rather than crashing outright, which made this
nasty to chase down: `screen->widget_count` might read as a small,
plausible-looking number by luck, `render_widget()` then wanders into
whatever nearby data that garbage `widgets` pointer happens to land on,
and the traversal has no reason to be bounded or terminate anymore. On
real hardware this looked like: a few widgets appear to render correctly
at first (by chance, reading nearby valid-ish flash), then a fill_rect
with a runaway/garbage width takes over and the screen never finishes
rendering — confirmed by tracing `x` in `draw_area_sync` climbing past
27,000+ across thousands of calls with `y` and tile size held constant
(i.e. one single `fill_rect()` call's inner tiling loop, given a rect
whose `.w` was never a real authored value), and the boot sequence never
reaching the print statement placed right after the first
`janus_render_screen()` call, even after 30-60s of waiting.

## Fixes applied (in `IHM/lib/GUI/`, AVR-target project)

**`janus_runtime.h`**: added two declarations —
```c
const janus_screen_desc_t *janus_app_get_screen(const janus_app_t *app, uint16_t index);
void janus_screen_load(janus_screen_desc_t *dst, const janus_screen_desc_t *src);
```

**`janus_runtime.c`**:
- `janus_screen_load()` — `memcpy_P(dst, src, sizeof(*dst))`, exact mirror
  of the existing `janus_widget_load()`, for the reason in bug #2 above.
- `janus_app_get_screen()` — `pgm_read_ptr(&app->screens[index])` (plus
  the bounds check `janus_switch_screen()` already did inline), for bug #1.
- `janus_render_screen(screen)` — now `janus_screen_load()`s `screen` into
  a local `s` first, reads `s.widget_count`/`s.widgets`/`s.bound_struct`.
  `g_current_screen` itself still stores the *identity* (flash) pointer,
  consistent with how `g_focused_widget` etc. already work — only content
  reads need the loaded copy.
- New `static const void *current_bound_struct(void)` helper — centralizes
  "safely read `g_current_screen`'s `.bound_struct`" so `janus_set_focus()`
  and `janus_toggle_box()` (both previously doing
  `g_current_screen->bound_struct` directly) don't each duplicate the
  load-then-read dance.
- `janus_switch_screen()` / `janus_switch_screen_async_start()` — now call
  `janus_app_get_screen()` instead of indexing `app->screens[screen_index]`
  directly.

**`janus_input_focus.c`**:
- `walk_screen()` — same fix as `janus_render_screen()`: loads the screen
  struct locally before reading `.widget_count`/`.widgets`, instead of
  dereferencing the PROGMEM struct directly.

`pgm_read_ptr`/`memcpy_P` are already defined portably in
`janus_runtime.h` (real macros under `#ifdef __AVR__`, plain-memory
fallbacks otherwise), so none of the above needed platform `#ifdef`s at
the call sites — same pattern `janus_widget_load()` already established.

## Verified

Confirmed on real hardware (Mega2560, `pwm_screen` — header + 2 boxes of
mixed toggle/label/progress widgets): renders correctly, and a full
`janus_render_screen()` call for that screen (blocking mode) measured at
**~41-42ms**, comfortably inside the ~100ms periodic-redraw budget this
project's main loop uses. Not a general perf number for Janus (this is
one project's tile size/widget count/bus speed), but useful as a sanity
data point: the blocking render path is not inherently too slow for a
100ms-class refresh cadence, at least at this screen's complexity.

## Not a Janus bug — for context, don't rediscover these

Two other things came up in the same debugging session that are **not**
runtime-library bugs, noted here only so they aren't confused with the
above if this doc gets reused:

- An apparent "hang" that appeared *after* the fixes above, once the
  screen was rendering correctly, turned out to be a debug
  `Serial.print()` in ArduinoIHM's own `main.cpp` (added during this same
  session, to measure the 41-42ms figure above) firing every ~100ms tick
  indefinitely. Arduino's `Serial.print()` blocks once its TX ring buffer
  fills if nothing drains it fast enough on the host side — sustained
  per-tick printing eventually stalled the whole single-threaded firmware,
  encoder input included. Removed once the timing number was captured;
  nothing in Janus caused it.
- A separate, unrelated pre-existing issue in this project: `dac0.begin()`/
  `dac1.begin()` (MCP4725, I2C) blocking forever in the AVR TWI driver's
  unbounded `while` loops when the DAC doesn't ack — not a Janus issue at
  all, just something hit in the same hardware bring-up session.
