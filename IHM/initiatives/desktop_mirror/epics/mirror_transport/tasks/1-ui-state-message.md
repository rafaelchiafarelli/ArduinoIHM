# Task 1: ui-state-message

**Status:** planned
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
