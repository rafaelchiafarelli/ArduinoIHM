#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <standardDefinitions.h>
#include <Widgets.h>
#define NUMBER_OF_BUTTONS 4
/**
 * This Element is the background for all elements (will be the selection button)
 */

class StatusBar: public Element {
    private:
        bool started = false;
        LED serialConn;
        LED tSlice;
        int x,y,w,h; //box size
        Icon battIcon;
    public:

        StatusBar(Display* tft):
                Element(STATUS_BAR,tft),
                serialConn(8,8,2,7,0,(const char *)"COM:8N19600",0,tft),
                tSlice(165,8,2,7,0,(const char *)"TS:---",0,tft),
                battIcon(250,2,10,15,tft)
        {
            serialConn.setLocation((int)right);
            tSlice.setState(4);
            tSlice.setLocation((int)right);
            battIcon.setLocation(0);

        }

        virtual Element setLabel(const char* label){ return *this; }
        virtual Element setLocation(int location){ return *this; }
        virtual Element setPosition(int x, int y){ return *this; }
        virtual Element setSize(int width, int height){ return *this; }    
        virtual Element setState(int state){ 
            
            return *this; 
        }
        virtual Element update(){ 
            if(!started)
                tft->drawFastHLine(0,18,320,WHITE);
            started = true;
            serialConn.update();
            tSlice.update();
            battIcon.update();
            return *this; 
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
        
    private:



};

#endif // STATUSBAR_H