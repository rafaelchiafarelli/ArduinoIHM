#pragma once
#include <stdint.h>
/**
 * Tracks which of the 4 PWM output channels is currently selected on screen,
 * and which configuration field within that channel is selected. This class
 * holds no hardware state -- it is pure UI-cursor bookkeeping. GUI/PWMScreen
 * drive it from encoder input, and the actual timer register writes happen
 * separately in PWM::setupPWMChannelN() once a configuration is confirmed.
 *
 * Channel numbering matches PWM::setupPWMChannelN() (see MultiOutput/src/PWM.h)
 * and PWMScreen's on-screen slot order (see Elements/src/PWMScreen.h):
 *   PWM_0 -> setupPWMChannel0 -> Timer3, 1 output  (PWMSimplex)
 *   PWM_1 -> setupPWMChannel1 -> Timer5, 1 output  (PWMSimplex)
 *   PWM_2 -> setupPWMChannel2 -> Timer1, 3 outputs (PWMComplex, A/B/C)
 *   PWM_3 -> setupPWMChannel3 -> Timer4, 3 outputs (PWMComplex, A/B/C)
 */

typedef enum {
    PWM_0,
    PWM_1,
    PWM_2,
    PWM_3,
    NUMBER_OF_PWM_CHANNELS
} PWMConfigStates;

typedef enum {
    CONFIG_MODE,
    CONFIG_FREQUENCY,
    CONFIG_DUTY_CYCLE,
    CONFIG_EDGE,
    NUMBER_OF_REGULAR_PWM_STATES
} RegularPWMStates;

/**
 * Field cursor for the complex (3-output) channels, PWM_2 and PWM_3.
 * Only outputs A, B and C exist in hardware -- see OutputType::AmountOfOutput
 * in PWMComplex.h -- so unlike an earlier draft of this enum, there are no
 * D/E/F entries here; a complex channel never has more than 3 outputs.
 */
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

    NUMBER_OF_SHARED_PWM_STATES
} SharedPWMStates;

class PWMStateMachine {
    private:
        PWMConfigStates currentChannel;
        RegularPWMStates regularField;
        SharedPWMStates sharedField;

    public:
        PWMStateMachine():
            currentChannel(PWM_0),
            regularField(CONFIG_MODE),
            sharedField(CONFIG_MODE_FOR_ALL){
        }

        // -- which of the 4 PWM channels is selected --------------------
        PWMConfigStates getSelectedChannel() const { return currentChannel; }

        void selectChannel(PWMConfigStates channel){
            if(channel < NUMBER_OF_PWM_CHANNELS){
                currentChannel = channel;
            }
        }

        void selectNextChannel(){
            currentChannel = (PWMConfigStates)((currentChannel + 1) % NUMBER_OF_PWM_CHANNELS);
        }

        void selectPreviousChannel(){
            currentChannel = (PWMConfigStates)((currentChannel + NUMBER_OF_PWM_CHANNELS - 1) % NUMBER_OF_PWM_CHANNELS);
        }

        // PWM_0/PWM_1 are single-output (PWMSimplex); PWM_2/PWM_3 have
        // three outputs A/B/C (PWMComplex).
        bool isComplexChannel() const {
            return currentChannel == PWM_2 || currentChannel == PWM_3;
        }

        // -- field cursor for simplex channels (PWM_0 / PWM_1) ----------
        RegularPWMStates getRegularField() const { return regularField; }

        void setRegularField(RegularPWMStates field){
            if(field < NUMBER_OF_REGULAR_PWM_STATES){
                regularField = field;
            }
        }

        void selectNextRegularField(){
            regularField = (RegularPWMStates)((regularField + 1) % NUMBER_OF_REGULAR_PWM_STATES);
        }

        void selectPreviousRegularField(){
            regularField = (RegularPWMStates)((regularField + NUMBER_OF_REGULAR_PWM_STATES - 1) % NUMBER_OF_REGULAR_PWM_STATES);
        }

        // -- field cursor for complex channels (PWM_2 / PWM_3) ----------
        SharedPWMStates getSharedField() const { return sharedField; }

        void setSharedField(SharedPWMStates field){
            if(field < NUMBER_OF_SHARED_PWM_STATES){
                sharedField = field;
            }
        }

        void selectNextSharedField(){
            sharedField = (SharedPWMStates)((sharedField + 1) % NUMBER_OF_SHARED_PWM_STATES);
        }

        void selectPreviousSharedField(){
            sharedField = (SharedPWMStates)((sharedField + NUMBER_OF_SHARED_PWM_STATES - 1) % NUMBER_OF_SHARED_PWM_STATES);
        }

        // Returns each channel's field cursor to its first field, without
        // changing which channel is selected.
        void resetFields(){
            regularField = CONFIG_MODE;
            sharedField = CONFIG_MODE_FOR_ALL;
        }
};
