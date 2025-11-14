#include "PWM.h"


void PWM::setupPWMChannel0(PWMFrequency f_selector,bool isInverting,uint16_t frequency, uint16_t dutyCycle){
    //setup timer 3 for PWM generation            
        //variable frequency
    
        switch (f_selector)
        {
        case frequency_variable:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A = (1 << WGM31) | (1 << COM3A1); //fast PWM, clear on compare, set at overflow
            else
                TCCR3A = (1 << WGM31) | (1 << COM3A1) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM33) |(1 << WGM32) | (1 << CS30); //NO PRESCALER
            ICR3 = frequency;
            OCR3A = dutyCycle;
            break;
        case frequency_62_500HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A =  (1 << WGM30) | (1 << COM3A1); //fast PWM, 8bit, clear on compare, set at overflow
            else
                TCCR3A =  (1 << WGM30) | (1 << COM3A1) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS30); //NO PRESCALER
            OCR3A = dutyCycle;
        break;
        case frequency_31_250HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A = (1 << WGM31); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR3A = (1 << WGM31) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS30); //NO PRESCALER
            OCR3A = dutyCycle;                
        break;
        case frequency_16_625HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A = (1 << WGM31) | (1 << WGM30); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR3A = (1 << WGM31) | (1 << WGM30) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS30); //NO PRESCALER
            OCR3A = dutyCycle;       
        break;
            case frequency_7812_5HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A =  (1 << WGM30) | (1 << COM3A1); //fast PWM, 8bit, clear on compare, set at overflow
            else
                TCCR3A =  (1 << WGM30) | (1 << COM3A1) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS31); // 1/8 PRESCALER
            OCR3A = dutyCycle;
        break;
        case frequency_3906_25HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A = (1 << WGM31); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR3A = (1 << WGM31) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS31); // 1/8 PRESCALER
            OCR3A = dutyCycle;                
        break;
        case frequency_1953_125HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A = (1 << WGM31) | (1 << WGM30); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR3A = (1 << WGM31) | (1 << WGM30) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS31); // 1/8 PRESCALER
            OCR3A = dutyCycle;       
        break;

            case frequency_976_5625HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A =  (1 << WGM30) | (1 << COM3A1); //fast PWM, 8bit, clear on compare, set at overflow
            else
                TCCR3A =  (1 << WGM30) | (1 << COM3A1) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR3A = dutyCycle;
        break;
        case frequency_488_28125HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A = (1 << WGM31); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR3A = (1 << WGM31) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR3A = dutyCycle;                
        break;
        case frequency_244_140625HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A = (1 << WGM31) | (1 << WGM30); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR3A = (1 << WGM31) | (1 << WGM30) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR3A = dutyCycle;       
        break;


        case frequency_122_0703125HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A = (1 << WGM31); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR3A = (1 << WGM31) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS32); // 1/256 PRESCALER
            OCR3A = dutyCycle;                
        break;
        case frequency_61_03515625HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A = (1 << WGM31) | (1 << WGM30); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR3A = (1 << WGM31) | (1 << WGM30) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS32); // 1/256 PRESCALER
            OCR3A = dutyCycle;       
        break;

        case frequency_30_517578125HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A = (1 << WGM31); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR3A = (1 << WGM31) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS32) | (1 << CS30); // 1/1024 PRESCALER
            OCR3A = dutyCycle;                
        break;
        case frequency_15_2587890625HZ:
            TCCR3A = 0;
            if(!isInverting)
                TCCR3A = (1 << WGM31) | (1 << WGM30); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR3A = (1 << WGM31) | (1 << WGM30) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            TCCR3B = 0;
            TCCR3B = (1 << WGM32) | (1 << CS32) | (1 << CS30); // 1/1024 PRESCALER
            OCR3A = dutyCycle;       
        break;
        default:
            break;
        }
    
}

        
        
void PWM::setupPWMChannel2(PWMFrequency f_selector,bool isInverting,uint16_t frequency, uint16_t dutyCycle){
//setup timer 4 for PWM generation            

    switch (f_selector)
    {
    case frequency_variable:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A = (1 << WGM11) | (1 << COM1A1); //fast PWM, clear on compare, set at overflow
        else
            TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM13) |(1 << WGM12) | (1 << CS10); //NO PRESCALER
        ICR1 = frequency;
        OCR1A = dutyCycle;
        break;
    case frequency_62_500HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A =  (1 << WGM10) | (1 << COM1A1); //fast PWM, 8bit, clear on compare, set at overflow
        else
            TCCR1A =  (1 << WGM10) | (1 << COM1A1) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS10); //NO PRESCALER
        OCR1A = dutyCycle;
    break;
    case frequency_31_250HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
        else
            TCCR1A = (1 << WGM11) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS10); //NO PRESCALER
        OCR1A = dutyCycle;                
    break;
    case frequency_16_625HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
        else
            TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS10); //NO PRESCALER
        OCR1A = dutyCycle;       
    break;
        case frequency_7812_5HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A =  (1 << WGM10) | (1 << COM1A1); //fast PWM, 8bit, clear on compare, set at overflow
        else
            TCCR1A =  (1 << WGM10) | (1 << COM1A1) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS11); // 1/8 PRESCALER
        OCR1A = dutyCycle;
    break;
    case frequency_3906_25HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
        else
            TCCR1A = (1 << WGM11) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS11); // 1/8 PRESCALER
        OCR1A = dutyCycle;                
    break;
    case frequency_1953_125HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
        else
            TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS11); // 1/8 PRESCALER
        OCR1A = dutyCycle;       
    break;

        case frequency_976_5625HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A =  (1 << WGM10) | (1 << COM1A1); //fast PWM, 8bit, clear on compare, set at overflow
        else
            TCCR1A =  (1 << WGM10) | (1 << COM1A1) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // 1/64 PRESCALER
        OCR1A = dutyCycle;
    break;
    case frequency_488_28125HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
        else
            TCCR1A = (1 << WGM11) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // 1/64 PRESCALER
        OCR1A = dutyCycle;                
    break;
    case frequency_244_140625HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
        else
            TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // 1/64 PRESCALER
        OCR1A = dutyCycle;       
    break;


    case frequency_122_0703125HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
        else
            TCCR1A = (1 << WGM11) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS12); // 1/256 PRESCALER
        OCR1A = dutyCycle;                
    break;
    case frequency_61_03515625HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
        else
            TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS12); // 1/256 PRESCALER
        OCR1A = dutyCycle;       
    break;

    case frequency_30_517578125HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
        else
            TCCR1A = (1 << WGM11) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // 1/1024 PRESCALER
        OCR1A = dutyCycle;                
    break;
    case frequency_15_2587890625HZ:
        TCCR1A = 0;
        if(!isInverting)
            TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
        else
            TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
        TCCR1B = 0;
        TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // 1/1024 PRESCALER
        OCR1A = dutyCycle;       
    break;
    default:
        break;
    }

}        


void PWM::setupPWMChannel1(PWMFrequency f_selector,bool isInverting,uint16_t frequency, uint16_t dutyCycle){
    //setup timer 4 for PWM generation            
    
        switch (f_selector)
        {
        case frequency_variable:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A = (1 << WGM41) | (1 << COM4A1); //fast PWM, clear on compare, set at overflow
            else
                TCCR4A = (1 << WGM41) | (1 << COM4A1) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM43) |(1 << WGM42) | (1 << CS30); //NO PRESCALER
            ICR4 = frequency;
            OCR4A = dutyCycle;
            break;
        case frequency_62_500HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A =  (1 << WGM40) | (1 << COM4A1); //fast PWM, 8bit, clear on compare, set at overflow
            else
                TCCR4A =  (1 << WGM40) | (1 << COM4A1) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS30); //NO PRESCALER
            OCR4A = dutyCycle;
        break;
        case frequency_31_250HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR4A = (1 << WGM41) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS30); //NO PRESCALER
            OCR4A = dutyCycle;                
        break;
        case frequency_16_625HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS30); //NO PRESCALER
            OCR4A = dutyCycle;       
        break;
            case frequency_7812_5HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A =  (1 << WGM40) | (1 << COM4A1); //fast PWM, 8bit, clear on compare, set at overflow
            else
                TCCR4A =  (1 << WGM40) | (1 << COM4A1) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS31); // 1/8 PRESCALER
            OCR4A = dutyCycle;
        break;
        case frequency_3906_25HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR4A = (1 << WGM41) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS31); // 1/8 PRESCALER
            OCR4A = dutyCycle;                
        break;
        case frequency_1953_125HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS31); // 1/8 PRESCALER
            OCR4A = dutyCycle;       
        break;

            case frequency_976_5625HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A =  (1 << WGM40) | (1 << COM4A1); //fast PWM, 8bit, clear on compare, set at overflow
            else
                TCCR4A =  (1 << WGM40) | (1 << COM4A1) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR4A = dutyCycle;
        break;
        case frequency_488_28125HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR4A = (1 << WGM41) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR4A = dutyCycle;                
        break;
        case frequency_244_140625HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR4A = dutyCycle;       
        break;


        case frequency_122_0703125HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR4A = (1 << WGM41) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS42); // 1/256 PRESCALER
            OCR4A = dutyCycle;                
        break;
        case frequency_61_03515625HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS42); // 1/256 PRESCALER
            OCR4A = dutyCycle;       
        break;

        case frequency_30_517578125HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR4A = (1 << WGM41) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS42) | (1 << CS30); // 1/1024 PRESCALER
            OCR4A = dutyCycle;                
        break;
        case frequency_15_2587890625HZ:
            TCCR4A = 0;
            if(!isInverting)
                TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            TCCR4B = 0;
            TCCR4B = (1 << WGM42) | (1 << CS42) | (1 << CS30); // 1/1024 PRESCALER
            OCR4A = dutyCycle;       
        break;
        default:
            break;
        }
    
}        


        
void PWM::setupPWMChannel3(PWMFrequency f_selector,bool isInverting,uint16_t frequency, uint16_t dutyCycle){
    //setup timer 4 for PWM generation            
    
        switch (f_selector)
        {
        case frequency_variable:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A = (1 << WGM51) | (1 << COM5C1); //fast PWM, clear on compare, set at overflow
            else
                TCCR5A = (1 << WGM51) | (1 << COM5C1) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM53) |(1 << WGM52) | (1 << CS50); //NO PRESCALER
            ICR5 = frequency;
            OCR5A = dutyCycle;
            break;
        case frequency_62_500HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A =  (1 << WGM50) | (1 << COM5C1); //fast PWM, 8bit, clear on compare, set at overflow
            else
                TCCR5A =  (1 << WGM50) | (1 << COM5C1) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS50); //NO PRESCALER
            OCR5A = dutyCycle;
        break;
        case frequency_31_250HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A = (1 << WGM51); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR5A = (1 << WGM51) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS50); //NO PRESCALER
            OCR5A = dutyCycle;                
        break;
        case frequency_16_625HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A = (1 << WGM51) | (1 << WGM50); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR5A = (1 << WGM51) | (1 << WGM50) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS50); //NO PRESCALER
            OCR5A = dutyCycle;       
        break;
            case frequency_7812_5HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A =  (1 << WGM50) | (1 << COM5C1); //fast PWM, 8bit, clear on compare, set at overflow
            else
                TCCR5A =  (1 << WGM50) | (1 << COM5C1) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS51); // 1/8 PRESCALER
            OCR5A = dutyCycle;
        break;
        case frequency_3906_25HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A = (1 << WGM51); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR5A = (1 << WGM51) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS51); // 1/8 PRESCALER
            OCR5A = dutyCycle;                
        break;
        case frequency_1953_125HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A = (1 << WGM51) | (1 << WGM50); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR5A = (1 << WGM51) | (1 << WGM50) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS51); // 1/8 PRESCALER
            OCR5A = dutyCycle;       
        break;

            case frequency_976_5625HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A =  (1 << WGM50) | (1 << COM5C1); //fast PWM, 8bit, clear on compare, set at overflow
            else
                TCCR5A =  (1 << WGM50) | (1 << COM5C1) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS51) | (1 << CS50); // 1/64 PRESCALER
            OCR5A = dutyCycle;
        break;
        case frequency_488_28125HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A = (1 << WGM51); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR5A = (1 << WGM51) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS51) | (1 << CS50); // 1/64 PRESCALER
            OCR5A = dutyCycle;                
        break;
        case frequency_244_140625HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A = (1 << WGM51) | (1 << WGM50); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR5A = (1 << WGM51) | (1 << WGM50) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS51) | (1 << CS50); // 1/64 PRESCALER
            OCR5A = dutyCycle;       
        break;


        case frequency_122_0703125HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A = (1 << WGM51); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR5A = (1 << WGM51) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS52); // 1/256 PRESCALER
            OCR5A = dutyCycle;                
        break;
        case frequency_61_03515625HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A = (1 << WGM51) | (1 << WGM50); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR5A = (1 << WGM51) | (1 << WGM50) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS52); // 1/256 PRESCALER
            OCR5A = dutyCycle;       
        break;

        case frequency_30_517578125HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A = (1 << WGM51); //fast PWM, 9bit, clear on compare, set at overflow
            else
                TCCR5A = (1 << WGM51) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS52) | (1 << CS50); // 1/1024 PRESCALER
            OCR5A = dutyCycle;                
        break;
        case frequency_15_2587890625HZ:
            TCCR5A = 0;
            if(!isInverting)
                TCCR5A = (1 << WGM51) | (1 << WGM50); //fast PWM, 10bit, clear on compare, set at overflow
            else
                TCCR5A = (1 << WGM51) | (1 << WGM50) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            TCCR5B = 0;
            TCCR5B = (1 << WGM52) | (1 << CS52) | (1 << CS50); // 1/1024 PRESCALER
            OCR5A = dutyCycle;       
        break;
        default:
            break;
        }
    
}                