#pragma once

#include <standardDefinitions.h>
#include <EdgeSelectionIcons.h>
#include <Element.h>
#include <PWMChannelConfig.h>
#include <PWMLabelFormat.h>
#include <PWM.h>
#include "PWMStateMachine.h"
#include "RotaryEncoder.h"

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

/**
 * A three-output PWM channel box (PWM_2 / PWM_3). Owns the UI-editable
 * PWMComplexChannelConfig for this channel and applies it live to the real
 * timer (via the bound PWM* and channelIndex) every time a field is edited.
 * See PWMSimplex.h for the same pattern on a single-output channel; the
 * actual edit/conversion logic lives in PWMChannelConfig/PWMLabelFormat,
 * which are unit tested -- this class is Display-facing glue.
 */
class PWMComplex: public Element {
    private:

        EdgeSelectionIcons edgeSelection[AmountOfOutput];
        Label dutyCycle[AmountOfOutput];
        Label status[AmountOfOutput];
        Label freqMode;
        Label frequency;
        bool isSelected = false;
        bool isShown = false;
        uint16_t w,h;

        PWMComplexChannelConfig config;
        PWM *pwm;
        uint8_t channelIndex;  // 2 -> PWM::setupPWMChannel2, 3 -> setupPWMChannel3

        PWMComplexOutputConfig& outputConfig(OutputType o){
            switch(o){
                case OutputA: return config.outputA;
                case OutputB: return config.outputB;
                default: return config.outputC;
            }
        }

        void updateEdgeIcon(OutputType o){
            bool inverting = outputConfig(o).inverting;
            if(inverting){
                edgeSelection[o].setIconState(VISIBLE, EdgeFallingIcon);
                edgeSelection[o].setIconState(GONE, EdgeRisingIcon);
            } else {
                edgeSelection[o].setIconState(GONE, EdgeFallingIcon);
                edgeSelection[o].setIconState(VISIBLE, EdgeRisingIcon);
            }
        }

        void refreshDisplay(){
            char buf[PWM_LABEL_BUFFER_SIZE];
            // No channel-level enable exists for a complex channel (each
            // output has its own, via ACTIVATE_CHANNEL_A/B/C), so Mode is
            // always shown as Fixed/Variable, never Off.
            formatModeLabel(buf, true, config.frequency);
            freqMode.setLabel(buf);
            formatFrequencyLabel(buf, config.frequency, config.variableTopValue);
            frequency.setLabel(buf);
            for(int i = 0; i < AmountOfOutput; i++){
                formatDutyCycleLabel(buf, outputConfig((OutputType)i).dutyCyclePercent);
                dutyCycle[i].setLabel(buf);
                formatShortStatusLabel(buf, outputConfig((OutputType)i).enabled);
                status[i].setLabel(buf);
                updateEdgeIcon((OutputType)i);
            }
        }

        void applyToHardware(){
            ComplexPWMCallArgs args = computeComplexCallArgs(config);
            if(channelIndex == 2){
                pwm->setupPWMChannel2(args.frequency, args.rawFrequency,
                                       args.invertingA, args.enabledA,
                                       args.invertingB, args.enabledB,
                                       args.invertingC, args.enabledC,
                                       args.rawDutyCycleA, args.rawDutyCycleB, args.rawDutyCycleC);
            } else {
                pwm->setupPWMChannel3(args.frequency, args.rawFrequency,
                                       args.invertingA, args.enabledA,
                                       args.invertingB, args.enabledB,
                                       args.invertingC, args.enabledC,
                                       args.rawDutyCycleA, args.rawDutyCycleB, args.rawDutyCycleC);
            }
        }

    public:
    PWMComplex(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state, Display *tft, PWM *pwm, uint8_t channelIndex):Element(tft),
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
                                            frequency(x+FREQUENCY_COMPLEX_X0,y+FREQUENCY_COMPLEX_Y0,FREQUENCY_COMPLEX_FONT_SIZE,h,FREQUENCY_COMPLEX_STATE,"F:----------Hz",1,tft),
                                            pwm(pwm), channelIndex(channelIndex)

    {
        Element::x = x;
        Element::y = y;
        this->w = w;
        this->h = h;
        Element::state = state;
    }

    void update(){
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
    }
    void show(){
        isShown = false;
        refreshDisplay();
        for(int i =0 ; i<AmountOfOutput;i++){
            status[i].setState(5);
        }
    }
        void setSelected(bool isSelected){
            this->isSelected = isSelected;
        }

        // Routes one encoder tick, while `field` is the highlighted field
        // for this channel, into a config edit -- applied to the real
        // timer immediately and reflected on screen right away.
        void editField(SharedPWMStates field, DIRECTION_TYPE dir){
            if(dir == not_supported){
                return;
            }
            bool increase = (dir == CW);
            switch(field){
                case CONFIG_MODE_FOR_ALL:
                    config.toggleMode();
                    break;
                case CONFIG_FREQUENCY_FOR_ALL:
                    if(config.frequency == frequency_variable){
                        if(increase) config.increaseVariableTop(); else config.decreaseVariableTop();
                    } else {
                        if(increase) config.selectNextFrequency(); else config.selectPreviousFrequency();
                    }
                    break;
                case CONFIG_DUTY_CYCLE_FOR_A:
                    if(increase) config.outputA.increaseDutyCycle(); else config.outputA.decreaseDutyCycle();
                    break;
                case CONFIG_EDGE_FOR_A:
                    config.outputA.toggleEdge();
                    break;
                case ACTIVATE_CHANNEL_A:
                    config.outputA.toggleEnabled();
                    break;
                case CONFIG_DUTY_CYCLE_FOR_B:
                    if(increase) config.outputB.increaseDutyCycle(); else config.outputB.decreaseDutyCycle();
                    break;
                case CONFIG_EDGE_FOR_B:
                    config.outputB.toggleEdge();
                    break;
                case ACTIVATE_CHANNEL_B:
                    config.outputB.toggleEnabled();
                    break;
                case CONFIG_DUTY_CYCLE_FOR_C:
                    if(increase) config.outputC.increaseDutyCycle(); else config.outputC.decreaseDutyCycle();
                    break;
                case CONFIG_EDGE_FOR_C:
                    config.outputC.toggleEdge();
                    break;
                case ACTIVATE_CHANNEL_C:
                    config.outputC.toggleEnabled();
                    break;
                default:
                    return;
            }
            applyToHardware();
            refreshDisplay();
        }
};
