#pragma once
#include <Element.h>
#include <standardDefinitions.h>
#include "Icon.h"
#include "Icons/BattCharging.h"
#include "Icons/BattEmpty.h"
#include "Icons/BattFull.h"


 typedef enum{
    //DebugConnection,
    //RS485Connection,
    //SDCardConnected,
    //SDCardRecording,
    //SDCArdError,
    //CANBus1Connected,
    //CANBus2Connected,
    //CommPortConnected,
    BattEmpty,
    BattCharding,
    BattCharged,
    MAX_NUMBER_OF_ICONS
}IconSelection;



class BattIcons: public Element {
    private:

    IconType_t icons[MAX_NUMBER_OF_ICONS] ={
        {
            .d = batt_full,
            .w = batt_full_w,
            .h = batt_full_h,
            .x = 280,
            .y = 1,
            .state = GONE,
            .isShown = false
        },
        {
            .d = batt_charging,
            .w = batt_charging_w,
            .h = batt_charging_h,
            .x = 280,
            .y = 1,
            .state = GONE,
            .isShown = false
        },
        {
            .d = batt_empty,
            .w = batt_empty_w,
            .h = batt_empty_h,
            .x = 280,
            .y = 1,
            .state = VISIBLE,
            .isShown = false
        }
    };

    public:
    
        BattIcons(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state, Display *tft):Element(ICON,tft)
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