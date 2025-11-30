#ifndef ICON_H
#define ICON_H
#include "Element.h"


typedef struct IconType
{
    /* data */
    const uint16_t *d;
    const uint16_t w;
    const uint16_t h;
    const uint16_t x;
    const uint16_t y;
    VisibilityControl state;
    bool isShown;
}IconType_t;


class Icon: public Element {
    private:
        
        IconType_t *icons;
        uint8_t maxNumberOfIcons;
    public:
        
        Icon(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state, uint8_t nIcons, Display *tft, IconType_t *i):Element(ICON,tft),icons(i)
        {
            maxNumberOfIcons = nIcons;
        }

        Element setLabel(const char* label){ return *this; }
        Element setLocation(int location){ return *this; }
        Element setPosition(int x, int y){ return *this; }
        Element setSize(int width, int height){ return *this; }    
        Element setState(int state){ 
            return *this; 
        }
        void setIconState(VisibilityControl visibilityControl, int id){
            if(id>=maxNumberOfIcons){
                return;
            }
            icons[id].state = visibilityControl;
            icons[id].isShown = false;
        }
        Element update(){ 
            for(int i = 0; i<maxNumberOfIcons; i++)
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