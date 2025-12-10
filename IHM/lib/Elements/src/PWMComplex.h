#pragma once

#include <standardDefinitions.h>
#include <EdgeSelectionIcons.h>
#include <Element.h>
typedef enum{
    OutputA,
    OutputB,
    OutputC,
    AmountOfOutput
}OutputType;

#define PWMCOMPLEX_HEIGHT 131
#define PWMCOMPLEX_WIDTH 315
#define PWMCOMPLEX_SELECTED_THICC 7
#define HEADER_HEIGHT 21
#define EDGESELECTION_COMPLEX_HEIGHT 54
#define DUTYCYCLE_COMPLEX_HEIGHT 30

#define STATUS_HEIGHT 26
#define HEADER_DIVSORY 135

#define FREQMODE_COMPLEX_X0 5
#define FREQMODE_COMPLEX_Y0 2
#define FREQMODE_COMPLEX_FONT_SIZE 2
#define FREQMODE_COMPLEX_STATE 1

#define FREQUENCY_COMPLEX_X0 138
#define FREQUENCY_COMPLEX_Y0 2
#define FREQUENCY_COMPLEX_FONT_SIZE 2
#define FREQUENCY_COMPLEX_STATE 1

#define SECCION_COMPLEX_HEIGHT 110
#define SECCION_WIDTH_A 79
#define SECCION_WIDTH_B 158
#define SECCION_WIDTH_C 236

#define EDGESELECTION_A_X0 0
#define EDGESELECTION_A_Y0 21

#define EDGESELECTION_B_X0 79
#define EDGESELECTION_B_Y0 21

#define EDGESELECTION_C_X0 158
#define EDGESELECTION_C_Y0 21

#define EDGESELECTION_STATE_A 1
#define EDGESELECTION_STATE_B 1
#define EDGESELECTION_STATE_C 1

#define DUTYCYCLE_DIVISORY_Y 75
#define DUTYCYCLE_DIVISORY_WIDTH 236

#define STATUS_DIVISORY_Y 105
#define STATUS_DIVISORY_WIDTH 236

#define DUTYCYCLE_A_X0 5
#define DUTYCYCLE_A_Y0 77

#define DUTYCYCLE_B_X0 84
#define DUTYCYCLE_B_Y0 77

#define DUTYCYCLE_C_X0 163
#define DUTYCYCLE_C_Y0 77

#define DUTYCYCLE_STATE_A 1
#define DUTYCYCLE_STATE_B 1
#define DUTYCYCLE_STATE_C 1

#define STATUS_A_X0 5
#define STATUS_A_Y0 106

#define STATUS_B_X0 84
#define STATUS_B_Y0 106

#define STATUS_C_X0 163
#define STATUS_C_Y0 106

#define STATUS_STATE_A 5
#define STATUS_STATE_B 5
#define STATUS_STATE_C 5

class PWMComplex: public Element {
    private:

        EdgeSelectionIcons edgeSelection[AmountOfOutput];
        Label dutyCycle[AmountOfOutput];
        Label status[AmountOfOutput];
        Label freqMode;
        Label frequency;
        uint8_t curEdgeSelected[AmountOfOutput];        
        bool isSelected = false;
        bool isShown = false;
        uint16_t w,h;
    public:
    PWMComplex(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state, Display *tft):Element(PWM_SELECTION,tft), 
                                            edgeSelection({
                                                {x+EDGESELECTION_A_X0,y+EDGESELECTION_A_Y0,w,h,EDGESELECTION_STATE_A,tft},
                                                {x+EDGESELECTION_B_X0,y+EDGESELECTION_B_Y0,w,h,EDGESELECTION_STATE_B,tft},
                                                {x+EDGESELECTION_C_X0,y+EDGESELECTION_C_Y0,w,h,EDGESELECTION_STATE_C,tft}
                                            }),
                                            dutyCycle({
                                                {x+DUTYCYCLE_A_X0,y+DUTYCYCLE_A_Y0,DUTYCYCLE_FONT_SIZE,h,DUTYCYCLE_STATE_A,"---%",1,tft},
                                                {x+DUTYCYCLE_B_X0,y+DUTYCYCLE_B_Y0,DUTYCYCLE_FONT_SIZE,h,DUTYCYCLE_STATE_B,"---%",1,tft},
                                                {x+DUTYCYCLE_C_X0,y+DUTYCYCLE_C_Y0,DUTYCYCLE_FONT_SIZE,h,DUTYCYCLE_STATE_C,"---%",1,tft}
                                            }),
                                            status({
                                                {x+STATUS_A_X0,y+STATUS_A_Y0,STATUS_FONT_SIZE,h,STATUS_STATE_A,"Desl.",1,tft},
                                                {x+STATUS_B_X0,y+STATUS_B_Y0,STATUS_FONT_SIZE,h,STATUS_STATE_B,"Desl.",1,tft},
                                                {x+STATUS_C_X0,y+STATUS_C_Y0,STATUS_FONT_SIZE,h,STATUS_STATE_C,"Desl.",1,tft}
                                            }),
                                            freqMode(x+FREQMODE_COMPLEX_X0,y+FREQMODE_COMPLEX_Y0,FREQMODE_COMPLEX_FONT_SIZE,h,FREQMODE_COMPLEX_STATE,"M:variavel",1,tft),
                                            frequency(x+FREQUENCY_COMPLEX_X0,y+FREQUENCY_COMPLEX_Y0,FREQUENCY_COMPLEX_FONT_SIZE,h,FREQUENCY_COMPLEX_STATE,"F:----------Hz",1,tft)
                                            
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
            if(isSelected)
            {
                tft->fillRect(x,y,PWMCOMPLEX_WIDTH,PWMCOMPLEX_HEIGHT,DARK_ORANGE);
            } else {
                tft->fillRect(x,y,PWMCOMPLEX_WIDTH,PWMCOMPLEX_HEIGHT,BLACK);
            }
                tft->drawFastHLine(x,y,PWMCOMPLEX_WIDTH,TFT_ORANGE);
            
                tft->drawFastVLine(x,y,PWMCOMPLEX_HEIGHT,TFT_ORANGE);
                tft->drawFastVLine(x+PWMCOMPLEX_WIDTH,y,PWMCOMPLEX_HEIGHT,TFT_ORANGE);
                
                tft->drawFastHLine(x,y+PWMCOMPLEX_HEIGHT,PWMCOMPLEX_WIDTH,TFT_ORANGE);
                //HEADER 
                tft->drawFastHLine(x,y+HEADER_HEIGHT,PWMCOMPLEX_WIDTH,TFT_ORANGE);
                tft->drawFastVLine(x+HEADER_DIVSORY,y,HEADER_HEIGHT,TFT_ORANGE);

                //SECCIONS
                tft->drawFastVLine(x+SECCION_WIDTH_A,y+HEADER_HEIGHT,SECCION_COMPLEX_HEIGHT,TFT_ORANGE);
                tft->drawFastVLine(x+SECCION_WIDTH_B,y+HEADER_HEIGHT,SECCION_COMPLEX_HEIGHT,TFT_ORANGE);
                tft->drawFastVLine(x+SECCION_WIDTH_C,y+HEADER_HEIGHT,SECCION_COMPLEX_HEIGHT,TFT_ORANGE);

                tft->drawFastHLine(x,y+DUTYCYCLE_DIVISORY_Y,DUTYCYCLE_DIVISORY_WIDTH,TFT_ORANGE);
                tft->drawFastHLine(x,y+STATUS_DIVISORY_Y,STATUS_DIVISORY_WIDTH,TFT_ORANGE);

            
            for(int i =0 ; i<AmountOfOutput;i++){
                edgeSelection[i].update();
                dutyCycle[i].update();
            }
            freqMode.update();
            frequency.update();
            
        }
        for(int i =0 ; i<AmountOfOutput;i++){
            status[i].update();
        }
        return *this;
    }
    void show(){
        isShown = false;
        for(int i =0 ; i<AmountOfOutput;i++){
            edgeSelection[i].setIconState(VISIBLE,curEdgeSelected[i]);
            dutyCycle[i].setLabel("---%");
            status[i].setLabel("Desl.");
            status[i].setState(5);
        }
        freqMode.setLabel("M:variavel");
        frequency.setLabel("F:---------Hz");        
    }
        void setSelected(bool isSelected){
            this->isSelected = isSelected;
        }    
};