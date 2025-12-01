#pragma once

#include <standardDefinitions.h>
#include <Element.h>
#include <EdgeSelectionIcons.h>

#define PWMSIMPLEX_HEIGHT 84
#define PWMSIMPLEX_WIDTH 315


#define EDGESELECTION_X0 2
#define EDGESELECTION_Y0 2
#define EDGESELECTION_STATE 1
#define EDGESELECTION_HEIGHT 52


#define DUDYCYCLE_X0 10
#define DUDYCYCLE_Y0 60
#define DUDYCYCLE_FONT_SIZE 2
#define DUDYCYCLE_STATE 1

#define FREQMODE_X0 75
#define FREQMODE_Y0 5
#define FREQMODE_FONT_SIZE 2
#define FREQMODE_STATE 1

#define FREQUENCY_X0 75
#define FREQUENCY_Y0 30
#define FREQUENCY_FONT_SIZE 2
#define FREQUENCY_STATE 1

#define STATUS_X0 75
#define STATUS_Y0 62
#define STATUS_FONT_SIZE 2
#define STATUS_STATE 5

#define FIRST_SECCION_WIDTH 70
#define FIRST_SECCION_ICON_HEIGHT 54

#define SECOND_SESSION_WIDTH 246
#define SECOND_SESSION_FIRSTH_THIRD 27
#define SECOND_SESSION_SECOND_THIRD 52


class PWMSimplex: public Element {
    private:
        EdgeSelectionIcons edgeSelection;    
        Label dutyCycle;
        Label freqMode;
        Label frequency;
        Label status;
        
        bool isShown = false;

        uint8_t currEdgeSelected = 0;
        uint16_t w,h;
    public:
        PWMSimplex(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state, Display *tft):
                                            Element(PWM_SELECTION,tft), 
                                            edgeSelection(x+EDGESELECTION_X0,y+EDGESELECTION_Y0,w,h,EDGESELECTION_STATE,tft),
                                            dutyCycle(x+DUDYCYCLE_X0,y+DUDYCYCLE_Y0,DUDYCYCLE_FONT_SIZE,h,DUDYCYCLE_STATE,"---%",1,tft),
                                            freqMode(x+FREQMODE_X0,y+FREQMODE_Y0,FREQUENCY_FONT_SIZE,h,FREQMODE_STATE,"Mode:variavel.",1,tft),
                                            frequency(x+FREQUENCY_X0,y+FREQUENCY_Y0,FREQUENCY_FONT_SIZE,h,FREQMODE_STATE,"f:----------Hz",1,tft),
                                            status(x+STATUS_X0,y+STATUS_Y0,STATUS_FONT_SIZE,h,STATUS_STATE,"Status:Inativo",1,tft)
        {
            Element::x = x;
            Element::y = y;
            this->w = w;
            this->h = h;
            Element::state = state;
        }

        Element update(){
            if(!isShown){
                isShown=true;
                //OUTER-BOX
                tft->drawFastHLine(x,y,PWMSIMPLEX_WIDTH,TFT_ORANGE);
                tft->drawFastHLine(x,y+PWMSIMPLEX_HEIGHT,PWMSIMPLEX_WIDTH,TFT_ORANGE);
                tft->drawFastVLine(x,y,PWMSIMPLEX_HEIGHT,TFT_ORANGE);
                tft->drawFastVLine(x+PWMSIMPLEX_WIDTH,y,PWMSIMPLEX_HEIGHT,TFT_ORANGE);
                //FIRST SECCION
                tft->drawFastVLine(x+FIRST_SECCION_WIDTH,y,PWMSIMPLEX_HEIGHT,TFT_ORANGE);
                tft->drawFastHLine(x,y+FIRST_SECCION_ICON_HEIGHT,FIRST_SECCION_WIDTH,TFT_ORANGE);
                
                //SECOND SECCION DIVISIONS
                tft->drawFastHLine(x+FIRST_SECCION_WIDTH,y+SECOND_SESSION_FIRSTH_THIRD,SECOND_SESSION_WIDTH,TFT_ORANGE);
                
                tft->drawFastHLine(x+FIRST_SECCION_WIDTH,y+SECOND_SESSION_SECOND_THIRD,SECOND_SESSION_WIDTH,TFT_ORANGE);

                edgeSelection.update();
                dutyCycle.update();
                freqMode.update();
                frequency.update();
                
            }
            status.update();
            return *this;
        }
        void show(){
            isShown = false;
            edgeSelection.setIconState(VISIBLE,currEdgeSelected);
            dutyCycle.setLabel("---%");
            freqMode.setLabel("Mode:variavel.");
            frequency.setLabel("f:----------Hz");
            status.setLabel("Status:Inativo");
            status.setState(5);
        }
};