# Task 5: bench-check

**Status:** done 2026-09-27, closed by Rafael's decisions:
- **The old board counts for the gate.** The mirror is about the UI, not
  the output bus, so no new-revision run is needed.
- **Mirror == TFT and no flicker: confirmed by Rafael** by eye.
- **The 308 gap is handed over, not fixed here.** Re-measured 2026-09-27 on
  `dev` (dbbdcce): idle heartbeat every 0.50 s, but one switch to the PWM
  tab stalls `IHM_UI_STATE` for 1.85 s, and two RE0 turns 1 s apart gave
  1.97 s and 2.38 s. That is past the 1.5 s "no board" timeout, so the
  overlay flashes on every switch to PWM (worse than the first run: the
  PWM tab has more widgets now). Rafael chose to fix the cause, the
  blocking full-screen redraw, in a new initiative (`non_blocking_redraw`),
  not with a longer timeout.
- Relays are covered since: the companion's relay switches
  (`serial_commands` `pc_companion/6`) moved relay 0 both ways, and the
  mirror follows 304.

## Bench run 2026-09-27 (old hardware revision, Rafael's go-ahead)

Setup: `mirror_transport` firmware (05b69cb) flashed to COM7; companion
connected on COM3 (Serial2). The board was driven through the companion's
own controls (scripted `BM_CLICK`), and the mirror was screenshotted per step.

- **308 stream** (pymavlink on COM3): one frame every ~0.5 s while idle,
  and one on every change: RE0 CW gives screen 0 -> 1 (`boxes_expanded` 7)
  -> 2; RE1 CW/CCW moves focus 0 -> 1 -> 0.
- **Mirror followed the board** through PWM idle (`5-01`), SERIAL with
  its boxes open (`5-02`), Output + RE1 focus, PWM + RE1 focus, a
  panel-sent `PWM_CHANNEL_CONFIG` (`5-08`: CH0 LED green, "Board applied
  ch0") and a companion restart (`5-09`: board state back immediately).
  The "no board" overlay showed in 0 of ~150 samples across these steps.
- **Finding (open, Rafael's call):** a full-screen redraw on the board
  blocks the superloop, so 308 pauses during screen switches. Two RE0
  turns ~1 s apart gave a 1.97 s gap, longer than the 1.5 s overlay
  timeout (initiative decision 1). Single switches stay under it. Options:
  a longer timeout, or sending 308 from a path that isn't blocked by
  rendering.
- **Not verifiable on this board:** a simulated pbRE1 on a focused relay
  did not toggle it (`5-05`; 304 stayed 0x00), and once it jumped the board
  to the PWM tab. `IHM_BOARD_STATE.buttons` reads 0b1010011 at rest
  (B0, B1, pbRE2 and pbRE0 "pressed"), which is the old revision's input
  wiring, not the mirror. The mirror matched 304 throughout.
- **Flicker report:** Rafael saw the whole companion window flicker during
  the scripted run. The likely cause is the test tooling (`PrintWindow`
  captures every 40-150 ms force full repaints). With the app idle and
  connected, 99 mirror captures over 6 s were identical, and
  `tests/RemoteStateRoundTrip.cpp` (companion) shows every valid board
  state round-trips through apply/get with no repeat redraw. Waiting on
  Rafael's eyes on the untouched app to confirm.
- **Not done:** mirror == TFT per screen (needs someone looking at the
  TFT), and the gate on the new hardware revision.
**Depends on:** task 4, `mirror_transport/2`

## Contract

Run the initiative's cross-epic gate on the **new** hardware revision.
Ask Rafael first: the COM7 board is the old revision. Record per screen
and per step whether mirror == TFT, with screenshots in this file's
folder. Any mismatch becomes a new task or a Janus handoff item, not a
patch in this task.
