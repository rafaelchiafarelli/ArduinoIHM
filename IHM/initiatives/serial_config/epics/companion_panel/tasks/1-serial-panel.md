# Task 1: serial-panel

**Status:** planned
**Depends on:** `wire/2`

## Contract

1. A "SERIAL command" group in `IHMPCController.cpp` (channel selector
   CAN0/CAN1/RS-485; per-kind fields: enable, bitrate/baud, parity,
   protocol, J1939 SA, generator ID/EXT/DLC/data/period/repeat) and Send.
2. `MavlinkLink.h`: senders for 301/302 and the new bus-params message;
   the listener decodes the config telemetry and posts it to the UI
   thread (same pattern as `WM_APP_PWM_STATE`).
3. A readback line per bus with applied/differs, like the PWM panel.
4. `HOW_TO_USE.md`: SERIAL panel section and the wire-table rows.
