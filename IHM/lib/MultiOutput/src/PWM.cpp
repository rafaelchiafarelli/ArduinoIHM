#include "PWM.h"


void PWM::setupPWMChannel0(PWMFrequency f_selector,bool isInverting, bool enabled, uint16_t frequency, uint16_t dutyCycle){
    //setup timer 3 for PWM generation            
        //variable frequency

		TCCR3B = 0;
		TCCR3A = 0;
        switch (f_selector)
        {
        case frequency_variable:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A = (1 << WGM31) | (1 << COM3A1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR3A = (1 << WGM31) | (1 << COM3A1) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM33) |(1 << WGM32) | (1 << CS30); //NO PRESCALER
            ICR3 = frequency;
            OCR3A = dutyCycle;
            break;
        case frequency_62_500HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A =  (1 << WGM30) | (1 << COM3A1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR3A =  (1 << WGM30) | (1 << COM3A1) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM32) | (1 << CS30); //NO PRESCALER
            OCR3A = dutyCycle;
        break;
        case frequency_31_250HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A = (1 << WGM31); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR3A = (1 << WGM31) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
			}
			TCCR3B = (1 << WGM32) | (1 << CS30); //NO PRESCALER
            OCR3A = dutyCycle;                
        break;
        case frequency_16_625HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A = (1 << WGM31) | (1 << WGM30); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR3A = (1 << WGM31) | (1 << WGM30) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM32) | (1 << CS30); //NO PRESCALER
            OCR3A = dutyCycle;       
        break;
            case frequency_7812_5HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A =  (1 << WGM30) | (1 << COM3A1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR3A =  (1 << WGM30) | (1 << COM3A1) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM32) | (1 << CS31); // 1/8 PRESCALER
            OCR3A = dutyCycle;
        break;
        case frequency_3906_25HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A = (1 << WGM31); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR3A = (1 << WGM31) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM32) | (1 << CS31); // 1/8 PRESCALER
            OCR3A = dutyCycle;                
        break;
        case frequency_1953_125HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A = (1 << WGM31) | (1 << WGM30); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR3A = (1 << WGM31) | (1 << WGM30) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM32) | (1 << CS31); // 1/8 PRESCALER
            OCR3A = dutyCycle;       
        break;

            case frequency_976_5625HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A =  (1 << WGM30) | (1 << COM3A1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR3A =  (1 << WGM30) | (1 << COM3A1) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR3A = dutyCycle;
        break;
        case frequency_488_28125HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A = (1 << WGM31); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR3A = (1 << WGM31) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR3A = dutyCycle;                
        break;
        case frequency_244_140625HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A = (1 << WGM31) | (1 << WGM30); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR3A = (1 << WGM31) | (1 << WGM30) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR3A = dutyCycle;       
        break;


        case frequency_122_0703125HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A = (1 << WGM31); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR3A = (1 << WGM31) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM32) | (1 << CS32); // 1/256 PRESCALER
            OCR3A = dutyCycle;                
        break;
        case frequency_61_03515625HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A = (1 << WGM31) | (1 << WGM30); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR3A = (1 << WGM31) | (1 << WGM30) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM32) | (1 << CS32); // 1/256 PRESCALER
            OCR3A = dutyCycle;       
        break;

        case frequency_30_517578125HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A = (1 << WGM31); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR3A = (1 << WGM31) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM32) | (1 << CS32) | (1 << CS30); // 1/1024 PRESCALER
            OCR3A = dutyCycle;                
        break;
        case frequency_15_2587890625HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR3A = (1 << WGM31) | (1 << WGM30); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR3A = (1 << WGM31) | (1 << WGM30) | (1 << COM3A0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR3B = (1 << WGM32) | (1 << CS32) | (1 << CS30); // 1/1024 PRESCALER
            OCR3A = dutyCycle;       
        break;
        default:
            break;
        }
    
}

      
void PWM::setupPWMChannel1(PWMFrequency f_selector,bool isInverting, bool enabled, uint16_t frequency, uint16_t dutyCycle){
    
    //setup timer 4 for PWM generation            
		TCCR5B = 0;
		TCCR5A = 0;
        switch (f_selector)
        {
        case frequency_variable:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A = (1 << WGM51) | (1 << COM5C1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR5A = (1 << WGM51) | (1 << COM5C1) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM53) |(1 << WGM52) | (1 << CS50); //NO PRESCALER
            ICR5 = frequency;
            OCR5A = dutyCycle;
            break;
        case frequency_62_500HZ:
            
            if(enabled) {            
                if(!isInverting)
                    TCCR5A =  (1 << WGM50) | (1 << COM5C1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR5A =  (1 << WGM50) | (1 << COM5C1) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS50); //NO PRESCALER
            OCR5A = dutyCycle;
        break;
        case frequency_31_250HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A = (1 << WGM51); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR5A = (1 << WGM51) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS50); //NO PRESCALER
            OCR5A = dutyCycle;                
        break;
        case frequency_16_625HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A = (1 << WGM51) | (1 << WGM50); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR5A = (1 << WGM51) | (1 << WGM50) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS50); //NO PRESCALER
            OCR5A = dutyCycle;       
        break;
            case frequency_7812_5HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A =  (1 << WGM50) | (1 << COM5C1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR5A =  (1 << WGM50) | (1 << COM5C1) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS51); // 1/8 PRESCALER
            OCR5A = dutyCycle;
        break;
        case frequency_3906_25HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A = (1 << WGM51); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR5A = (1 << WGM51) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS51); // 1/8 PRESCALER
            OCR5A = dutyCycle;                
        break;
        case frequency_1953_125HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A = (1 << WGM51) | (1 << WGM50); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR5A = (1 << WGM51) | (1 << WGM50) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS51); // 1/8 PRESCALER
            OCR5A = dutyCycle;       
        break;

            case frequency_976_5625HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A =  (1 << WGM50) | (1 << COM5C1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR5A =  (1 << WGM50) | (1 << COM5C1) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS51) | (1 << CS50); // 1/64 PRESCALER
            OCR5A = dutyCycle;
        break;
        case frequency_488_28125HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A = (1 << WGM51); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR5A = (1 << WGM51) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS51) | (1 << CS50); // 1/64 PRESCALER
            OCR5A = dutyCycle;                
        break;
        case frequency_244_140625HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A = (1 << WGM51) | (1 << WGM50); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR5A = (1 << WGM51) | (1 << WGM50) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS51) | (1 << CS50); // 1/64 PRESCALER
            OCR5A = dutyCycle;       
        break;


        case frequency_122_0703125HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A = (1 << WGM51); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR5A = (1 << WGM51) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS52); // 1/256 PRESCALER
            OCR5A = dutyCycle;                
        break;
        case frequency_61_03515625HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A = (1 << WGM51) | (1 << WGM50); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR5A = (1 << WGM51) | (1 << WGM50) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS52); // 1/256 PRESCALER
            OCR5A = dutyCycle;       
        break;

        case frequency_30_517578125HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A = (1 << WGM51); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR5A = (1 << WGM51) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS52) | (1 << CS50); // 1/1024 PRESCALER
            OCR5A = dutyCycle;                
        break;
        case frequency_15_2587890625HZ:
            
            if(enabled) {
                if(!isInverting)
                    TCCR5A = (1 << WGM51) | (1 << WGM50); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR5A = (1 << WGM51) | (1 << WGM50) | (1 << COM5C0); //fast PWM, set on compare, clear at overflow
            
            }
			TCCR5B = (1 << WGM52) | (1 << CS52) | (1 << CS50); // 1/1024 PRESCALER
            OCR5A = dutyCycle;       
        break;
        default:
            break;
        }
    
}                

        
void PWM::setupPWMChannel2(PWMFrequency f_selector,uint16_t frequency,bool isInvertingA,bool enabledA, bool isInvertingB,bool enabledB, bool isInvertingC,bool enabledC,  uint16_t dutyCycleA, uint16_t dutyCycleB, uint16_t dutyCycleC){
//setup timer 4 for PWM generation            
        TCCR1A = 0;
        TCCR1B = 0;
        switch (f_selector)
        {
        case frequency_variable:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A |= (1 << WGM11) | (1 << COM1A1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A |= (1 << WGM11) | (1 << COM1B1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR1A |= (1 << WGM11) | (1 << COM1B1) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A |= (1 << WGM11) | (1 << COM1C1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR1A |= (1 << WGM11) | (1 << COM1C1) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow                

            }
			TCCR1B = (1 << WGM13) |(1 << WGM12) | (1 << CS30); //NO PRESCALER
            ICR1 = frequency;
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;            
            break;
        case frequency_62_500HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A |= (1 << WGM10) | (1 << COM1A1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR1A |= (1 << WGM10) | (1 << COM1A1) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A |= (1 << WGM10) | (1 << COM1B1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR1A |= (1 << WGM10) | (1 << COM1B1) | (1 << COM1A1) |  (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {                
                if(!isInvertingC)
                    TCCR1A |= (1 << WGM10) | (1 << COM1C1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR1A |= (1 << WGM10) | (1 << COM1C1) | (1 << COM1A1) |  (1 << COM1C0); //fast PWM, set on compare, clear at overflow        
            
            }
			TCCR1B = (1 << WGM12) | (1 << CS30); //NO PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;            
        break;
        case frequency_31_250HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A |= (1 << WGM11); //fast PWM, clear on compare, set at overflow
                else
                    TCCR1A |= (1 << WGM11) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A |= (1 << WGM11); //fast PWM, clear on compare, set at overflow
                else
                    TCCR1A |= (1 << WGM11) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A |= (1 << WGM11); //fast PWM, clear on compare, set at overflow
                else
                    TCCR1A |= (1 << WGM11) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow            

            }
			TCCR1B = (1 << WGM12) | (1 << CS30); //NO PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;                            
        break;
        case frequency_16_625HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {    
                if(!isInvertingB)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow                                

            }
			TCCR1B = (1 << WGM12) | (1 << CS30); //NO PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;                   
        break;
        case frequency_7812_5HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A =  (1 << WGM10) | (1 << COM1A1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR1A =  (1 << WGM10) | (1 << COM1A1) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A =  (1 << WGM10) | (1 << COM1B1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR1A =  (1 << WGM10) | (1 << COM1B1) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A =  (1 << WGM10) | (1 << COM1C1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR1A =  (1 << WGM10) | (1 << COM1C1) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow                                
            }
			TCCR1B = (1 << WGM12) | (1 << CS31); // 1/8 PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;            
        break;
        case frequency_3906_25HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow                
            }
			TCCR1B = (1 << WGM12) | (1 << CS31); // 1/8 PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;                            
        break;
        case frequency_1953_125HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow                
            }
			TCCR1B = (1 << WGM12) | (1 << CS31); // 1/8 PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;                   
        break;
        case frequency_976_5625HZ:
            if(enabledA) {            
                if(!isInvertingA)
                    TCCR1A =  (1 << WGM10) | (1 << COM1A1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR1A =  (1 << WGM10) | (1 << COM1A1) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A =  (1 << WGM10) | (1 << COM1B1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR1A =  (1 << WGM10) | (1 << COM1B1) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A =  (1 << WGM10) | (1 << COM1C1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR1A =  (1 << WGM10) | (1 << COM1C1) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow
            }
			TCCR1B = (1 << WGM12) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;            
        break;
        case frequency_488_28125HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow


            }
			TCCR1B = (1 << WGM12) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;                            
        break;
        case frequency_244_140625HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow

            }
			TCCR1B = (1 << WGM12) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;                   
        break;


        case frequency_122_0703125HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow                                

            }
			TCCR1B = (1 << WGM12) | (1 << CS42); // 1/256 PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;                            
        break;
        case frequency_61_03515625HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow

            
            }
			TCCR1B = (1 << WGM12) | (1 << CS42); // 1/256 PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;                   
        break;

        case frequency_30_517578125HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A = (1 << WGM11); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow

            }
			TCCR1B = (1 << WGM12) | (1 << CS42) | (1 << CS30); // 1/1024 PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;                            
        break;
        case frequency_15_2587890625HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR1A = (1 << WGM11) | (1 << WGM10); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1C0); //fast PWM, set on compare, clear at overflow

            }
			TCCR1B = (1 << WGM12) | (1 << CS42) | (1 << CS30); // 1/1024 PRESCALER
            OCR1A = dutyCycleA;
            OCR1B = dutyCycleB;
            OCR1C = dutyCycleC;                   
        break;
        default:
            break;
        }
    

}        


void PWM::setupPWMChannel3(PWMFrequency f_selector,uint16_t frequency,bool isInvertingA,bool enabledA, bool isInvertingB,bool enabledB, bool isInvertingC,bool enabledC,  uint16_t dutyCycleA, uint16_t dutyCycleB, uint16_t dutyCycleC){
    //setup timer 4 for PWM generation            
        TCCR4A = 0;
        TCCR4B = 0;
        switch (f_selector)
        {
        case frequency_variable:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR4A |= (1 << WGM41) | (1 << COM4A1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR4A |= (1 << WGM41) | (1 << COM4A1) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB )
                    TCCR4A |= (1 << WGM41) | (1 << COM4B1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR4A |= (1 << WGM41) | (1 << COM4B1) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A |= (1 << WGM41) | (1 << COM4C1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR4A |= (1 << WGM41) | (1 << COM4C1) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow                

            }
			TCCR4B = (1 << WGM43) |(1 << WGM42) | (1 << CS30); //NO PRESCALER
            ICR4 = frequency;
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;            
            break;
        case frequency_62_500HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR4A |= (1 << WGM40) | (1 << COM4A1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR4A |= (1 << WGM40) | (1 << COM4A1) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR4A |= (1 << WGM40) | (1 << COM4B1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR4A |= (1 << WGM40) | (1 << COM4B1) | (1 << COM4A1) |  (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A |= (1 << WGM40) | (1 << COM4C1); //fast PWM, clear on compare, set at overflow
                else
                    TCCR4A |= (1 << WGM40) | (1 << COM4C1) | (1 << COM4A1) |  (1 << COM4C0); //fast PWM, set on compare, clear at overflow        
                
            }
			TCCR4B = (1 << WGM42) | (1 << CS30); //NO PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;            
        break;
        case frequency_31_250HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR4A |= (1 << WGM41); //fast PWM, clear on compare, set at overflow
                else
                    TCCR4A |= (1 << WGM41) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR4A |= (1 << WGM41); //fast PWM, clear on compare, set at overflow
                else
                    TCCR4A |= (1 << WGM41) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A |= (1 << WGM41); //fast PWM, clear on compare, set at overflow
                else
                    TCCR4A |= (1 << WGM41) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow            

            }
			TCCR4B = (1 << WGM42) | (1 << CS30); //NO PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;                            
        break;
        case frequency_16_625HZ:
            if(enabledA) {            
                if(!isInvertingA)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow                                

            }
			TCCR4B = (1 << WGM42) | (1 << CS30); //NO PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;                   
        break;
        case frequency_7812_5HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR4A =  (1 << WGM40) | (1 << COM4A1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR4A =  (1 << WGM40) | (1 << COM4A1) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
            if(enabledB) {
                if(!isInvertingB)
                    TCCR4A =  (1 << WGM40) | (1 << COM4B1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR4A =  (1 << WGM40) | (1 << COM4B1) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
            if(enabledC) {
                if(!isInvertingC)
                    TCCR4A =  (1 << WGM40) | (1 << COM4C1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR4A =  (1 << WGM40) | (1 << COM4C1) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow                                
            }
			TCCR4B = (1 << WGM42) | (1 << CS31); // 1/8 PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;            
        break;
        case frequency_3906_25HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow                

            }
			TCCR4B = (1 << WGM42) | (1 << CS31); // 1/8 PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;                            
        break;
        case frequency_1953_125HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow                
            }
			TCCR4B = (1 << WGM42) | (1 << CS31); // 1/8 PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;                   
        break;
        case frequency_976_5625HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR4A =  (1 << WGM40) | (1 << COM4A1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR4A =  (1 << WGM40) | (1 << COM4A1) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR4A =  (1 << WGM40) | (1 << COM4B1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR4A =  (1 << WGM40) | (1 << COM4B1) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A =  (1 << WGM40) | (1 << COM4C1); //fast PWM, 8bit, clear on compare, set at overflow
                else
                    TCCR4A =  (1 << WGM40) | (1 << COM4C1) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow
            }
			TCCR4B = (1 << WGM42) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;            
        break;
        case frequency_488_28125HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow


            }
			TCCR4B = (1 << WGM42) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;                            
        break;
        case frequency_244_140625HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow

            }
			TCCR4B = (1 << WGM42) | (1 << CS31) | (1 << CS30); // 1/64 PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;                   
        break;


        case frequency_122_0703125HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow                                

            }
			TCCR4B = (1 << WGM42) | (1 << CS42); // 1/256 PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;                            
        break;
        case frequency_61_03515625HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow

            
            }
			TCCR4B = (1 << WGM42) | (1 << CS42); // 1/256 PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;                   
        break;

        case frequency_30_517578125HZ:
            if(enabledA) {            
                if(!isInvertingA)
                    TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A = (1 << WGM41); //fast PWM, 9bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow

            }
			TCCR4B = (1 << WGM42) | (1 << CS42) | (1 << CS30); // 1/1024 PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;                            
        break;
        case frequency_15_2587890625HZ:
            if(enabledA) {
                if(!isInvertingA)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4A0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledB) {
                if(!isInvertingB)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4B0); //fast PWM, set on compare, clear at overflow
            }
			if(enabledC) {
                if(!isInvertingC)
                    TCCR4A = (1 << WGM41) | (1 << WGM40); //fast PWM, 10bit, clear on compare, set at overflow
                else
                    TCCR4A = (1 << WGM41) | (1 << WGM40) | (1 << COM4C0); //fast PWM, set on compare, clear at overflow

            }
			TCCR4B = (1 << WGM42) | (1 << CS42) | (1 << CS30); // 1/1024 PRESCALER
            OCR4A = dutyCycleA;
            OCR4B = dutyCycleB;
            OCR4C = dutyCycleC;                   
        break;
        default:
            break;
        }
    
}        

  