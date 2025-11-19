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
        LED serialConn;
        LED tSlice;
        int x,y,w,h; //box size
        Icon battIcon;
    public:

        StatusBar(const char** tabLabels, uint8_t tbLabelAmount, Display* tft):
                Element(STATUS_BAR,tft),
                serialConn(0,5,2,10,0,(const char *)"LED1",0,tft),
                tSlice(0,5,2,10,0,(const char *)"LED1",0,tft),
                battIcon(tft)
        {

        }

        virtual Element setLabel(const char* label){ return *this; }
        virtual Element setLocation(int location){ return *this; }
        virtual Element setPosition(int x, int y){ return *this; }
        virtual Element setSize(int width, int height){ return *this; }    
        virtual Element setState(int state){ 
            
            return *this; 
        }
        virtual Element update(){ 
            return *this; 
        }
    private:



};

#endif // STATUSBAR_H