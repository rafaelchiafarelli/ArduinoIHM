# Epic: pc_companion

The Windows companion app (`C:\Users\rafae\source\repos\IHMPCController`,
Win32, **not a git repo** -- edits there are not tracked by this
repository; this epic is the record) drives the board over the same MAVLink
dialect (`IHM/mavlink/`). It already shows live buttons/encoders/relays and
sends `IHM_SIMULATE_ENCODER`. Its vendored `mavlink/ihm_dialect/` must stay
in sync with `IHM/mavlink/generated/` (CRC extras verified identical
2026-09-20 for ids 300-305).

Janus is a black box here: the companion's `include/*.gen.h`,
`src/*.gen.c`, `janus_generated.harpia` are Janus output, not hand-edited.

## Tasks

```
1-pwm-panel     Win32 PWM tab: channel 0-3, frequency, per-output
                enable/invert/duty, sends PWM_CHANNEL_CONFIG   (deps: pwm_control/1)
2-how-to-use    HOW_TO_USE.md in the companion repo: build, connect,
                every panel, the wire protocol table, troubleshooting
```

## Acceptance

Companion builds in VS 2022 (x64 Debug); with a board on the port, applying
a PWM config from the panel changes the probed output (Rafael's bench step).
