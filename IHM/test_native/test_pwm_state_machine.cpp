#include "mini_test.h"
#include "PWMStateMachine.h"

// -- construction -------------------------------------------------------

TEST(PWMStateMachine, DefaultsToChannel0ModeField) {
    PWMStateMachine sm;
    CHECK_EQ(sm.getSelectedChannel(), PWM_0);
    CHECK_EQ(sm.getRegularField(), CONFIG_MODE);
    CHECK_EQ(sm.getSharedField(), CONFIG_MODE_FOR_ALL);
    CHECK_FALSE(sm.isComplexChannel());
}

// -- channel selection: every transition, both directions ---------------

TEST(PWMStateMachine, SelectNextChannelCyclesThroughAllFourAndWraps) {
    PWMStateMachine sm;
    CHECK_EQ(sm.getSelectedChannel(), PWM_0);
    sm.selectNextChannel();
    CHECK_EQ(sm.getSelectedChannel(), PWM_1);
    sm.selectNextChannel();
    CHECK_EQ(sm.getSelectedChannel(), PWM_2);
    sm.selectNextChannel();
    CHECK_EQ(sm.getSelectedChannel(), PWM_3);
    sm.selectNextChannel();
    CHECK_EQ(sm.getSelectedChannel(), PWM_0);  // wraps
}

TEST(PWMStateMachine, SelectPreviousChannelCyclesBackwardAndWraps) {
    PWMStateMachine sm;
    sm.selectPreviousChannel();
    CHECK_EQ(sm.getSelectedChannel(), PWM_3);  // wraps below PWM_0
    sm.selectPreviousChannel();
    CHECK_EQ(sm.getSelectedChannel(), PWM_2);
    sm.selectPreviousChannel();
    CHECK_EQ(sm.getSelectedChannel(), PWM_1);
    sm.selectPreviousChannel();
    CHECK_EQ(sm.getSelectedChannel(), PWM_0);
}

TEST(PWMStateMachine, SelectChannelJumpsDirectlyToAValidChannel) {
    PWMStateMachine sm;
    sm.selectChannel(PWM_2);
    CHECK_EQ(sm.getSelectedChannel(), PWM_2);
    sm.selectChannel(PWM_0);
    CHECK_EQ(sm.getSelectedChannel(), PWM_0);
}

TEST(PWMStateMachine, SelectChannelIgnoresOutOfRangeValue) {
    PWMStateMachine sm;
    sm.selectChannel(PWM_3);
    sm.selectChannel(NUMBER_OF_PWM_CHANNELS);  // out of range: must be a no-op
    CHECK_EQ(sm.getSelectedChannel(), PWM_3);
}

// -- isComplexChannel: every channel ------------------------------------

TEST(PWMStateMachine, IsComplexChannelMatchesHardwareTopology) {
    PWMStateMachine sm;
    sm.selectChannel(PWM_0);
    CHECK_FALSE(sm.isComplexChannel());
    sm.selectChannel(PWM_1);
    CHECK_FALSE(sm.isComplexChannel());
    sm.selectChannel(PWM_2);
    CHECK_TRUE(sm.isComplexChannel());
    sm.selectChannel(PWM_3);
    CHECK_TRUE(sm.isComplexChannel());
}

// -- regular (simplex) field cursor: every transition --------------------

TEST(PWMStateMachine, SelectNextRegularFieldCyclesThroughAllFourAndWraps) {
    PWMStateMachine sm;
    CHECK_EQ(sm.getRegularField(), CONFIG_MODE);
    sm.selectNextRegularField();
    CHECK_EQ(sm.getRegularField(), CONFIG_FREQUENCY);
    sm.selectNextRegularField();
    CHECK_EQ(sm.getRegularField(), CONFIG_DUTY_CYCLE);
    sm.selectNextRegularField();
    CHECK_EQ(sm.getRegularField(), CONFIG_EDGE);
    sm.selectNextRegularField();
    CHECK_EQ(sm.getRegularField(), CONFIG_MODE);  // wraps
}

TEST(PWMStateMachine, SelectPreviousRegularFieldCyclesBackwardAndWraps) {
    PWMStateMachine sm;
    sm.selectPreviousRegularField();
    CHECK_EQ(sm.getRegularField(), CONFIG_EDGE);  // wraps below CONFIG_MODE
    sm.selectPreviousRegularField();
    CHECK_EQ(sm.getRegularField(), CONFIG_DUTY_CYCLE);
    sm.selectPreviousRegularField();
    CHECK_EQ(sm.getRegularField(), CONFIG_FREQUENCY);
    sm.selectPreviousRegularField();
    CHECK_EQ(sm.getRegularField(), CONFIG_MODE);
}

TEST(PWMStateMachine, SetRegularFieldAcceptsValidFieldAndIgnoresInvalid) {
    PWMStateMachine sm;
    sm.setRegularField(CONFIG_DUTY_CYCLE);
    CHECK_EQ(sm.getRegularField(), CONFIG_DUTY_CYCLE);
    sm.setRegularField(NUMBER_OF_REGULAR_PWM_STATES);  // out of range: no-op
    CHECK_EQ(sm.getRegularField(), CONFIG_DUTY_CYCLE);
}

// -- shared (complex) field cursor: every transition ----------------------

TEST(PWMStateMachine, SelectNextSharedFieldCyclesThroughAllElevenAndWraps) {
    PWMStateMachine sm;
    const SharedPWMStates order[] = {
        CONFIG_MODE_FOR_ALL, CONFIG_FREQUENCY_FOR_ALL,
        CONFIG_DUTY_CYCLE_FOR_A, CONFIG_EDGE_FOR_A, ACTIVATE_CHANNEL_A,
        CONFIG_DUTY_CYCLE_FOR_B, CONFIG_EDGE_FOR_B, ACTIVATE_CHANNEL_B,
        CONFIG_DUTY_CYCLE_FOR_C, CONFIG_EDGE_FOR_C, ACTIVATE_CHANNEL_C,
    };
    const int n = sizeof(order) / sizeof(order[0]);
    CHECK_EQ(n, (int)NUMBER_OF_SHARED_PWM_STATES);

    CHECK_EQ(sm.getSharedField(), order[0]);
    for (int i = 1; i < n; i++) {
        sm.selectNextSharedField();
        CHECK_EQ(sm.getSharedField(), order[i]);
    }
    sm.selectNextSharedField();
    CHECK_EQ(sm.getSharedField(), order[0]);  // wraps
}

TEST(PWMStateMachine, SelectPreviousSharedFieldWrapsBelowFirst) {
    PWMStateMachine sm;
    sm.selectPreviousSharedField();
    CHECK_EQ(sm.getSharedField(), ACTIVATE_CHANNEL_C);  // last entry
    sm.selectNextSharedField();
    CHECK_EQ(sm.getSharedField(), CONFIG_MODE_FOR_ALL);  // back to first
}

TEST(PWMStateMachine, SelectPreviousSharedFieldCyclesBackwardThroughAllEleven) {
    PWMStateMachine sm;
    const SharedPWMStates reverseOrder[] = {
        ACTIVATE_CHANNEL_C, CONFIG_EDGE_FOR_C, CONFIG_DUTY_CYCLE_FOR_C,
        ACTIVATE_CHANNEL_B, CONFIG_EDGE_FOR_B, CONFIG_DUTY_CYCLE_FOR_B,
        ACTIVATE_CHANNEL_A, CONFIG_EDGE_FOR_A, CONFIG_DUTY_CYCLE_FOR_A,
        CONFIG_FREQUENCY_FOR_ALL, CONFIG_MODE_FOR_ALL,
    };
    const int n = sizeof(reverseOrder) / sizeof(reverseOrder[0]);
    CHECK_EQ(n, (int)NUMBER_OF_SHARED_PWM_STATES);

    for (int i = 0; i < n; i++) {
        sm.selectPreviousSharedField();
        CHECK_EQ(sm.getSharedField(), reverseOrder[i]);
    }
}

TEST(PWMStateMachine, SetSharedFieldAcceptsValidFieldAndIgnoresInvalid) {
    PWMStateMachine sm;
    sm.setSharedField(ACTIVATE_CHANNEL_B);
    CHECK_EQ(sm.getSharedField(), ACTIVATE_CHANNEL_B);
    sm.setSharedField(NUMBER_OF_SHARED_PWM_STATES);  // out of range: no-op
    CHECK_EQ(sm.getSharedField(), ACTIVATE_CHANNEL_B);
}

// -- resetFields ----------------------------------------------------------

TEST(PWMStateMachine, ResetFieldsRestoresBothCursorsWithoutChangingChannel) {
    PWMStateMachine sm;
    sm.selectChannel(PWM_3);
    sm.setRegularField(CONFIG_EDGE);
    sm.setSharedField(ACTIVATE_CHANNEL_C);

    sm.resetFields();

    CHECK_EQ(sm.getSelectedChannel(), PWM_3);  // unchanged
    CHECK_EQ(sm.getRegularField(), CONFIG_MODE);
    CHECK_EQ(sm.getSharedField(), CONFIG_MODE_FOR_ALL);
}

// -- channel selection and field cursors are independent -------------------

TEST(PWMStateMachine, ChannelAndFieldCursorsAreIndependent) {
    PWMStateMachine sm;
    sm.setRegularField(CONFIG_FREQUENCY);
    sm.setSharedField(CONFIG_EDGE_FOR_B);
    sm.selectNextChannel();
    sm.selectNextChannel();
    CHECK_EQ(sm.getSelectedChannel(), PWM_2);
    CHECK_EQ(sm.getRegularField(), CONFIG_FREQUENCY);  // untouched by channel change
    CHECK_EQ(sm.getSharedField(), CONFIG_EDGE_FOR_B);  // untouched by channel change
}
