# Initiative: desktop_mirror

The IHMPCController companion shows a live mirror of the board's TFT in
its screen placeholder, drawn by the same Janus-generated UI as the
board and pixel-comparable to it. The board is the single source of UI
truth. Nothing the user does inside the mirror changes it. The
companion's existing command panels (PWM, encoders, buttons) still send
MAVLink. The board acts on them, and the mirror follows the board.

This builds on Janus `desktop_windows_mirror` (merged to Janus `main` 2026-09-21,
answering our handoff `janus_handoff/2026-09-20-desktop-windows-and-mirror.md`):
an MSVC-safe desktop build, `janus_remote.h`
(`janus_remote_state_get` / `janus_remote_state_apply`, state =
`{screen, focus, nav_focus, boxes_expanded}`, 8 bytes), and
`janus.sh --mirror`.

## Decisions (Rafael, 2026-09-27)

1. **Embedded in the placeholder, with a companion-owned GDI driver.**
   The companion implements Janus's driver contract
   (`draw_area_sync` / `draw_area_async` / `display_busy`) into an RGB565
   framebuffer and paints it into the placeholder. Janus's SDL driver is
   not built (`JANUS_BUILD_DESKTOP_DRIVER=OFF`), so no SDL2 dependency
   and no Janus handoff. The companion's own loop (a Win32 timer) plays
   the role of the scaffolded SDL mirror `main.c`.
2. **Bound values come from existing telemetry.** The PC fills the
   generated bindings structs from `IHM_PWM_STATE` (307) and
   `IHM_RELAY_STATE` (304), the same way `main.cpp` does on the board. No
   new bound-value messages.
3. **UI state gets one new board -> PC message** carrying
   `janus_remote_state_t` (`mirror_transport/1`).
4. **Same UI source on both sides.** The companion's generated code must
   come from the same `lib/GUI/app.yaml` and the same Janus commit as the
   firmware's `lib/GUI`. Otherwise the mirror is not pixel-comparable. The
   Janus commit is recorded in both places.

## Epics

| Epic | Where | Scope |
|---|---|---|
| `mirror_transport` | this repo (firmware + dialect) | `IHM_UI_STATE` message; board sends `janus_remote_state_get` on change + heartbeat. |
| `companion_mirror` | `IHMPCController` (not under git; task files are the record) | Vendor the regenerated desktop UI + MSVC gate for Janus; GDI driver; telemetry -> bindings; mirror link; bench check. |

Task order:

```
mirror_transport/1-ui-state-message ─> mirror_transport/2-ui-state-send ─────────────┐
companion_mirror/1-vendor-and-msvc-gate ─> 2-gdi-display-driver ─> 3-telemetry-to-bindings ─> 4-mirror-link ─> 5-bench-check
                                                    (4 also needs mirror_transport/1) ┘
```

## Open questions -- all answered 2026-09-27 (Rafael)

1. ~~Heartbeat period.~~ **500 ms.** `IHM_UI_STATE` is also resent every
   500 ms when nothing changed (about 40 B/s at 250000 baud). The mirror
   treats 3 missed heartbeats (1.5 s) as "no board".
2. ~~`PWMLabelFormat.cpp` in the companion.~~ **Copy it.** The companion
   keeps its own copy of `lib/MultiOutput/src/PWMLabelFormat.{h,cpp}`.
   Record the source commit in the copy's header. Re-copy it whenever the
   board's version changes, and task 3's label check catches drift.
3. ~~SERIAL tab values.~~ **Show what the PC sent.** `bus_status_instance`
   holds the companion's own last-sent CAN/RS485 config this session. It
   is blank (generated defaults) after a companion restart or if another
   tool configured the board. `HOW_TO_USE.md` states this caveat. No new
   firmware message.

## Cross-epic gate

On the bench, with the **new** hardware revision (the COM7 board is the
old revision, so ask before using it): the mirror matches the TFT on every
screen after each of: a PC-sent `PWM_CHANNEL_CONFIG`, a simulated RE0/RE1/RE2
turn and push, a relay change, and a companion restart mid-session. The
firmware build and `test_native/run_tests.ps1` stay green.

## Out of scope

- Any Janus change (decision 1).
- Clicking in the mirror to drive the board. The mirror is view-only by
  Janus's design, and input keeps going through the existing panels.
- Bound-value telemetry beyond what exists (see open question 3).

## Branch chain

```
dev -> features -> desktop_mirror -> epics -> <epic> -> tasks -> <task>
```

`companion_mirror` tasks commit only their task-file records here. The
code lives in `C:\Users\rafae\source\repos\IHMPCController`, with `.bak`
files next to each edited file.
