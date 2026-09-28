# Task 1: serial-panel

**Status:** done (2026-09-28) -- builds and unit check pass; bench pending (see Result)
**Depends on:** `wire/2`

## Contract

1. A "SERIAL command" group in `IHMPCController.cpp` (channel selector
   CAN0/CAN1/RS-485; per-kind generator fields: enable, ID/EXT/DLC/data
   for CAN, length/data for RS-485, period, repeat) and Send. Laid out so
   bus initiatives can add their settings to a bus's section (the
   extension contract, initiative README).
2. `MavlinkLink.h`: senders for 301/302 (and the extension message if
   `wire/1` defines one); the listener decodes the config telemetry and
   posts it to the UI thread (same pattern as `WM_APP_PWM_STATE`).
3. A readback line per bus with applied/differs, like the PWM panel.
4. `HOW_TO_USE.md`: SERIAL panel section and the wire-table rows.

## Result

Companion `C:\Users\rafae\source\repos\IHMPCController` (not under git;
backups `*.serial-config.bak` beside every edited file, and
`mavlink/ihm_dialect.serial-config.bak/`):

- `mavlink/ihm_dialect/` re-synced from `IHM/mavlink/generated/ihm_dialect`
  (ids 300-314); the four new `mavlink_msg_*.h` added to
  `IHMPCController.vcxproj` + `.filters`.
- `MavlinkLink.h`: `SerialGeneratorSettings`, `SendSerialGeneratorConfig`
  (301/302), `SendSerialSetting` (313), `Pack/UnpackSerialSettingState`.
  The listener decodes 311/312 into a per-bus latest-value store behind a
  `CRITICAL_SECTION` (the RS-485 payload doesn't fit WPARAM/LPARAM) and
  posts `WM_APP_SERIAL_STATE` (bus in wParam); 314 posts
  `WM_APP_SERIAL_SETTING_STATE`. The store is cleared on `Stop()`.
- `IHMPCController.cpp`: "SERIAL command (PC -> board)" group below the
  PWM panel: bus, Enabled, ID (hex) + Extended (CAN only), DLC/LEN, data
  (hex bytes), period, repeat, Send; input checked against the board's
  ranges before sending; applied/differs result with the PWM panel's
  two-strike rule; one readback line per bus; a keyed-setting row
  (key, value, Set, the board's answer). Choosing a bus loads its latest
  readback into the fields. Window min height 680 -> 760.
- `HOW_TO_USE.md`: "Driving the SERIAL buses" section, window table row,
  wire table rows 301/302 (now sent) and 311-314.
- Checks: x64 and x86 Debug compile and link (built into a scratch output
  dir: the running companion held `x64\Debug\IHMPCController.exe`); only
  the file's existing C4312 `HMENU`-cast warning class, no new kind.
- **Pending, bench:** a config sent from the panel shows on the TFT and in
  the readback; an edit made with the knobs shows in the panel. Rebuild
  into `x64\Debug` once the running companion is closed.
