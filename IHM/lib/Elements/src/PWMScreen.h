#pragma once

#include <Element.h>
#include <PWMSimplex.h>
#include <PWMComplex.h>
#include <PWM.h>
#include "PWMStateMachine.h"
#include "RotaryEncoder.h"

#define PWM0_X0 4
#define PWM0_Y0 47
#define PWM0_W 316
#define PWM0_H 84

#define PWM1_X0 4
#define PWM1_Y0 131
#define PWM1_W 316
#define PWM1_H 84

#define PWM2_X0 4
#define PWM2_Y0 215
#define PWM2_W 316
#define PWM2_H 131

#define PWM3_X0 4
#define PWM3_Y0 346
#define PWM3_W 316
#define PWM3_H 131

/**
 * The PWM tab: 4 channel boxes (2 simplex, 2 complex) plus the navigation
 * state (PWMStateMachine) that decides which channel/field the encoders
 * currently drive. Two navigation levels, both driven by the same physical
 * encoder (rot1/d1) so a third encoder isn't needed:
 *   - channel-select mode (the default): d1 moves the highlighted box
 *     between the 4 channels.
 *   - field-edit mode (entered/left via toggleEditMode(), see GUI.cpp): d1
 *     moves between that channel's fields (mode/frequency/duty/edge/...),
 *     d2 adjusts the highlighted field's value -- applied to the real timer
 *     immediately by PWMSimplex/PWMComplex, see their editField().
 */
class PWMScreen{
    private:
        PWMSimplex pwm0;
        PWMSimplex pwm1;
        PWMComplex pwm2;
        PWMComplex pwm3;
        PWMStateMachine stateMachine;
        bool editingField = false;

        void applySelectionHighlight(){
            PWMConfigStates sel = stateMachine.getSelectedChannel();
            pwm0.setSelected(sel == PWM_0);
            pwm1.setSelected(sel == PWM_1);
            pwm2.setSelected(sel == PWM_2);
            pwm3.setSelected(sel == PWM_3);
        }

    public:
        PWMScreen(Display *tft, PWM *pwm):
            pwm0(PWM0_X0,PWM0_Y0,PWM0_W,PWM0_H,0,tft,pwm,0),
            pwm1(PWM1_X0,PWM1_Y0,PWM1_W,PWM1_H,0,tft,pwm,1),
            pwm2(PWM2_X0,PWM2_Y0,PWM2_W,PWM2_H,0,tft,pwm,2),
            pwm3(PWM3_X0,PWM3_Y0,PWM3_W,PWM3_H,0,tft,pwm,3) {

        }
        void update() {
            pwm0.update();
            pwm1.update();
            pwm2.update();
            pwm3.update();
        }
        void show(){
            pwm0.show();
            pwm1.show();
            pwm2.show();
            pwm3.show();
            applySelectionHighlight();
        }

        bool isEditingField() const { return editingField; }

        void toggleEditMode(){
            editingField = !editingField;
            if(editingField){
                stateMachine.resetFields();
            }
        }

        void selectNextChannel(){
            stateMachine.selectNextChannel();
            applySelectionHighlight();
        }
        void selectPreviousChannel(){
            stateMachine.selectPreviousChannel();
            applySelectionHighlight();
        }

        void selectNextField(){
            if(stateMachine.isComplexChannel()){
                stateMachine.selectNextSharedField();
            } else {
                stateMachine.selectNextRegularField();
            }
        }
        void selectPreviousField(){
            if(stateMachine.isComplexChannel()){
                stateMachine.selectPreviousSharedField();
            } else {
                stateMachine.selectPreviousRegularField();
            }
        }

        void adjustSelectedField(DIRECTION_TYPE dir){
            switch(stateMachine.getSelectedChannel()){
                case PWM_0:
                    pwm0.editField(stateMachine.getRegularField(), dir);
                    break;
                case PWM_1:
                    pwm1.editField(stateMachine.getRegularField(), dir);
                    break;
                case PWM_2:
                    pwm2.editField(stateMachine.getSharedField(), dir);
                    break;
                case PWM_3:
                    pwm3.editField(stateMachine.getSharedField(), dir);
                    break;
                default:
                    break;
            }
        }
};
