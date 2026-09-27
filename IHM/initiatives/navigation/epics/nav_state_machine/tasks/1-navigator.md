# Task 1: navigator

**Status:** dropped 2026-09-27, never built. Rafael kept the controls already on `dev` as the final design; see the initiative README, "Closing decisions". The contract below is kept as the record of what was planned.
**Branch:** `1-navigator` (from `tasks`)
**Depends on:** nothing

## Contract

### Delivers

1. **`lib/Navigation/src/Navigator.h` + `Navigator.cpp`**. No AVR includes
   and no Janus includes.
   - `enum NavLevel : uint8_t { NAV_TAB, NAV_OPTION, NAV_SETTING, NAV_VALUE };`
   - The shape is supplied by the caller through a small interface of
     counts: `tabCount()`, `optionCount(tab)`, `settingCount(tab, option)`.
     `Navigator` holds a pointer to it and never owns widget data.
   - Inputs: `turn(int8_t delta)` (RE0), `enter()` (pbRE0), `back()` (B0),
     `tick(uint32_t nowMs)`.
   - State: `level()`, `tab()`, `option()`, `setting()`.
   - Outputs: after each input, `takeEvent()` returns one of
     `NAV_EV_NONE`, `NAV_EV_TAB_CHANGED`, `NAV_EV_FOCUS_CHANGED`,
     `NAV_EV_EDIT_BEGIN`, `NAV_EV_EDIT_STEP` (carries +1/-1),
     `NAV_EV_EDIT_END`, `NAV_EV_TIMEOUT`. The caller maps these to Janus
     and hardware. `Navigator` never decides what a value step means.
   - Rules: turns wrap within the current level. `enter()` does nothing
     on an option or setting that has 0 children. `back()` at `NAV_TAB`
     does nothing. Entering a level starts at index 0.
   - **Timeout:** `NAV_TIMEOUT_MS = 4000`. If `tick` sees 4 s since the
     last input that counts (see open question 6), the level returns to
     `NAV_TAB` and the current tab is kept. It emits `NAV_EV_TIMEOUT`
     only when the level was not already `NAV_TAB`. Handles `millis()`
     wraparound.
   - What happens to an edit in progress on timeout or `back()`
     (`NAV_EV_EDIT_END` with a keep/discard flag) follows initiative open
     question 3. Until that is decided, the flag is carried but not
     interpreted.
2. **`test_native/test_navigator.cpp`**: every row of the initiative's
   level table; wrap in both directions; zero-child enter; back at L0;
   timeout from each level; no timeout at L0; timer reset by input;
   `millis` wraparound. `lib/Navigation/src` and `Navigator.cpp` are
   added to the runner allowlists.
3. **`lib/Navigation/README.md`**: short.

Nothing calls it yet.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; task file
marked done in the same commit.
