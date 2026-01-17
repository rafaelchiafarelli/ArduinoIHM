#pragma once
#include <events.h

typedef enum {
    PWM_0, //TMR5_CONFIG,
    PWM_1, //TMR3_CONFIG,
    PWM_2, //TMR1_CONFIG,
    PWM_3, //TMR4_CONFIG,
    NONE_SELECTED
    
}PWMConfigStates;

typedef enum {
    CONFIG_MODE,
    CONFIG_FREQUENCY,
    CONFIG_DUTY_CYCLE,
    CONFIG_EDGE
}RegularPWMStates;

typedef enum {
    CONFIG_MODE_FOR_ALL,
    CONFIG_FREQUENCY_FOR_ALL,
    
    CONFIG_DUTY_CYCLE_FOR_A,
    CONFIG_EDGE_FOR_A,
    ACTIVATE_CHANNEL_A,

    CONFIG_DUTY_CYCLE_FOR_B,
    CONFIG_EDGE_FOR_B,
    ACTIVATE_CHANNEL_B,

    CONFIG_DUTY_CYCLE_FOR_C,
    CONFIG_EDGE_FOR_C,
    ACTIVATE_CHANNEL_C,
    
    CONFIG_DUTY_CYCLE_FOR_D,
    CONFIG_EDGE_FOR_D,
    ACTIVATE_CHANNEL_D,

    CONFIG_DUTY_CYCLE_FOR_E,
    CONFIG_EDGE_FOR_E,
    ACTIVATE_CHANNEL_E,

    CONFIG_DUTY_CYCLE_FOR_F,
    CONFIG_EDGE_FOR_F,
    ACTIVATE_CHANNEL_F
}SharedPWMStates;

class PWMStateMachine {
    private:
        PWMConfigStates currentState;
        RegularPWMStates regularPWMState;
        SharedPWMStates sharedPWMState;

    public:
        PWMStateMachine():currentState(NONE_SELECTED),regularPWMState(CONFIG_MODE),sharedPWMState(CONFIG_MODE_FOR_ALL){

        }

        void setState(PWMConfigStates newState){
            currentState = newState;
        }

        PWMConfigStates getState(){
            return currentState;
        }

        void setRegularPWMState(RegularPWMStates newState){
            regularPWMState = newState;
        }

        RegularPWMStates getRegularPWMState(){
            return regularPWMState;
        }

        void setSharedPWMState(SharedPWMStates newState){
            sharedPWMState = newState;
        }

        SharedPWMStates getSharedPWMState(){
            return sharedPWMState;
        }
};