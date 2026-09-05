// Real janus_handle_action() -- lib/GUI/janus_actions.c is Janus's
// generated scaffold (all-TODO stub), never compiled directly (it sits
// outside lib/GUI/src/, so PlatformIO's LDF skips it -- see
// platformio.ini's lib_deps comment for the same "Janus's own files
// aren't touched, ours make the real connection" split). This is that
// connection for the relay tab: relayState[]/multiOuput are main.cpp
// globals (declared extern there already, see its own comment on
// relayState[]).
#include <Arduino.h>
#include <MultiOutput.h>

extern "C" {
#include "janus_actions.gen.h"
}

extern MultiOutput multiOuput;
extern bool relayState[NUMBER_OF_RELAYS];

extern "C" void janus_handle_action(janus_action_t action) {
    if (action < JANUS_ACTION_TOGGLE_RELAY_0 || action > JANUS_ACTION_TOGGLE_RELAY_7) return;
    uint8_t index = (uint8_t)(action - JANUS_ACTION_TOGGLE_RELAY_0);
    relayState[index] = !relayState[index];
    multiOuput.getRelays()->setRelay(index, relayState[index]);
}
