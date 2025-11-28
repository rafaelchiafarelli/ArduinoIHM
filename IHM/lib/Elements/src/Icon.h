#ifndef ICON_H
#define ICON_H
#include <standardDefinitions.h>
#include <BattIcons.h>
#define NUMBER_OF_BUTTONS 4
/**
 * This Element is the background for all elements (will be the selection button)
 */

 typedef enum{
    //DebugConnection,
    //RS485Connection,
    //SDCardConnected,
    //SDCardRecording,
    //SDCArdError,
    //CANBus1Connected,
    //CANBus2Connected,
    //CommPortConnected,
    //BattConnected,
    BattCharding,
    BattCharged,
    MAX_NUMBER_OF_ICONS
}IconSelection;

typedef struct 
{
    /* data */
    const uint16_t *d;
    const uint8_t w;
    const uint8_t h;
    const uint16_t x;
    const uint16_t y;
    VisibilityControl state;
    bool isShown;
}IconType;


class Icon: public Element {
    private:

    IconType icons[MAX_NUMBER_OF_ICONS] ={
        {
            .d = batt_full,
            .w = batt_full_w,
            .h = batt_full_h,
            .x = 280,
            .y = 1,
            .state = 0,
            .isShown = false
        },
        {
            .d = batt_charging,
            .w = batt_charging_w,
            .h = batt_charging_h,
            .x = 280,
            .y = 1,
            .state = 0,
            .isShown = false
        }
    };

    public:
        Icon(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Display *tft):Element(ICON,tft)
        {

        }

        virtual Element setLabel(const char* label){ return *this; }
        virtual Element setLocation(int location){ return *this; }
        virtual Element setPosition(int x, int y){ return *this; }
        virtual Element setSize(int width, int height){ return *this; }    
        virtual Element setState(int state){ 
            return *this; 
        }
        void setIconState(VisibilityControl visibilityControl, int id){
            if(id>=MAX_NUMBER_OF_ICONS){
                return;
            }
            icons[id].state = visibilityControl;
            icons[id].isShown = false;
        }
        virtual Element update(){ 
            for(int i = 0; i<MAX_NUMBER_OF_ICONS; i++)
            {
                if(icons[i].state == VISIBLE)
                {
                    if(!icons[i].isShown)
                        {
                            icons[i].isShown = true;
                            tft->drawRGBBitmap(icons[i].x,icons[i].y,icons[i].d,icons[i].w,icons[i].h);
                        }
                }
                
            }

            return *this; 
        }
    private:



};

#endif /* ICON_H */