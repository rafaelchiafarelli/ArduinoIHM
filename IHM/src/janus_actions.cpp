// Real janus_handle_action() -- lib/GUI/janus_actions.c is Janus's
// generated scaffold (all-TODO stub), never compiled directly (it sits
// outside lib/GUI/src/, so PlatformIO's LDF skips it -- see
// platformio.ini's lib_deps comment for the same "Janus's own files
// aren't touched, ours make the real connection" split). This is that
// connection for the relay tab and the PWM tab's switches: relayState[]/multiOuput are main.cpp
// globals (declared extern there already, see its own comment on
// relayState[]).
#include <Arduino.h>
#include <MultiOutput.h>

extern "C" {
#include "janus_actions.gen.h"
#include "janus_bindings.gen.h"
}

extern MultiOutput multiOuput;
extern bool relayState[NUMBER_OF_RELAYS];

// main.cpp: flips one PWM output's enable/inverting bit and re-applies the
// channel to the timers (and the PWM tab) immediately.
void pwmToggleOutput(uint8_t ch, uint8_t out, bool inverting);
// main.cpp: steps channel `ch`'s frequency one lower (wrapping) and
// re-applies it. RE2 turns on the same label are handled in main.cpp.
void pwmCycleChannelFrequency(uint8_t ch);

extern "C" void janus_handle_action(janus_action_t action) {
    if (action >= JANUS_ACTION_TOGGLE_RELAY_0 && action <= JANUS_ACTION_TOGGLE_RELAY_7) {
        uint8_t index = (uint8_t)(action - JANUS_ACTION_TOGGLE_RELAY_0);
        relayState[index] = !relayState[index];
        multiOuput.getRelays()->setRelay(index, relayState[index]);
        // Mirror into the Relay tab's binding (switch + LED) -- nothing else
        // writes relay_instance, so without this the screen never follows.
        int* const field[NUMBER_OF_RELAYS] = {
            &relay_instance.relay_0, &relay_instance.relay_1, &relay_instance.relay_2, &relay_instance.relay_3,
            &relay_instance.relay_4, &relay_instance.relay_5, &relay_instance.relay_6, &relay_instance.relay_7 };
        bool* const dirty[NUMBER_OF_RELAYS] = {
            &relay_dirty.relay_0, &relay_dirty.relay_1, &relay_dirty.relay_2, &relay_dirty.relay_3,
            &relay_dirty.relay_4, &relay_dirty.relay_5, &relay_dirty.relay_6, &relay_dirty.relay_7 };
        *field[index] = relayState[index] ? 1 : 0;
        *dirty[index] = true;
        return;
    }
    switch (action) {
        case JANUS_ACTION_TOGGLE_PWM_CH0_ENABLED:   pwmToggleOutput(0, 0, false); break;
        case JANUS_ACTION_TOGGLE_PWM_CH0_INVERTING: pwmToggleOutput(0, 0, true);  break;
        case JANUS_ACTION_TOGGLE_PWM_CH1_ENABLED:   pwmToggleOutput(1, 0, false); break;
        case JANUS_ACTION_TOGGLE_PWM_CH1_INVERTING: pwmToggleOutput(1, 0, true);  break;
        case JANUS_ACTION_TOGGLE_PWM_CH2_A_ENABLED: pwmToggleOutput(2, 0, false); break;
        case JANUS_ACTION_TOGGLE_PWM_CH2_B_ENABLED: pwmToggleOutput(2, 1, false); break;
        case JANUS_ACTION_TOGGLE_PWM_CH2_C_ENABLED: pwmToggleOutput(2, 2, false); break;
        case JANUS_ACTION_TOGGLE_PWM_CH3_A_ENABLED: pwmToggleOutput(3, 0, false); break;
        case JANUS_ACTION_TOGGLE_PWM_CH3_B_ENABLED: pwmToggleOutput(3, 1, false); break;
        case JANUS_ACTION_TOGGLE_PWM_CH3_C_ENABLED: pwmToggleOutput(3, 2, false); break;
        case JANUS_ACTION_CYCLE_PWM_CH0_FREQUENCY:  pwmCycleChannelFrequency(0); break;
        case JANUS_ACTION_CYCLE_PWM_CH1_FREQUENCY:  pwmCycleChannelFrequency(1); break;
        case JANUS_ACTION_CYCLE_PWM_CH2_FREQUENCY:  pwmCycleChannelFrequency(2); break;
        case JANUS_ACTION_CYCLE_PWM_CH3_FREQUENCY:  pwmCycleChannelFrequency(3); break;
        default: break;
    }
}
