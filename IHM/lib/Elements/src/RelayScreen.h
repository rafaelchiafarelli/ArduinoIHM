#pragma once

#include <Relay.h>
#include "RelayElement.h"

#define RELAYSCREEN_X0 4
#define RELAYSCREEN_Y0 47
#define RELAYSCREEN_W 314
#define RELAYSCREEN_ROW_H 53

/**
 * The Output tab: one row per relay (Relay 0..7), plus the selection cursor
 * that decides which row the encoder currently drives. Only one navigation
 * level is needed -- unlike PWMScreen's channel-select/field-edit split
 * (see PWMStateMachine.h) -- because a relay has nothing to edit besides
 * on/off: d1 moves the highlighted row, the button toggles it (see
 * GUI.cpp's OUPTUT_SELECTED case).
 */
class RelayScreen {
    private:
        RelayElement relay0;
        RelayElement relay1;
        RelayElement relay2;
        RelayElement relay3;
        RelayElement relay4;
        RelayElement relay5;
        RelayElement relay6;
        RelayElement relay7;
        RelayElement *rows[NUMBER_OF_RELAYS];
        uint8_t selectedIndex = 0;

        void applySelectionHighlight() {
            for (uint8_t i = 0; i < NUMBER_OF_RELAYS; i++) {
                rows[i]->setSelected(i == selectedIndex);
            }
        }

    public:
        RelayScreen(Display *tft, Relay *relay):
            relay0(RELAYSCREEN_X0, RELAYSCREEN_Y0 + 0 * RELAYSCREEN_ROW_H, RELAYSCREEN_W, RELAYSCREEN_ROW_H, tft, relay, 0),
            relay1(RELAYSCREEN_X0, RELAYSCREEN_Y0 + 1 * RELAYSCREEN_ROW_H, RELAYSCREEN_W, RELAYSCREEN_ROW_H, tft, relay, 1),
            relay2(RELAYSCREEN_X0, RELAYSCREEN_Y0 + 2 * RELAYSCREEN_ROW_H, RELAYSCREEN_W, RELAYSCREEN_ROW_H, tft, relay, 2),
            relay3(RELAYSCREEN_X0, RELAYSCREEN_Y0 + 3 * RELAYSCREEN_ROW_H, RELAYSCREEN_W, RELAYSCREEN_ROW_H, tft, relay, 3),
            relay4(RELAYSCREEN_X0, RELAYSCREEN_Y0 + 4 * RELAYSCREEN_ROW_H, RELAYSCREEN_W, RELAYSCREEN_ROW_H, tft, relay, 4),
            relay5(RELAYSCREEN_X0, RELAYSCREEN_Y0 + 5 * RELAYSCREEN_ROW_H, RELAYSCREEN_W, RELAYSCREEN_ROW_H, tft, relay, 5),
            relay6(RELAYSCREEN_X0, RELAYSCREEN_Y0 + 6 * RELAYSCREEN_ROW_H, RELAYSCREEN_W, RELAYSCREEN_ROW_H, tft, relay, 6),
            relay7(RELAYSCREEN_X0, RELAYSCREEN_Y0 + 7 * RELAYSCREEN_ROW_H, RELAYSCREEN_W, RELAYSCREEN_ROW_H, tft, relay, 7)
        {
            rows[0] = &relay0; rows[1] = &relay1; rows[2] = &relay2; rows[3] = &relay3;
            rows[4] = &relay4; rows[5] = &relay5; rows[6] = &relay6; rows[7] = &relay7;
        }

        void update() {
            for (uint8_t i = 0; i < NUMBER_OF_RELAYS; i++) {
                rows[i]->update();
            }
        }

        void show() {
            for (uint8_t i = 0; i < NUMBER_OF_RELAYS; i++) {
                rows[i]->show();
            }
            applySelectionHighlight();
        }

        void selectNext() {
            selectedIndex = (selectedIndex + 1) % NUMBER_OF_RELAYS;
            applySelectionHighlight();
        }

        void selectPrevious() {
            selectedIndex = (selectedIndex == 0) ? (NUMBER_OF_RELAYS - 1) : (selectedIndex - 1);
            applySelectionHighlight();
        }

        void toggleSelected() {
            rows[selectedIndex]->toggle();
        }
};
