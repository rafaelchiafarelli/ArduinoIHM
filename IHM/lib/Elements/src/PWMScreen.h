#pragma once

#include <Element.h>
#include <PWMSimplex.h>
#include <PWMComplex.h>

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


class PWMScreen{
    private:
        PWMSimplex pwm0;
        PWMSimplex pwm1;
        PWMComplex pwm2;
        PWMComplex pwm3;
    public:
        PWMScreen(Display *tft):pwm0(PWM0_X0,PWM0_Y0,PWM0_W,PWM0_H,0,tft),pwm1(PWM1_X0,PWM1_Y0,PWM1_W,PWM1_H,0,tft),pwm2(PWM2_X0,PWM2_Y0,PWM2_W,PWM2_H,0,tft),pwm3(PWM3_X0,PWM3_Y0,PWM3_W,PWM3_H,0,tft) {

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
        }

};