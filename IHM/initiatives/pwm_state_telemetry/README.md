# Initiative: pwm_state_telemetry

**Defect record, not scheduled.** Found 2026-09-21/22 explaining the PC
companion app's PWM panel to Rafael: it always shows "Sent ch%d @ %s (no
ack -- check the output pin)" after `PWM_CHANNEL_CONFIG`, and that message
is honest, not a bug -- the board never tells the PC whether a PWM config
was actually applied. Rafael: "I will fix it later" -- this initiative
exists so the gap isn't lost, not to be picked up immediately.

Normally this branches off `features` per the workflow skill's chain
(`features -> pwm_state_telemetry -> epics -> <epic> -> tasks -> <task>`).
It skips the `epics`/`tasks` container branch levels here only because
those names were already taken by the in-flight `serial_commands` chain
in the same clone when this was written (see
`initiatives/workflow_defects/` for the process gap this exposed) --
folder layout below still follows the normal initiative/epic/task shape,
so create the container branches normally when this is actually picked up
for implementation.

## The gap

`mavlink/ihm_dialect.xml` has `PWM_CHANNEL_CONFIG` (id 303, PC -> board)
but nothing carrying PWM state back. Compare `IHM_RELAY_STATE` (id 304,
board -> PC): relays have a real telemetry readback; PWM does not. The
companion app's PWM panel therefore can never show ground truth, only
"the last thing I sent" -- and if a frame is dropped, sent to a channel
that's actually disabled some other way, or the board rejects it
(`pwmWireConfigValid()`), the PC has no way to notice.

The board-side data already exists: `src/main.cpp` (around the
`pwm_instance.chN_*` assignments, `pwm_control` epic task 5) reads
exactly this per-channel state -- enabled, duty, inverting/A-B-C -- to
mirror it into the on-screen PWM tab every time a config is applied. This
initiative is "send what's already computed," not new hardware-state
tracking.

## Scope (for whoever picks this up)

- One epic, `pwm_state_message`, one task -- see
  `epics/pwm_state_message/`.
- Out of scope: PWM readback isn't the same as confirming the *physical*
  pin -- this closes the "did the board accept and apply the command"
  gap, not the "is the transistor/relay downstream actually switching"
  gap, which has no telemetry path regardless.
