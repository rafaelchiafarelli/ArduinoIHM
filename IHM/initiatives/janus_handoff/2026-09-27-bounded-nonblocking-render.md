# Handoff to Janus: a non-blocking render that fits an 8 KB AVR

Written 2026-09-27. Same purpose as [README.md](README.md): an item for a
session working in Janus directly (WSL `~/workspace/janus`, dev fcc4ed3 at
the time). Nothing here changes Janus from this repo. ArduinoIHM initiative
`non_blocking_redraw` waits on it (its `janus_render` epic).

## The problem (measured on ArduinoIHM hardware)

ArduinoIHM (Mega2560, 8 KB RAM, 320x480 parallel TFT on one 8-bit port)
uses `render_mode: blocking`. Every full render stalls the superloop:

- A switch to the PWM tab (`janus_switch_screen`: clear + status + nav +
  `janus_render_screen`) blocks for **~1.85 s**. Two switches 1 s apart
  gave 1.97 s and 2.38 s gaps in the board's 500 ms UI-state heartbeat.
- An on-screen action followed by `janus_render_screen` blocks > 0.7 s.
  Two button presses inside that window merge into one.
- During the stall the board sends no telemetry, so the desktop mirror
  (Janus's own `janus_remote_*`, desktop target) shows "no board".

Rafael's target: **rendering never holds one superloop pass for more than
~20 ms** (16 MHz AVR), for every kind of redraw: screen switch, full
re-render after an action, dirty sweep, focus change, box toggle.

## Why the existing `render_mode: non_blocking` doesn't fit

`janus_runtime.c`'s async path (`janus_render_screen_async_start` +
`janus_render_poll`) cannot be used here:

1. **RAM.** `g_async_ops[256]` is 6912 B of .bss. ArduinoIHM has ~3.3 KB
   free in total (59.3 % of 8192 B used, before the stack).
2. **Capacity.** 256 ops for a whole screen, silently truncated. With
   16x16 tiles, the PWM tab (four channel icons of 80x80 and 126x126, plus
   bars, switches and text) is likely well past 256 fills + glyphs.
3. **Still-blocking parts.** `janus_switch_screen_async_start` does the
   full-panel clear (320x480 = 600 tiles), the status band and the nav
   strip synchronously. The clear alone is a large share of a switch.
4. **Coverage.** Only full-screen render and switch have async forms.
   `janus_render_screen_if_dirty`, `janus_set_focus` (ring draw/erase),
   `janus_toggle_box` and the nav/status bars are blocking only.

## What ArduinoIHM needs (requirements, not a design)

1. **Bounded RAM:** the async state costs a small, fixed amount (order of
   100-300 B), independent of screen complexity. No per-op queue for the
   whole screen, and no silent truncation.
2. **Budgeted progress:** one call does a bounded amount of driver work
   and returns, e.g. `bool janus_render_step(uint16_t max_ops)` (or a
   pixel budget), returning true while work remains. The caller picks the
   budget per pass.
3. **Everything incremental:** the panel clear (in bands), the status and
   nav bars, full screen render, dirty sweep, focus ring draw/erase and box
   toggle all go through the same stepped path.
4. **Supersession is defined:** starting a new job while one is in flight
   (a second screen switch mid-render is the normal case with an encoder)
   cancels the old one, and the panel still ends up fully correct for the
   new job. State the rule in the header.
5. **Values:** say whether a widget drawn late in a job shows the value at
   job start or at draw time, and what happens to a dirty bit set while a
   job is running. It must not be lost; the next sweep has to repaint.
6. **Blocking API stays.** The desktop target (the companion's mirror
   calls `janus_remote_state_apply` + render on a timer) and existing
   projects keep working unchanged.
7. **Host tests:** a stepped render through the mock driver produces the
   same driver log as the blocking render, a job superseded at every step
   index still ends with the new screen's exact log, and the RAM
   bound is a `sizeof` check.

## One possible approach (Janus's call)

The runtime's comment rejects a resumable traversal because fill/glyph
loops would need coroutines. An alternative that needs neither a queue nor
coroutines is **replay with skip**. Draw functions are deterministic, so a
job remembers only "N driver ops already emitted". Each step re-runs the
traversal with driver calls suppressed for ops < N, emits the next K and
stops. The cost is CPU (re-walking the tree without driver calls), not RAM.
The driver calls are what's slow here. A per-top-level-widget cursor would
make the re-walk cheap. Clearing in horizontal bands fits the same op
counter.

## ArduinoIHM side (for context)

Once a regen provides it, ArduinoIHM's `non_blocking_redraw` initiative
switches `app.yaml` to the new mode, calls the step function from
`main.cpp`'s superloop within its budget, converts every blocking render
call site, and measures the result with its own max-pass-time telemetry.
The companion's vendored `include/` is regenerated from the same commit.
