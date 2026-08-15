#pragma once

#include <standardDefinitions.h>
#include <Element.h>
#include <EdgeSelectionIcons.h>
#include <PWMChannelConfig.h>
#include <PWMLabelFormat.h>
#include <PWM.h>
#include "PWMStateMachine.h"
#include "RotaryEncoder.h"

#define PWMSIMPLEX_HEIGHT 84
#define PWMSIMPLEX_WIDTH 315
#define PWMSIMPLEX_SELECTED_THICC 7

#define EDGESELECTION_X0 2
#define EDGESELECTION_Y0 2
#define EDGESELECTION_STATE 1
#define EDGESELECTION_HEIGHT 52


#define DUTYCYCLE_X0 10
#define DUTYCYCLE_Y0 60
#define DUTYCYCLE_FONT_SIZE 2
#define DUTYCYCLE_STATE 1

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

/**
 * A single-output PWM channel box (PWM_0 / PWM_1). Owns the UI-editable
 * PWMChannelConfig for this channel and applies it live to the real timer
 * (via the bound PWM* and channelIndex) every time a field is edited --
 * there is no separate "confirm" step, matching a typical function
 * generator's turn-the-dial-and-see-it-change UX. All of that logic (what
 * changes for which field/direction, how it converts to raw register
 * arguments) lives in PWMChannelConfig/PWMLabelFormat, which are unit
 * tested; this class is just the Display-facing glue.
 */
class PWMSimplex: public Element {
    private:
        EdgeSelectionIcons edgeSelection;
        Label dutyCycle;
        Label freqMode;
        Label frequency;
        Label status;

        bool isShown = false;
        bool isSelected = false;

        uint16_t w,h;

        PWMChannelConfig config;
        PWM *pwm;
        uint8_t channelIndex;  // 0 -> PWM::setupPWMChannel0, 1 -> setupPWMChannel1

        void updateEdgeIcon(){
            if(config.inverting){
                edgeSelection.setIconState(VISIBLE, EdgeFallingIcon);
                edgeSelection.setIconState(GONE, EdgeRisingIcon);
            } else {
                edgeSelection.setIconState(GONE, EdgeFallingIcon);
                edgeSelection.setIconState(VISIBLE, EdgeRisingIcon);
            }
        }

        void refreshDisplay(){
            char buf[PWM_LABEL_BUFFER_SIZE];
            formatModeLabel(buf, config.enabled, config.frequency);
            freqMode.setLabel(buf);
            formatFrequencyLabel(buf, config.frequency, config.variableTopValue);
            frequency.setLabel(buf);
            formatDutyCycleLabel(buf, config.dutyCyclePercent);
            dutyCycle.setLabel(buf);
            formatStatusLabel(buf, config.enabled);
            status.setLabel(buf);
            updateEdgeIcon();
        }

        void applyToHardware(){
            SimplexPWMCallArgs args = computeSimplexCallArgs(config);
            if(channelIndex == 0){
                pwm->setupPWMChannel0(args.frequency, args.inverting, args.enabled, args.rawFrequency, args.rawDutyCycle);
            } else {
                pwm->setupPWMChannel1(args.frequency, args.inverting, args.enabled, args.rawFrequency, args.rawDutyCycle);
            }
        }

    public:
        PWMSimplex(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t state, Display *tft, PWM *pwm, uint8_t channelIndex):
                                            Element(tft),
                                            edgeSelection(x+EDGESELECTION_X0,y+EDGESELECTION_Y0,w,h,EDGESELECTION_STATE,tft),
                                            dutyCycle(x+DUTYCYCLE_X0,y+DUTYCYCLE_Y0,DUTYCYCLE_FONT_SIZE,h,DUTYCYCLE_STATE,"---%",1,tft),
                                            freqMode(x+FREQMODE_X0,y+FREQMODE_Y0,FREQUENCY_FONT_SIZE,h,FREQMODE_STATE,"Mode:variavel.",1,tft),
                                            frequency(x+FREQUENCY_X0,y+FREQUENCY_Y0,FREQUENCY_FONT_SIZE,h,FREQMODE_STATE,"f:----------Hz",1,tft),
                                            status(x+STATUS_X0,y+STATUS_Y0,STATUS_FONT_SIZE,h,STATUS_STATE,"Status:Inativo",1,tft),
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
                if(isSelected){
                    tft->fillRect(x,y,PWMSIMPLEX_WIDTH,PWMSIMPLEX_HEIGHT,DARK_ORANGE);
                } else {
                    tft->fillRect(x,y,PWMSIMPLEX_WIDTH,PWMSIMPLEX_HEIGHT,BLACK);
                }
                    tft->drawFastHLine(x,y,PWMSIMPLEX_WIDTH,TFT_ORANGE);

                    tft->drawFastVLine(x,y,PWMSIMPLEX_HEIGHT,TFT_ORANGE);
                    tft->drawFastVLine(x+PWMSIMPLEX_WIDTH,y,PWMSIMPLEX_HEIGHT,TFT_ORANGE);

                    tft->drawFastHLine(x,y+PWMSIMPLEX_HEIGHT,PWMSIMPLEX_WIDTH,TFT_ORANGE);
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
        }
        void show(){
            isShown = false;
            refreshDisplay();
            status.setState(5);
        }
        void setSelected(bool isSelected){
            this->isSelected = isSelected;
        }

        // Routes one encoder tick, while `field` is the highlighted field
        // within this channel's config, into a config edit -- applied to
        // the real timer immediately (see class comment) and reflected on
        // screen right away.
        void editField(RegularPWMStates field, DIRECTION_TYPE dir){
            if(dir == not_supported){
                return;
            }
            bool increase = (dir == CW);
            switch(field){
                case CONFIG_MODE:
                    if(increase) config.cycleModeNext(); else config.cycleModePrevious();
                    break;
                case CONFIG_FREQUENCY:
                    if(config.mode() == PWMChannelMode::Variable){
                        if(increase) config.increaseVariableTop(); else config.decreaseVariableTop();
                    } else {
                        if(increase) config.selectNextFrequency(); else config.selectPreviousFrequency();
                    }
                    break;
                case CONFIG_DUTY_CYCLE:
                    if(increase) config.increaseDutyCycle(); else config.decreaseDutyCycle();
                    break;
                case CONFIG_EDGE:
                    config.toggleEdge();
                    break;
                default:
                    return;
            }
            applyToHardware();
            refreshDisplay();
        }
};
