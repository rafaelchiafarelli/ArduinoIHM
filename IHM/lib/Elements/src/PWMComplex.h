#pragma once

#include <standardDefinitions.h>
#include <EdgeSelectionIcons.h>
#include <Element.h>

class PWMComplex: public Element {
    private:
        EdgeSelectionIcons edgeSelection;
    public:
    PWMComplex(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state, Display *tft):Element(ICON,tft), edgeSelection(x,y,w,h,state,tft){}
    void show(){}
};