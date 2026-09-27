# Task 4: encoder-hold-repeat

**Status:** done 2026-09-26. Builds x64 and x86 Debug and is installed in
x64\Debug. Rate check on the board: 20 `IHM_SIMULATE_ENCODER` steps at
200 ms on a selected duty bar all applied, both ways (50 -> 70 -> 50). The
click-and-hold feel is Rafael's check.
**Depends on:** nothing

Requested by Rafael 2026-09-26: "keep sending the command while the button
is pressed (once every 200 ms is enough)", so a held RE2 CW/CCW can sweep
a selected duty bar (1 % per detent) without 100 clicks.

## Contract (as delivered)

`IHMPCController` is not under git, so this file is the record. Backups
next to the edited files: `IHMPCController.cpp.hold-repeat.bak` and
`HOW_TO_USE.md.hold-repeat.bak`.

1. **CCW/CW buttons repeat while held.** `RepeatButtonProc`
   (`SetWindowSubclass`, comctl32) sends one `IHM_SIMULATE_ENCODER` on
   mouse-down, then one every 200 ms (`kEncoderRepeatMs`) while the button
   still shows pressed. Dragging off the button pauses the repeat;
   release, losing capture or disabling the button stops it. The release's
   `BN_CLICKED` is swallowed (`g_encoderStepSentOnPress`), so a single
   click still sends exactly one step. A Space-key press still sends one
   step through `BN_CLICKED`.
2. **B0-B3 / Push do not repeat**, on purpose. `HOW_TO_USE.md` already
   says a simulated click needs ~1 s to register, so 200 ms repeats would
   merge into one click.
3. The send logic is factored into `SendSimulatedInput(id)`, used by both
   `WM_COMMAND` and the repeat timer.
4. `HOW_TO_USE.md`: the CCW/CW row documents holding.

## Definition of done

Builds (x64 + x86 Debug). Bench: holding RE2 CW on a selected duty bar
raises the duty about 5 % per second and stops on release; one click is
one step.
