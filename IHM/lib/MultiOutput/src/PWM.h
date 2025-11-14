#ifndef PWM_H_
#define PWM_H_

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>


typedef enum{
    frequency_62_500HZ,
    frequency_31_250HZ,
    frequency_16_625HZ,
    frequency_7812_5HZ,
    frequency_3906_25HZ,
    frequency_1953_125HZ,
    frequency_976_5625HZ,
    frequency_488_28125HZ,
    frequency_244_140625HZ,
    frequency_122_0703125HZ,
    frequency_61_03515625HZ,
    frequency_30_517578125HZ,
    frequency_15_2587890625HZ,
    frequency_variable

}PWMFrequency;


class PWM
{
    private:

    public:
        PWM(){};
        void setupPWMChannel0(PWMFrequency f_selector,bool isInverting,uint16_t frequency, uint16_t dutyCycle);
        void setupPWMChannel1(PWMFrequency f_selector,bool isInverting,uint16_t frequency, uint16_t dutyCycle);
        void setupPWMChannel2(PWMFrequency f_selector,bool isInverting,uint16_t frequency, uint16_t dutyCycle);
        void setupPWMChannel3(PWMFrequency f_selector,bool isInverting,uint16_t frequency, uint16_t dutyCycle);

};

#endif /* PWM_H_ */