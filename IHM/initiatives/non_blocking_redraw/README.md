# Initiative: non_blocking_redraw

Rendering must never stall the board. Today every full redraw blocks the
superloop: a switch to the PWM tab ~1.85 s, a re-render after an on-screen
action > 0.7 s. During that time no input is processed and no telemetry is
sent. Two presses merge into one, and the companion's mirror shows "no board"
(its 1.5 s timeout) on every switch to the PWM tab.

Planned 2026-09-27 as initiative-only (planning docs, no code yet), after
`desktop_mirror` closed with this gap handed over.

## Decisions (Rafael, 2026-09-27)

1. **Fix the cause, not the symptom.** Not a longer mirror timeout, and not
   sending the heartbeat from outside the superloop.
2. **Target: no superloop pass blocked more than 20 ms by rendering,** for
   every kind of redraw (screen switch, re-render after an action, dirty
   sweep, focus move, box toggle).
3. **Janus handoff now, no profiling first.** Janus's existing
   `render_mode: non_blocking` doesn't fit: its op queue alone is 6912 B of
   .bss against ~3.3 KB free, it caps at 256 ops (silently truncated), and
   the panel clear and bars stay blocking. What we need is written up in
   [`../janus_handoff/2026-09-27-bounded-nonblocking-render.md`](../janus_handoff/2026-09-27-bounded-nonblocking-render.md).
4. **`ui-rotation.md` stays shelved.** This initiative builds the
   incremental-redraw mechanism its scheduler question asks for. The
   rotation feature itself (widget contract, per-angle layouts) is not in
   scope.

## Epics

| Epic | Scope | Waits on |
|---|---|---|
| `loop_timing` | Measure the longest superloop pass and report it over MAVLink: the baseline now, and the gate's instrument later. | nothing |
| `janus_render` | Regenerate `lib/GUI` (and the companion's vendored `include/`) with Janus's bounded non-blocking render. | Janus delivering the handoff |
| `superloop_render` | `main.cpp` steps the render within a per-pass budget; every blocking render call site converted; then the bench gate. | both above |

Order: `loop_timing` (now) and the Janus work in parallel -> `janus_render`
-> `superloop_render`.

## Open questions (each blocks the task named in brackets)

1. **How max pass time travels** [`loop_timing/1`]: a new message
   `IHM_LOOP_STATS` (id 310), so older companions keep working, or a new
   field in `IHM_BOARD_STATE` (300), which changes its CRC and needs a
   companion re-sync.
2. **Render budget per pass** [`superloop_render/1`]: how much of the 20 ms
   the render step may use. Decide from `loop_timing`'s baseline of the
   non-render work.
3. **Input during a render job** [`superloop_render/1`]: an RE1 focus move
   or an RE2 edit mid-job. Is it applied now and drawn by the job, or
   queued until the job ends? That depends on the supersession/value rules
   Janus picks (handoff requirements 4-5).

## Cross-epic gate

On the bench (the old board revision counts, as for `desktop_mirror`):

- Max superloop pass <= 20 ms, measured by `loop_timing`, during a PWM
  tab switch, a switch back and forth 5x quickly, an on-screen action
  re-render, a `PWM_CHANNEL_CONFIG` dirty repaint and RE1 focus moves.
- `IHM_UI_STATE` heartbeat gaps stay <= 0.6 s during all of the above, and
  the companion mirror never shows "no board".
- Two simulated pbRE1 presses 300 ms apart both register.
- RAM within headroom (flag any jump > 3 %); `platformio run` and
  `test_native/run_tests.ps1` green.

## Branch chain

```
dev -> features -> non_blocking_redraw -> epics -> <epic> -> tasks -> <task>
```
