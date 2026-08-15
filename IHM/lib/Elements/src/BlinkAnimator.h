#pragma once
#include <stdint.h>

// Shared counter-driven blink mechanics for Label/LED. tick() is meant to be
// called once per update() -- this project has no delay()/timer-driven
// animation (see ARCHITECTURE.md's "no delay()" rule), so "how fast" a blink
// looks is purely a function of how many superloop passes it takes to flip,
// not wall-clock time. Ticks-to-toggle (a small threshold) is what makes the
// UI read as more responsive than its actual update rate.
class BlinkAnimator {
public:
    bool on = false;

    // Returns true only on the tick where `on` actually flips -- callers
    // redraw on that edge, not every tick, which is the partial-redraw
    // contract both Label and LED depend on.
    bool tick(uint8_t threshold){
        if(++counter_ < threshold) return false;
        counter_ = 0;
        on = !on;
        return true;
    }

    // Called whenever a widget's state changes away from blinking, so the
    // next time it blinks again it starts from a known edge instead of
    // wherever the counter happened to be left.
    void reset(bool startOn){
        counter_ = 0;
        on = startOn;
    }

private:
    uint8_t counter_ = 0;
};
