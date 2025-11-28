#ifndef TABSELECTOR_H
#define TABSELECTOR_H

#include <standardDefinitions.h>
#include "Label.h"
#define NUMBER_OF_BUTTONS 4
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
        int16_t w,h; //box size - ALWAYS START FROM 0,25 (25 pixels high)
    public:
        TabSelector(Display *tft):Element(TAB_SELECTOR,tft), 
                    pwmConf(3,25,2,10,5,(const char *)"PWM",0,tft),
                    serialConf(50, 25,2,10,1,(const char *)"SERIAL",0,tft ),
                    multiOuputConf(140,25,2,10,1,(const char *)"Output",0,tft)
        {
            w = 320;
            h = 479;
        }

        virtual Element setLabel(const char* label){ return *this; }
        virtual Element setLocation(int location){ return *this; }
        virtual Element setPosition(int x, int y){ return *this; }
        virtual Element setSize(int width, int height){ return *this; }    
        virtual Element setState(int state){ 
            
            return *this; 
        }
        virtual Element update(){ 
            if(!started) {
                tft->drawFastHLine(0,45,w,WHITE);
                tft->drawFastHLine(0,478,w,WHITE);
                tft->drawFastVLine(1,20,h-20,WHITE);
                tft->drawFastVLine(w-2,20,h-20,WHITE);

                tft->drawFastVLine(46,20,25,WHITE);
                tft->drawFastVLine(136,20,25,WHITE);
                tft->drawFastVLine(246,20,25,WHITE);

            }
            started = true;
            pwmConf.update();
            serialConf.update();
            multiOuputConf.update();
            return *this; 
        }
        SelectedOption getCurrentSelected(){return curSelectedoption;}
        void selectNext(){
            switch (curSelectedoption)
            {
            case PWM_SELECTED:
                /* code */
                curSelectedoption = SERIAL_SELECTED;
                pwmConf.setState(1);
                serialConf.setState(5);
                multiOuputConf.setState(1);
                break;
            case SERIAL_SELECTED:
                /* code */
                curSelectedoption = OUPTUT_SELECTED;
                pwmConf.setState(1);
                serialConf.setState(1);
                multiOuputConf.setState(5);
                break;
            case OUPTUT_SELECTED:
                /* code */
                curSelectedoption = PWM_SELECTED;
                pwmConf.setState(5);
                serialConf.setState(1);
                multiOuputConf.setState(1);
                break;                            
            default:
                break;
            }
        }
    private:
   
};

#endif // TABSELECTOR_H