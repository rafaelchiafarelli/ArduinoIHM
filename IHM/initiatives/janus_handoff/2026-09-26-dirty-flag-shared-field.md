# Handoff to Janus: a dirty flag is consumed by the first widget bound to it

**Resolved 2026-09-26.** Janus initiative `shared_field_dirty` (Janus `dev`
c56f14c): `janus_render_screen_if_dirty` now tests the flags in one pass and
clears them in a second, and `janus_render_widget_if_dirty` is deprecated.
Regenerated here on navigation `value_editing/1`, and the workaround below is
removed. The PC path is fixed by the same regeneration.

Written 2026-09-26. Same purpose as [README.md](README.md): an item for a
session working in Janus directly (WSL `~/workspace/janus`, dev 36adb5a at
the time). Nothing here changes Janus from this repo.

## Symptom (found on ArduinoIHM hardware)

Two widgets bound to the same field only half-update on a dirty-aware
repaint. ArduinoIHM's PWM tab binds each channel's LED **and** its Enabled
toggle to one field (e.g. `pwm.ch0_enabled`), and the Relay tab binds each
relay's toggle **and** its LED to `relay.relay_N`. After the field changes
and `janus_render_screen_if_dirty()` runs:

| Tab / rows | Tree order | Repaints | Stale until something else redraws it |
|---|---|---|---|
| PWM CH0/CH1 | LED, then toggle | LED | toggle |
| PWM CH2/CH3 A/B/C, relays | toggle, then LED | toggle | LED |

## Root cause

`bind_consume_dirty()` (`runtime/embedded_c/src/janus_runtime.c`) checks
**and clears** the field's flag the moment one widget reads it. Every later
widget bound to the same field then sees it clear and is skipped. The
per-field dirty flag is shared, but consumption is per widget.

## Suggested fix

Don't clear inside the per-widget check. Test the flags during the sweep,
and clear every set flag once, after the whole sweep of
`janus_render_screen_if_dirty` (and after `janus_render_widget_if_dirty`
for its subtree). Alternatively, give each widget its own dirty bit at
generation time. A native test: two leaves bound to one field; mark it
dirty; one `if_dirty` sweep must repaint both.

## ArduinoIHM workaround until then

`src/main.cpp`: when RE2 sets a switch, do a full `janus_render_screen()`
(what pbRE1's press path already does) instead of the dirty-only repaint.
Frequency labels and duty bars own their fields and keep the dirty-only
repaint. **Not worked around:** the PC path (`PWM_CHANNEL_CONFIG` ->
`janus_render_screen_if_dirty(&pwm_screen)`) still leaves one widget of each
pair stale. Remove the workaround once a regen picks up the Janus fix.
