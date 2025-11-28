#ifndef LABEL_H
#define LABEL_H
#include <Element.h>
#include <standardDefinitions.h>

#define BLINKING_THRESHOLD 20
#define BLINKING_FAST_THRESHOLD 40
#define BLINKING_VERY_FAST_THRESHOLD 20
#define BACKBROUND_BLINK_THRESHOLD 30

class Label : public Element{
private:
    uint8_t blinkCounter = 0;
    uint8_t blinkState = 0;
    bool isShown = false;
     //text size 
    uint8_t wide() {return strlen(label)*width*6;};
    uint8_t large(){return width*8;};


    void drawLabel(){

        tft->setTextColor(WHITE);
        tft->setTextSize(width);
        tft->setCursor(x, y);
        if(isShown)
            return;
        switch (state)
        {
        case 0:
            /* turned off */

            tft->fillRect(x, y, wide(), large(), BLACK);
            isShown = true;
            break;
        case 1:
            /* turned on */
            tft->fillRect(x, y, wide(), large(), BLACK);
            tft->println(label);
            isShown = true;
            break;
        case 2:
            /* turned blinking */
            if(blinkCounter++ >= BLINKING_THRESHOLD){
                blinkCounter = 0;
                blinkState = !blinkState;
                if(blinkState){
                    tft->println(label);
                }else{
                    tft->fillRect(x, y, wide(), large(), BLACK);
                }
            }            
            break;
        case 3:
            /* turned blinking fast */
            if(blinkCounter++ >= BLINKING_FAST_THRESHOLD){
                blinkCounter = 0;
                blinkState = !blinkState;
                if(blinkState){
                    tft->println(label);
                }else{
                    tft->fillRect(x, y, wide(), large(), BLACK);
                }
            }
            break;                        
        case 4:
            /* turned blinking very fast */
            if(blinkCounter++ >= BLINKING_VERY_FAST_THRESHOLD){
                blinkCounter = 0;
                blinkState = !blinkState;
                if(blinkState){
                    tft->println(label);
                }else{
                    tft->fillRect(x, y, wide(), large(), BLACK);
                }
            }            
            break;                             
        case 5:
            /* turned blinking very fast */
            blinkCounter++;
            if(blinkCounter >= BACKBROUND_BLINK_THRESHOLD){
                blinkCounter = 0;
                blinkState = !blinkState;
                if(blinkState){
                    tft->fillRect(x, y, wide(), large(), WHITE);
                    tft->setTextColor(BLACK);
                    tft->println(label);
                }else{
                    tft->fillRect(x, y, wide(), large(), BLACK);
                    tft->setTextColor(WHITE);
                    tft->println(label);
                }
            }            
            break;               
        default:
            break;
        }

    }
public:
    Label(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state,const char *ldl, uint8_t l,Display *tft):Element(INDICATOR,tft){
        this->x = x;
        this->y = y;
        this->width = w;
        this->height = h;
        this->state = state;
        strncpy(this->label,ldl,LABEL_STRING_SIZE);
        this->location = l;
    }

    ~Label(){}
    
    Element setLabel(char* lbl) override {
        strncpy(label, lbl,LABEL_STRING_SIZE);
        isShown = false; //force redraw
        return *this;
    }
    Element setLocation(int location) override {
        // Implement location setting logic here
        return *this;
    }
    Element setPosition(int x, int y) override {
        // Implement position setting logic here
        return *this;
    }
    Element setSize(int width, int height) override { 
        // Implement size setting logic here
        return *this;
    }
    Element setState(int state) override {
        // Implement state setting logic here
        this->state = state;
        isShown = false; //force redraw
        return *this;
    }
    Element update() override {
        // Implement label update logic here
        drawLabel();
        return *this;
    }
};
#endif // LABEL_H
