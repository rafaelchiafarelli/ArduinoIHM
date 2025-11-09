#ifndef _LED_H_
#define _LED_H_

#include <Element.h>
#define BLINKING_THRESHOLD 80
#define BLINKING_FAST_THRESHOLD 40
#define BLINKING_VERY_FAST_THRESHOLD 20
typedef enum{
    VISIBLE,
    GONE,
    IDLE
}LabelControl;

class LED : public Element {
private:
    uint8_t blinkCounter = 0;
    uint8_t blinkState = 0;
    LabelControl labelControl = IDLE;
     //text size 
    uint8_t wide() {return strlen(label)*width*6;};
    uint8_t large(){return width*8;};

    void drawLED() {
        // Implement LED drawing logic here
            //height is radius
            //width is text size
        switch (state)
        {
        case 0:
            /* turned off */
            tft->fillCircle(x, y, height, DARK_GREEN);
            break;
        case 1:
            /* turned on */
            tft->fillCircle(x, y, height, GREEN);
            break;
        case 2:
            /* turned blinking */
            if(blinkCounter++ >= BLINKING_THRESHOLD){
                blinkCounter = 0;
                blinkState = !blinkState;
                if(blinkState){
                    tft->fillCircle(x, y, height, GREEN);
                }else{
                    tft->fillCircle(x, y, height, DARK_GREEN);
                }
            }            
            break;
        case 3:
            /* turned blinking fast */
            if(blinkCounter++ >= BLINKING_FAST_THRESHOLD){
                blinkCounter = 0;
                blinkState = !blinkState;
                if(blinkState){
                    tft->fillCircle(x, y, height, GREEN);
                }else{
                    tft->fillCircle(x, y, height, DARK_GREEN);
                }
            }
            break;                        
        case 4:
            /* turned blinking very fast */
            if(blinkCounter++ >= BLINKING_VERY_FAST_THRESHOLD){
                blinkCounter = 0;
                blinkState = !blinkState;
                if(blinkState){
                    tft->fillCircle(x, y, height, GREEN);
                }else{
                    tft->fillCircle(x, y, height, DARK_GREEN);
                }
            }            
            break;                             
        default:
            break;
        }

        if(labelControl == VISIBLE) {
            tft->setTextSize(width);
            tft->setTextColor(WHITE);
            switch (location)
            {
            case 1:
                //top
                tft->setCursor(x-wide()/2, y-large()-height*2);
                break;
            case 2:
                //bottom
                tft->setCursor(x-wide()/2, y+large()+height);
                break;
            case 3:
                //left
                tft->setCursor(x-wide()-height*2, y-large()/2);
                break;
            case 4:
                //right
                tft->setCursor(x+wide()/2-height, y-large()/2);
                break;
        
            }
            tft->println(label);
            labelControl = IDLE;
        } else if (labelControl == GONE)
        {
            switch (location)
            {
            case 1:
                tft->fillRect(x-wide()/2, y-large()-height*2, wide(), large(), BLACK);
                break;
            case 2:
                tft->fillRect(x-wide()/2, y+large()+height, wide(), large(), BLACK);
                break;
            case 3:
                tft->fillRect(x-wide()-height*2, y-large()/2, wide(), large(), BLACK);
                break;
            case 4:
                tft->fillRect(x+wide()/2-height, y-large()/2, wide(), large(), BLACK);
                break;                                            
            default:
                break;
            }
            location = 0;
            labelControl = IDLE;
        }

    }

public:
    LED(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state,const char *ldl, uint8_t l,Display *tft):Element(LABEL,tft){
        this->x = x;
        this->y = y;
        this->width = w;
        this->height = h;
        this->state = state;
        this->label = ldl;
        this->location = l;

    }

    ~LED(){}
    Element setLabel(const char* lbl) override {
        label = lbl;
        return *this;
    }
    Element setLocation(int lcn) override {
        // Implement location setting logic here
        if(lcn==0)
        {
            labelControl = GONE;
        }
        else
        {
            labelControl = VISIBLE;
            location = lcn;
        }
            
        return *this;
    }
    /**
     * this is not a moving LED
     */
    Element setPosition(int x, int y) override {
        //nothing to do here
        return *this;
    }
    /**
     * @brief Set the size of the LED element.
     * @param width The width (text size) of the LED label.
     * @param height The radius of the LED circle.
    */
    Element setSize(int width, int height) override { 
        // Implement size setting logic here
        this->width = width;
        this->height = height;
        return *this;
    }
    Element setState(int state) override {
        // Implement state setting logic here
        this->state = state;
        return *this;
    }
    Element update() override {
        // Implement LED update logic here
        drawLED();
        return *this;
    }   

};

#endif // _LED_H_