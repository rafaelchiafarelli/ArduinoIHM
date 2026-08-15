#ifndef TABSELECTOR_H
#define TABSELECTOR_H

#include <standardDefinitions.h>
#include "Label.h"
#define NUMBER_OF_BUTTONS 4
#define TABSELECTOR_WIDTH 318
#define TABSELECTOR_HEIGHT 459
#define TABSELECTOR_HEADER_SIZE 25
#define TABSELECTOR_FONT_SIZE 2
#define TABSELECTOR_LED_SIZE 10
#define TABSELECTOR_HEADER_TEXT_PADDING 5
#define TABSELECTOR_HEADER_LATERAL_PADDING 5

// Per-tab column width. Same numeric values as before this cleanup (46/90/
// 110) -- only how they combine into label/divider x-positions changed:
// every call site below uses one of the _OFFSET constants (computed once,
// here) instead of re-summing widths by hand at each use.
#define TABSELECTOR_PWM_WIDTH 46
#define TABSELECTOR_SERIAL_WIDTH 90
#define TABSELECTOR_OUTPUT_WIDTH 110

#define TABSELECTOR_SERIAL_OFFSET (TABSELECTOR_PWM_WIDTH)
#define TABSELECTOR_OUTPUT_OFFSET (TABSELECTOR_PWM_WIDTH + TABSELECTOR_SERIAL_WIDTH)
#define TABSELECTOR_END_OFFSET (TABSELECTOR_PWM_WIDTH + TABSELECTOR_SERIAL_WIDTH + TABSELECTOR_OUTPUT_WIDTH)

/**
 * This Element is the background for all elements (will be the selection button)
 */

typedef enum {
    PWM_SELECTED,
    SERIAL_SELECTED,
    OUPTUT_SELECTED
}SelectedOption;

class TabSelector: public Element {
    private:
        bool started = false;
        SelectedOption curSelectedoption = PWM_SELECTED;
        Label pwmConf;
        Label serialConf;
        Label multiOuputConf;
    public:
        TabSelector(uint16_t x0, uint16_t y0, Display *tft):Element(tft),
                    // Initial state is LABEL_BLINK_BACKGROUND (matches the
                    // original numeric value 5, not 6/LABEL_HIGHLIGHTED which
                    // every later selectCurrTab()/selectNext()/selectBefore()
                    // call uses) -- the default-selected tab blinks briefly on
                    // startup before settling once any encoder input happens.
                    pwmConf(x0+TABSELECTOR_HEADER_TEXT_PADDING,y0+TABSELECTOR_HEADER_TEXT_PADDING,TABSELECTOR_FONT_SIZE,TABSELECTOR_LED_SIZE,LABEL_BLINK_BACKGROUND,(const char *)"PWM",0,tft),
                    serialConf(x0+TABSELECTOR_HEADER_TEXT_PADDING+TABSELECTOR_SERIAL_OFFSET+TABSELECTOR_HEADER_LATERAL_PADDING, y0+TABSELECTOR_HEADER_TEXT_PADDING,TABSELECTOR_FONT_SIZE,TABSELECTOR_LED_SIZE,LABEL_ON,(const char *)"SERIAL",0,tft ),
                    multiOuputConf(x0+TABSELECTOR_HEADER_TEXT_PADDING+TABSELECTOR_OUTPUT_OFFSET+TABSELECTOR_HEADER_LATERAL_PADDING,y0+TABSELECTOR_HEADER_TEXT_PADDING,TABSELECTOR_FONT_SIZE,TABSELECTOR_LED_SIZE,LABEL_ON,(const char *)"Output",0,tft)
        {
            x = x0;
            y = y0;
        }

        void update(){
            if(!started) {
                //DRAW HEADER
                tft->drawFastHLine(0,y+TABSELECTOR_HEADER_SIZE,TABSELECTOR_WIDTH,WHITE);

                //DRAW LATERAL LINES
                tft->drawFastVLine(x,y,TABSELECTOR_HEIGHT,WHITE);
                tft->drawFastVLine(TABSELECTOR_WIDTH,y,TABSELECTOR_HEIGHT-y,WHITE);

                //DRAW TAB SELECTION
                tft->drawFastVLine(TABSELECTOR_SERIAL_OFFSET,y,TABSELECTOR_HEADER_SIZE,WHITE);
                tft->drawFastVLine(TABSELECTOR_OUTPUT_OFFSET,y,TABSELECTOR_HEADER_SIZE,WHITE);
                tft->drawFastVLine(TABSELECTOR_END_OFFSET,y,TABSELECTOR_HEADER_SIZE,WHITE);

            }
            started = true;
            pwmConf.update();
            serialConf.update();
            multiOuputConf.update();
        }
        SelectedOption getCurrentSelected(){return curSelectedoption;}

        void selectCurrTab(){
            switch (curSelectedoption)
            {
            case PWM_SELECTED:
                pwmConf.setState(LABEL_HIGHLIGHTED);
                break;
            case SERIAL_SELECTED:
                serialConf.setState(LABEL_HIGHLIGHTED);
                break;
            case OUPTUT_SELECTED:
                multiOuputConf.setState(LABEL_HIGHLIGHTED);
                break;
            default:
                break;
            }
        }
        void selectNext(){
            switch (curSelectedoption)
            {
            case PWM_SELECTED:
                curSelectedoption = SERIAL_SELECTED;
                pwmConf.setState(LABEL_ON);
                serialConf.setState(LABEL_HIGHLIGHTED);
                multiOuputConf.setState(LABEL_ON);
                break;
            case SERIAL_SELECTED:
                curSelectedoption = OUPTUT_SELECTED;
                pwmConf.setState(LABEL_ON);
                serialConf.setState(LABEL_ON);
                multiOuputConf.setState(LABEL_HIGHLIGHTED);
                break;
            case OUPTUT_SELECTED:
                curSelectedoption = PWM_SELECTED;
                pwmConf.setState(LABEL_HIGHLIGHTED);
                serialConf.setState(LABEL_ON);
                multiOuputConf.setState(LABEL_ON);
                break;
            default:
                break;
            }
        }

        void selectBefore(){
            switch (curSelectedoption)
            {
            case PWM_SELECTED:
                curSelectedoption = OUPTUT_SELECTED;
                pwmConf.setState(LABEL_ON);
                serialConf.setState(LABEL_ON);
                multiOuputConf.setState(LABEL_HIGHLIGHTED);
                break;
            case SERIAL_SELECTED:
                curSelectedoption = PWM_SELECTED;
                pwmConf.setState(LABEL_HIGHLIGHTED);
                serialConf.setState(LABEL_ON);
                multiOuputConf.setState(LABEL_ON);
                break;
            case OUPTUT_SELECTED:
                curSelectedoption = SERIAL_SELECTED;
                pwmConf.setState(LABEL_ON);
                serialConf.setState(LABEL_HIGHLIGHTED);
                multiOuputConf.setState(LABEL_ON);
                break;
            default:
                break;
            }
        }
};

#endif // TABSELECTOR_H
