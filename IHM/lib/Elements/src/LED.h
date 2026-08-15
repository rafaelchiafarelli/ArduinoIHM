#ifndef _LED_H_
#define _LED_H_

#include <Element.h>
#include "BlinkAnimator.h"

#define LED_BLINK_THRESHOLD_SLOW 80
#define LED_BLINK_THRESHOLD_FAST 40
#define LED_BLINK_THRESHOLD_VERY_FAST 20

typedef enum {
    LED_OFF,
    LED_ON,
    LED_BLINK_SLOW,
    LED_BLINK_FAST,
    LED_BLINK_VERY_FAST
} LEDState;

class LED : public Element {
private:
    BlinkAnimator blink;
    LEDState ledState = LED_OFF;
    bool isShown = false; // gates the solid circle redraw (LED_OFF/LED_ON only)
    VisibilityControl visibilityControl = IDLE;

    uint8_t wide() {return strlen(label)*width*6;};
    uint8_t large(){return width*8;};

    void drawCircle(bool on){
        tft->fillCircle(x, y, height, on ? GREEN : DARK_GREEN);
    }

    void drawLED() {
        // height is radius, width is text size (of the optional side-label)
        switch (ledState)
        {
        case LED_OFF:
            if(!isShown){ drawCircle(false); isShown = true; }
            break;
        case LED_ON:
            if(!isShown){ drawCircle(true); isShown = true; }
            break;
        case LED_BLINK_SLOW:
        case LED_BLINK_FAST:
        case LED_BLINK_VERY_FAST: {
            uint8_t threshold = ledState == LED_BLINK_SLOW ? LED_BLINK_THRESHOLD_SLOW
                               : ledState == LED_BLINK_FAST ? LED_BLINK_THRESHOLD_FAST
                               : LED_BLINK_THRESHOLD_VERY_FAST;
            if(blink.tick(threshold)) drawCircle(blink.on);
            break;
        }
        }

        if(visibilityControl == VISIBLE) {
            tft->setTextSize(width);
            tft->setTextColor(WHITE);
            switch (location)
            {
            case top:
                tft->setCursor(x-wide()/2, y-large()-height*2);
                break;
            case bottom:
                tft->setCursor(x-wide()/2, y+large()+height);
                break;
            case left:
                tft->setCursor(x-wide()-height*2, y-large()/2);
                break;
            default:
            case right:
                tft->setCursor(x+height*2+1, y-large()/2);
                break;
            }
            tft->println(label);
            visibilityControl = IDLE;
        } else if (visibilityControl == GONE)
        {
            switch (location)
            {
            case top: //above
                tft->fillRect(x-wide()/2, y-large()-height*2, wide(), large(), BLACK);
                break;
            case bottom: //bellow
                tft->fillRect(x-wide()/2, y+large()+height, wide(), large(), BLACK);
                break;
            case left:
                tft->fillRect(x-wide()-height*2, y-large()/2, wide(), large(), BLACK);
                break;
            case right:
                tft->fillRect(x+wide()/2-height, y-large()/2, wide(), large(), BLACK);
                break;
            default:
                break;
            }
            location = 0;
            visibilityControl = IDLE;
        }
    }

public:
    LED(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state,const char *ldl, uint8_t l,Display *tft):Element(tft){
        this->x = x;
        this->y = y;
        this->width = w;
        this->height = h;
        this->ledState = (LEDState)state;
        strncpy(this->label,ldl,LABEL_STRING_SIZE);
        this->location = l;
    }

    ~LED(){}

    void setLabel(const char* lbl){
        strncpy(label,lbl,LABEL_STRING_SIZE);
    }
    // Real behavior, unlike most other widgets' setLocation: lcn==0 hides
    // the side-label (visibilityControl = GONE, one-shot erase), any other
    // value shows it at that LocationType (top/bottom/left/right).
    void setLocation(int lcn){
        if(lcn==0)
        {
            visibilityControl = GONE;
        }
        else
        {
            visibilityControl = VISIBLE;
            location = (LocationType)lcn;
        }
    }
    void setState(int state){
        ledState = (LEDState)state;
        isShown = false;
        blink.reset(false);
    }
    void update(){
        drawLED();
    }

};

#endif // _LED_H_
