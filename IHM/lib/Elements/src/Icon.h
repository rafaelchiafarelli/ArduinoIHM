#ifndef ICON_H
#define ICON_H
#include <standardDefinitions.h>

#define NUMBER_OF_BUTTONS 4
/**
 * This Element is the background for all elements (will be the selection button)
 */

 typedef enum{
    DebugConnection,
    RS485Connection,
    SDCardConnected,
    SDCardRecording,
    SDCArdError,
    CANBus1Connected,
    CANBus2Connected,
    CommPortConnected,
    BattConnected,
    BattCharding,
    BattCharged
}IconSelection;

class Icon: public Element {
    private:
        uint16_t x,y,w,h; //box size
        uint8_t currentState,lastState;

    public:
        Icon(Display *tft):Element(ICON,tft)
        {

        }

        virtual Element setLabel(const char* label){ return *this; }
        virtual Element setLocation(int location){ return *this; }
        virtual Element setPosition(int x, int y){ return *this; }
        virtual Element setSize(int width, int height){ return *this; }    
        virtual Element setState(int state){ 
            currentState = state;

            return *this; 
        }
        virtual Element update(){ 
            if(lastState != currentState){
                lastState = currentState;
                //do the update
                switch(lastState){
                    case 0:
                    //put the code of the icon 0 here.
                    break;
                    case 1:
                    //put the code of the icon 1 here.
                    break;
                    case 2:
                    //put the code of the icon 2 here.
                    break;                                        
                }
            }
            return *this; 
        }
    private:



};

#endif /* ICON_H */