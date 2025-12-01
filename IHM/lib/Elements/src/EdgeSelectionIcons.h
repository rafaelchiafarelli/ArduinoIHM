#pragma once
#include <Element.h>
#include <standardDefinitions.h>
#include "Icon.h"
#include "Icons/EdgeFalling.h"
#include "Icons/EdgeRising.h"

#define HORIZONTAL_PADDING 10
#define VERTICAL_PADDING 2

typedef enum{
    EdgeFallingIcon,
    EdgeRisingIcon
}EdgeSelection;



class EdgeSelectionIcons: public Icon {
    private:

    IconType_t edgeIcons[2];
    public:
    
        EdgeSelectionIcons(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state, Display *tft):Icon(x,y,w,h,state,2,tft,edgeIcons),edgeIcons({
                {
                    .d = edge_falling,
                    .w = edge_falling_w,
                    .h = edge_falling_h,
                    .x = x+HORIZONTAL_PADDING,
                    .y = y+VERTICAL_PADDING,
                    .state = VISIBLE,
                    .isShown = false
                },
                {
                    .d = edge_rising,
                    .w = edge_rising_w,
                    .h = edge_rising_h,
                    .x = x+HORIZONTAL_PADDING,
                    .y = y+VERTICAL_PADDING,
                    .state = GONE,
                    .isShown = false
                }})
        {

            
        }

    private:



};