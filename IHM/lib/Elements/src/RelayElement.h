#pragma once

#include <Element.h>
#include <Relay.h>
#include <RelayConfig.h>
#include <PWMLabelFormat.h>
#include "LED.h"
#include "Label.h"

#define RELAYELEMENT_WIDTH 314
#define RELAYELEMENT_LED_X 20
#define RELAYELEMENT_LED_RADIUS 15
#define RELAYELEMENT_LED_TEXTSIZE 2
#define RELAYELEMENT_STATUS_X 160
#define RELAYELEMENT_STATUS_TEXTSIZE 2

/**
 * One relay's on-screen row (Relay 0..7). Owns the UI-editable RelayConfig
 * for this relay and applies it live to the real hardware (via the bound
 * Relay* and relayIndex) every time it's toggled -- same "mutate config ->
 * apply to hardware -> refresh display" shape as PWMSimplex::editField
 * (lib/Elements/src/PWMSimplex.h), just without any multi-field editing
 * since a relay is only ever on or off.
 */
class RelayElement : public Element {
    private:
        LED led;
        Label status;

        bool isShown = false;
        bool isSelected = false;

        uint16_t w, h;

        RelayConfig config;
        Relay *relay;
        uint8_t relayIndex;

        char nameBuf[LABEL_STRING_SIZE];

        void refreshDisplay() {
            char buf[PWM_LABEL_BUFFER_SIZE];
            formatShortStatusLabel(buf, config.state);
            status.setLabel(buf);
            led.setState(config.state ? 1 : 0);
        }

    public:
        RelayElement(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Display *tft, Relay *relay, uint8_t relayIndex):
                                            Element(PWM_SELECTION, tft),
                                            led(x + RELAYELEMENT_LED_X, y + h / 2, RELAYELEMENT_LED_TEXTSIZE, RELAYELEMENT_LED_RADIUS, 0, "Rele", 0, tft),
                                            status(x + RELAYELEMENT_STATUS_X, y + h / 2 - 8, RELAYELEMENT_STATUS_TEXTSIZE, h, 1, "Desl.", 1, tft),
                                            relay(relay), relayIndex(relayIndex)
        {
            Element::x = x;
            Element::y = y;
            this->w = w;
            this->h = h;
            snprintf(nameBuf, LABEL_STRING_SIZE, "Rele %u", (unsigned int)relayIndex);
            led.setLabel(nameBuf);
            led.setLocation((int)right);
        }

        Element update() {
            if (!isShown) {
                isShown = true;
                if (isSelected) {
                    tft->fillRect(x, y, RELAYELEMENT_WIDTH, h, DARK_ORANGE);
                } else {
                    tft->fillRect(x, y, RELAYELEMENT_WIDTH, h, BLACK);
                }
                tft->drawFastHLine(x, y + h, RELAYELEMENT_WIDTH, TFT_ORANGE);
                led.setState(config.state ? 1 : 0);
            }
            led.update();
            status.update();
            return *this;
        }

        void show() {
            isShown = false;
            refreshDisplay();
            status.setState(1);
        }

        void setSelected(bool isSelected) {
            this->isSelected = isSelected;
            isShown = false;
        }

        void toggle() {
            config.toggle();
            relay->setRelay(relayIndex, config.state);
            refreshDisplay();
        }
};
