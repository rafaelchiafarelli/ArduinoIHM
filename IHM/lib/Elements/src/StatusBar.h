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

        StatusBar(Display* tft):
                Element(STATUS_BAR,tft),
                serialConn(2,2,2,10,0,(const char *)"COM",0,tft),
                tSlice(30,2,2,10,0,(const char *)"TS:",0,tft),
                battIcon(tft)
        {
            serialConn.setLocation(0);
            tSlice.setLocation(0);
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
            serialConn.update();
            tSlice.update();
            battIcon.update();
            return *this; 
        }
    private:



};

#endif // STATUSBAR_H