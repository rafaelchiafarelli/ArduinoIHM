#pragma once
/**
 * UI-editable relay state: pure data + pure mutation, no BinaryOutputs and
 * no AVR dependency, so it's fully unit-testable on the host. Mirrors
 * PWMChannelConfig's shape (see PWMChannelConfig.h) but trivial, since a
 * relay is just an on/off toggle -- no frequency/duty/edge fields.
 * RelayElement (lib/Elements) renders this and applies it to the real
 * relay via Relay::setRelay(), the same "mutate config -> apply to
 * hardware -> refresh display" split PWMSimplex uses.
 */
struct RelayConfig {
    bool state = false;

    void toggle() { state = !state; }
};
