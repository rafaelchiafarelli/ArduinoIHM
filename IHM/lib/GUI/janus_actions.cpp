// Hand-maintained action dispatch (Janus's own convention -- see
// janus_actions.gen.h's header comment: the enum is regenerated in full
// on every janus-generate run, this switch body never is). .cpp rather
// than the scaffolded .c because toggling a relay needs the real C++
// objects main.cpp owns (MultiOutput's Relay, and relayState[] -- Relay
// itself has no getter for a relay's current on/off state, see main.cpp).
extern "C" {
#include "janus_actions.gen.h"
#include "janus_bindings.gen.h"
}
#include <MultiOutput.h>

extern MultiOutput multiOuput;
extern bool relayState[NUMBER_OF_RELAYS];

extern "C" void janus_handle_action(janus_action_t action) {
    switch (action) {
        case JANUS_ACTION_TOGGLE_RELAY_0:
            relayState[0] = !relayState[0];
            multiOuput.getRelays()->setRelay(0, relayState[0]);
            relay_instance.relay_0 = relayState[0] ? 1 : 0;
            break;
        case JANUS_ACTION_TOGGLE_RELAY_1:
            relayState[1] = !relayState[1];
            multiOuput.getRelays()->setRelay(1, relayState[1]);
            relay_instance.relay_1 = relayState[1] ? 1 : 0;
            break;
        case JANUS_ACTION_TOGGLE_RELAY_2:
            relayState[2] = !relayState[2];
            multiOuput.getRelays()->setRelay(2, relayState[2]);
            relay_instance.relay_2 = relayState[2] ? 1 : 0;
            break;
        case JANUS_ACTION_TOGGLE_RELAY_3:
            relayState[3] = !relayState[3];
            multiOuput.getRelays()->setRelay(3, relayState[3]);
            relay_instance.relay_3 = relayState[3] ? 1 : 0;
            break;
        case JANUS_ACTION_TOGGLE_RELAY_4:
            relayState[4] = !relayState[4];
            multiOuput.getRelays()->setRelay(4, relayState[4]);
            relay_instance.relay_4 = relayState[4] ? 1 : 0;
            break;
        case JANUS_ACTION_TOGGLE_RELAY_5:
            relayState[5] = !relayState[5];
            multiOuput.getRelays()->setRelay(5, relayState[5]);
            relay_instance.relay_5 = relayState[5] ? 1 : 0;
            break;
        case JANUS_ACTION_TOGGLE_RELAY_6:
            relayState[6] = !relayState[6];
            multiOuput.getRelays()->setRelay(6, relayState[6]);
            relay_instance.relay_6 = relayState[6] ? 1 : 0;
            break;
        case JANUS_ACTION_TOGGLE_RELAY_7:
            relayState[7] = !relayState[7];
            multiOuput.getRelays()->setRelay(7, relayState[7]);
            relay_instance.relay_7 = relayState[7] ? 1 : 0;
            break;
        default:
            break;
    }
}
