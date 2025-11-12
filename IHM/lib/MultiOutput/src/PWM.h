#ifndef PWM_H_
#define PWM_H_

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>

typedef enum{
    PWM_CHANNEL_0, /* TMR3 independent */
    PWM_CHANNEL_1, /* TMR4 independent  */
    PWM_CHANNEL_2, /* TMR2 independent  */
    PWM_CHANNEL_3, /* TMR5 independent  */
    PWM_CHANNEL_4, /* TMR1 kills servo */
    PWM_CHANNEL_5, /* TMR1 kills servo  */
    PWM_CHANNEL_6, /* TMR1 kills servo  */
    PWM_CHANNEL_7, /* TMR2 slave of channel 2 */
    PWM_CHANNEL_8, /* TMR4 slave of channel 1 */
    PWM_CHANNEL_9, /* TMR4 slave of channel 1 */
    PWM_CHANNEL_10, /* TMR0 fixed frequncy */
    NUMBER_OF_PWM_CHANNELS
}PWMChannel;

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
        void setDutyCycle(PWMChannel channel, uint8_t dutyCycle){
            switch (channel)
            {
            case PWM_CHANNEL_0:
                OCR0A = dutyCycle;
                break;
            case PWM_CHANNEL_1:
                OCR3A = dutyCycle;
                break;
            case PWM_CHANNEL_2:
                OCR2A = dutyCycle;
                break;            
            default:
                break;
            }
        }
};

#endif /* PWM_H_ */