#pragma once

#include <Element.h>
#include <PWMSimplex.h>
#include <PWMComplex.h>

#define PWM0_X0 2
#define PWM0_Y0 47
#define PWM0_W 320
#define PWM0_H 80

#define PWM1_X0 2
#define PWM1_Y0 127
#define PWM1_W 320
#define PWM1_H 80

#define PWM2_X0 2
#define PWM2_Y0 207
#define PWM2_W 320
#define PWM2_H 135

#define PWM3_X0 2
#define PWM3_Y0 342
#define PWM3_W 320
#define PWM3_H 135


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

};