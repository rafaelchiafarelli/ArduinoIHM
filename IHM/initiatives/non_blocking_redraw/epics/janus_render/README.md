# Epic: janus_render

Pick up Janus's bounded non-blocking render once it exists
([handoff](../../../janus_handoff/2026-09-27-bounded-nonblocking-render.md)).
No ArduinoIHM code changes here beyond the regenerated `lib/GUI` and the
`app.yaml` mode switch.

## Tasks

```
1-regen-bounded-render   regenerate lib/GUI + companion include/ with the new mode   (deps: Janus)
```

## Acceptance gate

`platformio run` builds with the new mode enabled, RAM within headroom
(the handoff asks for a 100-300 B async state); `test_native/run_tests.ps1`
passes; the companion builds x64/x86 with the regenerated `include/`.
