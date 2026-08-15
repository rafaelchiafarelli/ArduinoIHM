#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <standardDefinitions.h>
#include <Widgets.h>
#include <BattIcons.h>

#define NUMBER_OF_BUTTONS 4
#define STATUSBAR_THICKNESS 21
/**
 * This Element is the background for all elements (will be the selection button)
 */

class StatusBar: public Element {
    private:
        bool started = false;
        LED serialConn;
        LED tSlice;
        int x,y,w,h; //box size
        BattIcons battIcon;
    public:

        StatusBar(Display* tft):
                Element(tft),
                serialConn(8,8,2,7,LED_OFF,(const char *)"COM:8N19600",0,tft),
                tSlice(165,8,2,7,LED_OFF,(const char *)"TS:---",0,tft),
                battIcon(250,0,20,20,0,tft)
        {
            serialConn.setLocation((int)right);
            tSlice.setState(LED_BLINK_VERY_FAST);
            tSlice.setLocation((int)right);
            // battIcon has no setLocation -- BattIcons/Icon never had real
            // behavior there (was a no-op override), this call did nothing.
        }

        void update(){
            if(!started)
                tft->drawFastHLine(0,STATUSBAR_THICKNESS,320,WHITE);
            started = true;
            serialConn.update();
            tSlice.update();
            battIcon.update();
        }
        void setBattLevel(uint8_t bLevel){
            if(bLevel>=85){
                battIcon.setIconState(VISIBLE,0);
                battIcon.setIconState(GONE,1);
            }
            if(bLevel<85){
                battIcon.setIconState(GONE,0);
                battIcon.setIconState(VISIBLE,1);
            }

        }
        void setTimeSlice(uint8_t avg){
            char labelMsg[15];
            sprintf(labelMsg, "TS:%d",avg);
            tSlice.setLabel(labelMsg);
        }

};

#endif // STATUSBAR_H
