# Handoff to Janus: authorable knob colours on `toggle`

Written 2026-09-27. Same purpose as [README.md](README.md): an item for a
session working in Janus directly. It asks for a new yaml option; nothing
here changes Janus itself.

## The bug (ArduinoIHM side, reported by Rafael 2026-09-27)

ArduinoIHM treats the look of its switches as a bug, tracked as
**`fixes/00000b/toggle-colors`**. That fix is blocked on this handoff.

Today every `toggle` in `lib/GUI` (the PWM tab's Enabled/Inverting
switches, the Output tab's relay switches) draws:

- **off:** a white elliptical blob (the default `bg_color` track with its
  lightened knob on the left);
- **on:** a gray circle on the right (the default `color` track with its
  lightened knob).

The switch Rafael wants:

| state | track | knob |
|---|---|---|
| off | light gray | **red**, on the left |
| on | light gray (same) | **green**, on the right |

## Why ArduinoIHM can't do it alone

`Janus.md`'s widget catalog, `toggle` row: "a rounded pill track (`color`
when on, `bg_color` when off) … the knob colour is a fixed lightened tint
of the track, not separately authorable." In the runtime
(`janus_runtime.c`, `draw_toggle`):

```c
uint16_t track = on ? lw.color : lw.bg_color;
janus_fill_rounded_rect(r, r.h / 2, track);
janus_fill_circle(cx, cy, kd / 2 - pad, janus_rgb565_lerp(track, 0xffff, 96));
```

So the yaml can set two colours, and the track changes with the state.
There is no way to keep one track colour and give the knob its own colour
per state. Patching the vendored runtime in ArduinoIHM isn't an option
either: every regen overwrites it, and the PC companion's screen mirror
is generated from the same Janus and must stay pixel-comparable to the
board.

## What we need (the design is Janus's; this is the shape we'd use)

An explicit, authored option on `toggle` (never inferred) for:

1. the **track** colour, the same in both states, and
2. the **knob** colour **per state** (off / on).

For example (names are only a suggestion):

```yaml
- { kind: toggle, id: relay_0, bind: { message: relay, field: relay_0, type: int },
    on_press: toggle_relay_0,
    bg: "#D0D0D0",            # track, both states
    knob_off: "#FF0000",      # knob when off (left)
    knob_on: "#00FF00" }      # knob when on (right)
```

A toggle that sets none of the new keys should render exactly as today, so
existing apps don't change. Knob position (left when off, right when on)
stays as it is.

## What ArduinoIHM does once it lands

`fixes/00000b/toggle-colors`: set the new keys on every `toggle` in
`lib/GUI/pwm.screen.yaml` and `relay.screen.yaml` (light-gray track, red
off knob, green on knob; the exact gray can be tuned on the TFT). Then
regen `lib/GUI` with that Janus commit, re-vendor the companion's
`include/` from the same commit (desktop_mirror, see
`initiatives/desktop_mirror`), and check it on the TFT and in the mirror.
