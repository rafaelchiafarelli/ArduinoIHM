#ifndef LABEL_H
#define LABEL_H
#include <Element.h>
#include <standardDefinitions.h>
#include "BlinkAnimator.h"

// Ticks-to-toggle for each blink speed (see BlinkAnimator.h) -- smaller is
// faster. Fixed here vs. the original constants: those had "fast" blink
// *slower* than "slow" (40 vs 20 ticks) and "very fast" at the same rate as
// "slow" -- an ordering bug, not intentional timing. These three are now
// genuinely distinct, decreasing speeds.
#define BLINK_THRESHOLD_SLOW 80
#define BLINK_THRESHOLD_FAST 40
#define BLINK_THRESHOLD_VERY_FAST 20
#define BLINK_THRESHOLD_BACKGROUND 30

typedef enum {
    LABEL_OFF,
    LABEL_ON,
    LABEL_BLINK_SLOW,
    LABEL_BLINK_FAST,
    LABEL_BLINK_VERY_FAST,
    LABEL_BLINK_BACKGROUND, // inverted colors, alternating
    LABEL_HIGHLIGHTED       // solid inverted (selected, non-blinking)
} LabelState;

class Label : public Element{
private:
    BlinkAnimator blink;
    bool isShown = false; // gates the one-shot states (OFF/ON/HIGHLIGHTED)
    LabelState labelState = LABEL_OFF;

    uint8_t wide() {return strlen(label)*width*6;};
    uint8_t large(){return width*8;};

    void clear(){ tft->fillRect(x, y, wide(), large(), BLACK); }

    void drawLabel(){
        tft->setTextColor(WHITE);
        tft->setTextSize(width);
        tft->setCursor(x, y);

        switch (labelState)
        {
        case LABEL_OFF:
            if(isShown) return;
            clear();
            isShown = true;
            break;
        case LABEL_ON:
            if(isShown) return;
            clear();
            tft->println(label);
            isShown = true;
            break;
        case LABEL_HIGHLIGHTED:
            if(isShown) return;
            tft->fillRect(x, y, wide(), large(), WHITE);
            tft->setTextColor(BLACK);
            tft->println(label);
            isShown = true;
            break;
        case LABEL_BLINK_SLOW:
        case LABEL_BLINK_FAST:
        case LABEL_BLINK_VERY_FAST: {
            uint8_t threshold = labelState == LABEL_BLINK_SLOW ? BLINK_THRESHOLD_SLOW
                               : labelState == LABEL_BLINK_FAST ? BLINK_THRESHOLD_FAST
                               : BLINK_THRESHOLD_VERY_FAST;
            if(blink.tick(threshold)){
                if(blink.on) tft->println(label); else clear();
            }
            break;
        }
        case LABEL_BLINK_BACKGROUND:
            if(blink.tick(BLINK_THRESHOLD_BACKGROUND)){
                if(blink.on){
                    tft->fillRect(x, y, wide(), large(), WHITE);
                    tft->setTextColor(BLACK);
                } else {
                    clear();
                    tft->setTextColor(WHITE);
                }
                tft->println(label);
            }
            break;
        }
    }
public:
    Label(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state,const char *ldl, uint8_t l,Display *tft):Element(tft){
        this->x = x;
        this->y = y;
        this->width = w;
        this->height = h;
        this->labelState = (LabelState)state;
        strncpy(this->label,ldl,LABEL_STRING_SIZE);
        this->location = l;
    }

    ~Label(){}

    void setLabel(const char* lbl){
        strncpy(label, lbl,LABEL_STRING_SIZE);
        isShown = false; //force redraw
    }
    void setState(int state){
        labelState = (LabelState)state;
        isShown = false; //force redraw
        blink.reset(false);
    }
    void update(){
        drawLabel();
    }
};
#endif // LABEL_H
