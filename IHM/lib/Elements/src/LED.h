#ifndef _LED_H_
#define _LED_H_

#include <Element.h>
#define BLINKING_LED_THRESHOLD 80
#define BLINKING_LED_FAST_THRESHOLD 40
#define BLINKING_LED_VERY_FAST_THRESHOLD 20


class LED : public Element {
private:
    uint8_t blinkCounter = 0;
    uint8_t blinkState = 0;
    VisibilityControl visibilityControl = IDLE;
     //text size 
    uint8_t wide() {return strlen(label)*width*6;};
    uint8_t large(){return width*8;};

    void drawLED() {
        // Implement LED drawing logic here
            //height is radius
            //width is text size
        tft->setTextColor(WHITE);
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
            if(blinkCounter++ >= BLINKING_LED_THRESHOLD){
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
            if(blinkCounter++ >= BLINKING_LED_FAST_THRESHOLD){
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
            if(blinkCounter++ >= BLINKING_LED_VERY_FAST_THRESHOLD){
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

        if(visibilityControl == VISIBLE) {
            tft->setTextSize(width);
            tft->setTextColor(WHITE);
            switch (location)
            {
            case top:
                //top
                tft->setCursor(x-wide()/2, y-large()-height*2);
                break;
            case bottom:
                //bottom
                tft->setCursor(x-wide()/2, y+large()+height);
                break;
            case left:
                //left
                tft->setCursor(x-wide()-height*2, y-large()/2);
                break;
            default:
            case right:
                //right
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
    LED(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state,const char *ldl, uint8_t l,Display *tft):Element(LABEL,tft){
        this->x = x;
        this->y = y;
        this->width = w;
        this->height = h;
        this->state = state;
        strncpy(this->label,ldl,LABEL_STRING_SIZE);
        this->location = l;

    }

    ~LED(){}
    Element setLabel(const char* lbl) override {
        strncpy(label,lbl,LABEL_STRING_SIZE);
        return *this;
    }
    Element setLocation(int lcn) override {
        // Implement location setting logic here
        if(lcn==0)
        {
            visibilityControl = GONE;
        }
        else
        {
            visibilityControl = VISIBLE;
            location = (LocationType)lcn;
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