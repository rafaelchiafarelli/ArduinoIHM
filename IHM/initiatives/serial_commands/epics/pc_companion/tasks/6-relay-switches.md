# Task 6: relay-switches

**Status:** planned
**Depends on:** `relay_control/2-relay-command-message` (the dialect with
`IHM_RELAY_COMMAND`, id 309).

Rafael, 2026-09-27: "put 8 relays in the companion software and one switch
for each". Chosen behaviour: **send on click, follow the board**.

The companion (`C:\Users\rafae\source\repos\IHMPCController`) is not under
git: leave a `.bak` beside each edited file; this file is the record.

## Contract

### Delivers

1. **Dialect re-sync**: the companion's `mavlink/ihm_dialect/` from
   `IHM/mavlink/generated/` (old copy kept as a `.bak` folder).
2. **`MavlinkLink.h`**: `SendRelayCommand(port, mask, state)`, same style as
   `SendSimulateButton`.
3. **`IHMPCController.cpp`**: one "Relay N" checkbox per relay (8), beside
   the existing relay LEDs in Outputs. A click sends
   `IHM_RELAY_COMMAND { mask = 1 << N, state = (want on) << N }`; the box
   does not toggle itself. Its check state follows `WM_APP_RELAY_STATE`
   (304), so a change made on the board shows too. Disabled while
   disconnected, like the other command controls.
4. **`HOW_TO_USE.md`**: the relay switches, and 309 in the protocol table.

## Definition of done

Builds x64 and x86 Debug. Bench (after Rafael's go-ahead for the board):
clicking Relay 0 on the companion turns the board's relay 0 on (LED and
checkbox follow the readback), clicking again turns it off; toggling relay 0
on the board's Output tab moves the checkbox. Task file marked done.
