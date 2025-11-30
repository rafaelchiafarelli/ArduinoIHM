#ifndef TABSELECTOR_H
#define TABSELECTOR_H

#include <standardDefinitions.h>
#include "Label.h"
#define NUMBER_OF_BUTTONS 4
#define TABSELECTOR_WIDTH 320
#define TABSELECTOR_HEIGHT 479
#define TABSELECTOR_HEADER_SIZE 25
#define TABSELECTOR_FONT_SIZE 2
#define TABSELECTOR_LED_SIZE 10
#define TABSELECTOR_PWM_OPTION 46
#define TABSELECTOR_SERIAL_OPTION 90
#define TABSELECTOR_MULTIOUT_OPTION 110
#define TABSELECTOR_HEADER_TEXT_PADDING 5
#define TABSELECTOR_HEADER_LATERAL_PADDING 5
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
        TabSelector(uint16_t x0, uint16_t y0, Display *tft):Element(TAB_SELECTOR,tft), 
                    pwmConf(x0+TABSELECTOR_HEADER_TEXT_PADDING,y0+TABSELECTOR_HEADER_TEXT_PADDING,TABSELECTOR_FONT_SIZE,TABSELECTOR_LED_SIZE,5,(const char *)"PWM",0,tft),
                    serialConf(x0+TABSELECTOR_HEADER_TEXT_PADDING+TABSELECTOR_PWM_OPTION+TABSELECTOR_HEADER_LATERAL_PADDING, y0+TABSELECTOR_HEADER_TEXT_PADDING,TABSELECTOR_FONT_SIZE,TABSELECTOR_LED_SIZE,1,(const char *)"SERIAL",0,tft ),
                    multiOuputConf(x0+TABSELECTOR_HEADER_TEXT_PADDING+TABSELECTOR_PWM_OPTION+TABSELECTOR_SERIAL_OPTION+TABSELECTOR_HEADER_LATERAL_PADDING,y0+TABSELECTOR_HEADER_TEXT_PADDING,TABSELECTOR_FONT_SIZE,TABSELECTOR_LED_SIZE,1,(const char *)"Output",0,tft)
        {
            x = x0;
            y = y0;
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
                //DRAW HEADER
                tft->drawFastHLine(0,y+TABSELECTOR_HEADER_SIZE,TABSELECTOR_WIDTH,WHITE);
                
                //DRAW LATERAL LINES
                tft->drawFastVLine(x,y,TABSELECTOR_HEIGHT-y,WHITE);
                tft->drawFastVLine(TABSELECTOR_WIDTH-2,y,TABSELECTOR_HEIGHT-y,WHITE);
                
                //DRAW TAB SELECTION
                tft->drawFastVLine(TABSELECTOR_PWM_OPTION,y,TABSELECTOR_HEADER_SIZE,WHITE);
                tft->drawFastVLine(TABSELECTOR_PWM_OPTION+TABSELECTOR_SERIAL_OPTION,y,TABSELECTOR_HEADER_SIZE,WHITE);
                tft->drawFastVLine(TABSELECTOR_PWM_OPTION+TABSELECTOR_SERIAL_OPTION+TABSELECTOR_MULTIOUT_OPTION,y,TABSELECTOR_HEADER_SIZE,WHITE);

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