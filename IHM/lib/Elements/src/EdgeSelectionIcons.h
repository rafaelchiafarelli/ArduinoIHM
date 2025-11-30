#pragma once
#include <Element.h>
#include <standardDefinitions.h>
#include "Icon.h"
#include "Icons/EdgeFalling.h"
#include "Icons/EdgeRising.h"



typedef enum{
    EdgeFallingIcon,
    EdgeRisingIcon
}EdgeSelection;



class EdgeSelectionIcons: public Icon {
    private:

    IconType_t edgeIcons[2] = {
                {
                    .d = edge_falling,
                    .w = edge_falling_w,
                    .h = edge_falling_h,
                    .x = 280,
                    .y = 1,
                    .state = GONE,
                    .isShown = false
                },
                {
                    .d = edge_rising,
                    .w = edge_rising_w,
                    .h = edge_rising_h,
                    .x = 280,
                    .y = 1,
                    .state = GONE,
                    .isShown = false
                }};
    public:
    
        EdgeSelectionIcons(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state, Display *tft):Icon(x,y,w,h,state,2,tft,edgeIcons)
        {
            
        }

    private:



};