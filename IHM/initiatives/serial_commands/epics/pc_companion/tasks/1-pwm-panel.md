# Task 1: pwm-panel

**Status:** done
**Branch:** `1-pwm-panel` (from `tasks`; the code itself lives outside this repo)
**Depends on:** `pwm_control` task 1 (message)

## Contract

Delivered in `C:\Users\rafae\source\repos\IHMPCController` (not a git repo,
so no commit exists for it -- this file is the record):

- `MavlinkLink.h`: `PwmOutputSettings`, `SendPwmChannelConfig(port, channel,
  frequencySelector, rawTop, out[3])` packing `PWM_CHANNEL_CONFIG`.
- `IHMPCController.cpp`: "PWM command" group -- channel combo (0-3),
  frequency combo (13 fixed + VARIABLE), raw TOP edit (enabled only for
  VARIABLE), per-output Enabled / Inverting / Duty% (B, C disabled for
  channels 0/1), Send (enabled only while connected), result line. The
  screen placeholder moved below the panel.
- Validation on the PC: duty 0-100, TOP 1-65535 when VARIABLE.

## Verification

- Builds: MSBuild Debug|x64 succeeds.
- Wire-compat: a frame built the same way (Python encoder) decodes with the
  firmware's C headers; CRC extras for 300-305 are identical on both sides.
- NOT verified: window layout (never viewed), actual waveform on a board.
  Rafael's bench step.
