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

## Open questions (each blocks the task named)

1. **Heartbeat period** for `IHM_UI_STATE` when nothing changes. Proposal:
   1 s (20 B/s on a 250000-baud link). Blocks `mirror_transport/2`.
2. **`PWMLabelFormat.cpp` in the companion.** Should the vcxproj reference
   `lib/MultiOutput/src/PWMLabelFormat.{h,cpp}` in this repo by path (one
   source, but the companion build then needs this checkout), or keep a
   copy (self-contained, can drift)? Blocks `companion_mirror/3`.
3. **SERIAL tab (`bus_status`) values.** The board never reports its
   CAN/RS485 config back. The PC could show only what it sent itself this
   session (blank after a companion restart or if another tool sent it),
   or the board could echo the config, which is a new telemetry message
   and new scope. Blocks `companion_mirror/3`, for that screen only.

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
