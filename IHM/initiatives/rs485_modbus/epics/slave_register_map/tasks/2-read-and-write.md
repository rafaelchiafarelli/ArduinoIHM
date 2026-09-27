# Task 2: read-and-write

**Status:** planned
**Depends on:** task 1, `rtu_framing/2`

## Contract

1. Read callbacks serve from state snapshots that are safe to read from
   the Timer2 tick (the same atomic copy-out rules as `MavlinkComms`).
2. Write callbacks validate like the MAVLink path (`pwmWireConfigValid`,
   relay index) and queue; the superloop applies them through the same
   functions as MAVLink (`applyPwmChannel`, the relay toggle path), so
   the PWM tab, relays and telemetry all follow.
3. Invalid writes answer exception 03 (illegal data value); unknown
   addresses answer 02.
4. Native tests for the map callbacks (pure parts).
