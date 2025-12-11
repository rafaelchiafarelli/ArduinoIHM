#pragma once

typedef enum {
    TMR5_CONFIG,
    TMR3_CONFIG,
    TMR1_CONFIG,
    TMR4_CONFIG,
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
    ACTIVATE_CHANNEL_C
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