#pragma once

#include <Element.h>
#include "LED.h"
#include "Label.h"

#define BUSSTATUSROW_WIDTH 314
#define BUSSTATUSROW_LED_X 20
#define BUSSTATUSROW_LED_RADIUS 15
#define BUSSTATUSROW_LED_TEXTSIZE 2
#define BUSSTATUSROW_STATUS_X 100
#define BUSSTATUSROW_STATUS_TEXTSIZE 2

// How many update() calls a "just changed" blink pulse lasts before the LED
// settles back to a solid on/off -- same idea as LED_BLINK_FAST's own
// threshold (lib/Elements/src/LED.h), just how long the *pulse itself* runs.
#define BUSSTATUSROW_BLINK_HOLD_TICKS 60

/**
 * One row of the SERIAL/bus-status screen (BusStatusScreen.h): CAN0, CAN1,
 * or RS-485. Read-only -- see BusStatusScreen.h for why. Same "LED + one
 * summary Label" shape as RelayElement.h's relay rows.
 */
class BusStatusRow : public Element {
    private:
        LED led;
        Label status;
        bool isShown = false;
        bool enabled = false;
        uint16_t h;
        uint8_t blinkTicksRemaining = 0;

    public:
        BusStatusRow(uint16_t x, uint16_t y, uint16_t h, const char *rowName, Display *tft):
            Element(tft),
            led(x + BUSSTATUSROW_LED_X, y + h / 2, BUSSTATUSROW_LED_TEXTSIZE, BUSSTATUSROW_LED_RADIUS, LED_OFF, rowName, 0, tft),
            status(x + BUSSTATUSROW_STATUS_X, y + h / 2 - 8, BUSSTATUSROW_STATUS_TEXTSIZE, h, LABEL_ON, "no config", 1, tft)
        {
            // led/status above are built from the constructor *parameter*
            // h (in scope for the whole init-list), not this member -- same
            // "parameter shadows same-named member, member set afterward in
            // the body" shape RelayElement.h already uses, kept consistent
            // rather than introducing a differently-ordered member list here.
            Element::x = x;
            Element::y = y;
            this->h = h;
            led.setLocation((int)right);
        }

        // summary must already be formatted to fit LABEL_STRING_SIZE (see
        // BusStatusScreen's formatCanSummary/formatRs485Summary). enabled
        // decides the LED's steady state; justChanged starts a brief blink
        // pulse that settles to that steady state once it runs out.
        void setSummary(const char *summary, bool enabled, bool justChanged) {
            status.setLabel(summary);
            this->enabled = enabled;
            if (justChanged) {
                blinkTicksRemaining = BUSSTATUSROW_BLINK_HOLD_TICKS;
                led.setState(LED_BLINK_FAST);
            }
        }

        void update() {
            if (!isShown) {
                isShown = true;
                tft->drawFastHLine(x, y + h, BUSSTATUSROW_WIDTH, TFT_ORANGE);
                led.setState(enabled ? LED_ON : LED_OFF);
            }
            if (blinkTicksRemaining > 0 && --blinkTicksRemaining == 0) {
                led.setState(enabled ? LED_ON : LED_OFF); // pulse over, settle
            }
            led.update();
            status.update();
        }

        void show() {
            isShown = false;
        }
};
