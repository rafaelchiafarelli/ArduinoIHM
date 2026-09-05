#ifndef JANUS_GEN_BINDINGS_H
#define JANUS_GEN_BINDINGS_H

#include <stdbool.h>

typedef struct {
    int ch0_enabled;
    int ch0_inverting;
    const char * ch0_mode_icon;
    const char * ch0_state_label;
    int ch0_duty_percent;
    int ch1_enabled;
    int ch1_inverting;
    const char * ch1_mode_icon;
    const char * ch1_state_label;
    int ch1_duty_percent;
    int ch2_a_enabled;
    int ch2_a_duty_percent;
    int ch2_b_enabled;
    int ch2_b_duty_percent;
    int ch2_c_enabled;
    int ch2_c_duty_percent;
    const char * ch2_mode_icon;
    const char * ch2_state_label;
    int ch3_a_enabled;
    int ch3_a_duty_percent;
    int ch3_b_enabled;
    int ch3_b_duty_percent;
    int ch3_c_enabled;
    int ch3_c_duty_percent;
    const char * ch3_mode_icon;
    const char * ch3_state_label;
} pwm_t;

extern pwm_t pwm_instance;

typedef struct {
    bool ch0_enabled;
    bool ch0_inverting;
    bool ch0_mode_icon;
    bool ch0_state_label;
    bool ch0_duty_percent;
    bool ch1_enabled;
    bool ch1_inverting;
    bool ch1_mode_icon;
    bool ch1_state_label;
    bool ch1_duty_percent;
    bool ch2_a_enabled;
    bool ch2_a_duty_percent;
    bool ch2_b_enabled;
    bool ch2_b_duty_percent;
    bool ch2_c_enabled;
    bool ch2_c_duty_percent;
    bool ch2_mode_icon;
    bool ch2_state_label;
    bool ch3_a_enabled;
    bool ch3_a_duty_percent;
    bool ch3_b_enabled;
    bool ch3_b_duty_percent;
    bool ch3_c_enabled;
    bool ch3_c_duty_percent;
    bool ch3_mode_icon;
    bool ch3_state_label;
} pwm_dirty_t;

extern pwm_dirty_t pwm_dirty;

typedef struct {
    int can0_enabled;
    int can0_id;
    int can0_dlc;
    int can0_extended;
    int can1_enabled;
    int can1_id;
    int can1_dlc;
    int can1_extended;
    int rs485_enabled;
    int rs485_length;
    int rs485_period_ms;
} bus_status_t;

extern bus_status_t bus_status_instance;

typedef struct {
    bool can0_enabled;
    bool can0_id;
    bool can0_dlc;
    bool can0_extended;
    bool can1_enabled;
    bool can1_id;
    bool can1_dlc;
    bool can1_extended;
    bool rs485_enabled;
    bool rs485_length;
    bool rs485_period_ms;
} bus_status_dirty_t;

extern bus_status_dirty_t bus_status_dirty;

typedef struct {
    int relay_0;
    int relay_1;
    int relay_2;
    int relay_3;
    int relay_4;
    int relay_5;
    int relay_6;
    int relay_7;
} relay_t;

extern relay_t relay_instance;

typedef struct {
    bool relay_0;
    bool relay_1;
    bool relay_2;
    bool relay_3;
    bool relay_4;
    bool relay_5;
    bool relay_6;
    bool relay_7;
} relay_dirty_t;

extern relay_dirty_t relay_dirty;

#endif  /* JANUS_GEN_BINDINGS_H */
