# Handoff to Janus: vertical (cross-axis) alignment in a `row`

Written 2026-09-27. Same purpose as [README.md](README.md): an item for a
session working in Janus directly. It asks for a new yaml option; nothing
here changes Janus itself.

## The bug (ArduinoIHM side, reported by Rafael 2026-09-27)

ArduinoIHM tracks this as **`fixes/00000d/row-alignment`**, and that fix
is blocked on this handoff. Widgets of different heights in one `row`
don't line up. Every child is placed at the row's top, and nothing in the
yaml can center them. `Janus.md` already lists this gap ("`fill` doesn't
stretch the cross axis (open, 2026-08-23)" and the flexbox `align-items`
comparison).

Where it shows (geometry from the generated screens, Janus 125eef4):

| Where | Row children (x, y, w, h) | What you see |
|---|---|---|
| Output tab, each relay row (`focus_ring: true`) | toggle `6,54,24,12`; label "Relay N" `34,54,260,32`; LED `298,54,16,16` | switch and LED hug the top edge of a 32 px label |
| PWM tab, CH2/CH3 output rows | ring row with the toggle `130,254,40,30` (toggle at y 260 after the 6 px ring inset); label "A" `192,254,18,18`; ring row with the duty bar (bar at y 260) | the "A/B/C" letter sits 6 px above its switch and bar |

The second case is general: any plain widget next to a `focus_ring` row
is 6 px high against the ring row's content, because of the ring inset.

## What we need (the design is Janus's; this is the shape we'd use)

An explicit, authored option on `row` (never inferred) that places each
child on the cross axis, for example:

```yaml
- kind: row
  focus_ring: true
  align: center        # top (default) | center | bottom
  children: [ ... ]
```

- `center`: each child's y = row y + (row height - child height) / 2,
  done at build time like the rest of layout (no on-device layout math).
- A row without `align` keeps today's top alignment, so existing apps
  don't change.
- It must work on `focus_ring` rows (the relay rows) and on plain rows
  that contain ring rows (the PWM output rows).

## What ArduinoIHM does once it lands

`fixes/00000d/row-alignment`: add `align: center` to the 8 relay rows
(`relay.screen.yaml`) and to the CH2/CH3 A/B/C rows (`pwm.screen.yaml`).
Then regen `lib/GUI`, flash, re-vendor the companion mirror's `include/`
from the same Janus commit, and check it on the TFT and in the mirror.
