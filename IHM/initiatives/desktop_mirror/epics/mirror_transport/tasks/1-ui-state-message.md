# Task 1: ui-state-message

**Status:** done 2026-09-27. Delivered as written: message 308 in the dialect,
`mavlink/generated{,_py}` regenerated, `MavlinkComms::sendUiState` (four
scalars), `test_native/test_ui_state_message.cpp` (3 tests; the runner now
also has `mavlink/generated/ihm_dialect` on its include path). 135 native
tests pass. Build: RAM 59.1 % (4841 B, +10 B for the dialect's CRC table
entry), flash 76830 B. Nothing calls `sendUiState` yet (task 2).
**Branch:** `1-ui-state-message` (from `tasks`)
**Depends on:** nothing

## Contract

1. `mavlink/ihm_dialect.xml`: message **308 `IHM_UI_STATE`**, board -> PC
   telemetry, one field per `janus_remote_state_t` member, same types and
   meaning (copy the semantics from `lib/GUI/runtime/include/janus_remote.h`):
   `uint16_t screen`, `int16_t focus`, `int16_t nav_focus`,
   `uint16_t boxes_expanded`. The description says it is the payload
   for `janus_remote_state_apply` and names the Janus commit the struct
   comes from.
2. Regenerate `mavlink/generated` and `mavlink/generated_py` with
   `mavlink/generate.py` (the procedure in `mavlink/README.md`).
3. `MavlinkComms::sendUiState(screen, focus, navFocus, boxesExpanded)`
   takes four scalars, not the Janus struct: the serial_commands
   "Layering rule" keeps Janus UI headers out of `MavlinkComms`. It only
   packs and sends, like `sendRelayState`.
4. Native test: pack -> decode round-trip of all four fields, including
   `focus = -1` and `nav_focus = -1`.

## Not in this task

Calling it from `main.cpp` (task 2). Anything on the PC.
