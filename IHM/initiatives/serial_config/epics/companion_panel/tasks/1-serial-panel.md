# Task 1: serial-panel

**Status:** planned
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
