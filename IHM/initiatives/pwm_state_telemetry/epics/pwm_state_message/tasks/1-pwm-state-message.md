# Task 1: pwm-state-message

**Status:** done 2026-09-26 -- on `fixes/000005/pwm-state-telemetry` (off
`dev`), not the `1-pwm-state-message` task branch: Rafael chose a fix
branch. Board-side acceptance (listener sees `IHM_PWM_STATE` matching the
PWM tab) is Rafael's bench step.
**Depends on:** nothing

## As delivered (supersedes the contract below where they differ)

Decisions by Rafael, 2026-09-26 -- see the epic README:

1. `IHM_PWM_STATE` (307) is **per channel, same fields as
   `PWM_CHANNEL_CONFIG`** (13 B payload), not the 18-byte flat draft. Sent
   round-robin, one channel per ~100 ms tick, from `main.cpp`'s `pwmLast[]`
   (duty clamped to 100), not from `pwm_instance`.
2. `UART2_TX_RING_SIZE` default **64 -> 128** (`lib/Uart2/src/Uart2.h`), so
   board state + relay state + PWM state (69 B per tick) fit. RAM 58.8 %
   (was 56.4 %).
3. `MavlinkComms::sendPwmState(const mavlink_ihm_pwm_state_t&)`.
4. **Companion (`IHMPCController`, not under git -- this is the record):**
   `mavlink/ihm_dialect/` re-synced from `IHM/mavlink/generated/`
   (old copy kept as `mavlink/ihm_dialect.000005.bak/`); `MavlinkLink.h`
   decodes 307 and posts `WM_APP_PWM_STATE` (state split over
   WPARAM/LPARAM so the Win32 build works too; `PackPwmState`/
   `UnpackPwmState` round-trip checked on x64 and x86);
   `IHMPCController.cpp` shows live `PWM chN` readback in Outputs and
   replaces "no ack -- check the output pin" with "waiting for the board's
   readback..." -> "Board applied chN" / "Board reports chN differs" (after
   two mismatching readbacks, since the first can predate the apply);
   `HOW_TO_USE.md` updated. Backups: `MavlinkLink.h.bak`,
   `IHMPCController.cpp.bak`, `HOW_TO_USE.md.000005.bak`. Builds x64 and
   x86 Debug.

Power-on check (item 5 below): `initPwmDefaults()` fills `pwmLast[]` with
62500 Hz / 50 % / every output **off**, and leaves the timers untouched
until the first apply. The readback's `enabled = 0` is true to the
hardware. The frequency/duty it reports for a channel that was never applied
are the defaults the first switch press will use, not register contents.
That's acceptable because the outputs are disconnected. It's recorded here
rather than treated as a bug.

## Contract

### In

Nothing new. Protocol-definition change plus one send call wired to
state `main.cpp` already computes (the `pwm_instance.chN_*` assignments
made when a `PWM_CHANNEL_CONFIG` is applied -- `pwm_control` epic task 5,
`src/main.cpp` around line 169-194 as of 2026-09-22, confirm current line
numbers before editing).

### Delivers

1. **`mavlink/ihm_dialect.xml`** gains message **`IHM_PWM_STATE`, id
   307**, board -> PC, with the 18 fields in the epic README's table.
   Message `<description>`: periodic snapshot of all 4 PWM channels'
   applied state, board -> PC telemetry, mirrors `main.cpp`'s
   `pwm_instance.chN_*` fields the same way `IHM_RELAY_STATE` mirrors
   `relayState[]`. Read-only observation -- `PWM_CHANNEL_CONFIG` (303) is
   still the only way to change PWM state, this doesn't add a second one.

2. **`mavlink/generated/`** regenerated: `python mavlink/generate.py`.
   New `mavlink/generated/ihm_dialect/mavlink_msg_ihm_pwm_state.h` plus
   the touched shared headers, committed per policy (`mavlink/README.md`).

3. **`mavlink/README.md`** -- add the `IHM_PWM_STATE` row to the Messages
   table (board -> PC, 18 bytes).

4. **`lib/MavlinkComms/src/MavlinkComms.h`** gains a `sendPwmState(...)`
   taking the 18 fields (or a small struct -- match whatever
   `sendBoardState`/`sendRelayState` already do stylistically), packing
   and transmitting `IHM_PWM_STATE`. Class only packs/sends; caller
   gathers values (initiative's layering rule, same as `sendRelayState`).

5. **`src/main.cpp`** calls `mavlinkComms.sendPwmState(...)` with the
   current `pwm_instance.chN_*` values, in the same `timeCounter>=100`
   block that already calls `sendBoardState()`/`sendRelayState()` -- same
   cadence, no new timing mechanism. Confirm `pwm_instance` holds valid
   values even before any `PWM_CHANNEL_CONFIG` has ever arrived (i.e. its
   power-on-reset default matches actual hardware state) -- if not, that's
   a real bug to flag, not silently work around here.

6. **Companion app** (`IHMPCController`, outside this repo): update its
   own dialect copy and, if in scope for whoever implements this, replace
   or augment the "no ack -- check the output pin" label with real
   `IHM_PWM_STATE` readback. Confirm with Rafael whether that UI change
   is part of this task or a follow-up -- don't assume.

## Dependencies

None on other unmerged work. Depends conceptually on `pwm_control`
(merged) for `pwm_instance` to exist.

## Pre-work

None beyond what `pwm_control` task 1 already documented: `pymavlink`
installed under the Windows Python `mavlink/generate.py` runs with.

## Definition of done

- `python mavlink/generate.py` runs clean; `git status` shows only
  `ihm_dialect.xml`, files under `mavlink/generated/`, `mavlink/README.md`,
  `MavlinkComms.h`, and `main.cpp` (plus the companion app's own copy,
  tracked separately, if item 6 is in scope).
- `platformio run` (env `megaatmega2560`) builds.
- `test_native/run_tests.ps1` passes.
- Task file marked done in the same commit.

## Notes

- Message ids so far: 300 `IHM_BOARD_STATE`, 301 `CAN_SIGNAL_CONFIG`,
  302 `RS485_SIGNAL_CONFIG`, 303 `PWM_CHANNEL_CONFIG`, 304
  `IHM_RELAY_STATE`, 305 `IHM_SIMULATE_ENCODER`, 306
  `IHM_SIMULATE_BUTTON`. 307 is next -- re-check `ihm_dialect.xml` for
  what actually landed since 2026-09-22 before claiming 307 is still free.
- No `<include>common.xml</include>` -- same reasoning as every other
  message in this dialect.
- Conventional-commit: `feat(mavlink): add IHM_PWM_STATE message`.
