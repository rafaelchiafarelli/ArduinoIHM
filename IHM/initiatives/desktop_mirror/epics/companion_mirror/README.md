# Epic: companion_mirror

The IHMPCController side. The companion is not under git, so each task
file here is the record of what shipped: files touched, `.bak` names,
build result. Build with MSBuild Debug|x64 (and x86). Close the running
exe first.

## Tasks

```
1-vendor-and-msvc-gate   regen desktop target, vendor it, MSVC ctest for Janus   (no deps)
2-gdi-display-driver     draw_area_* -> RGB565 buffer -> placeholder             (deps: 1)
3-telemetry-to-bindings  304/307 -> relay_instance / pwm_instance (+ labels)     (deps: 2; open questions 2, 3)
4-mirror-link            308 -> janus_remote_state_apply; dirty redraw timer     (deps: 3, mirror_transport/1)
5-bench-check            mirror vs TFT on every screen                           (deps: 4, mirror_transport/2)
```

## Acceptance gate

The initiative's cross-epic gate (this epic is the last to land).
