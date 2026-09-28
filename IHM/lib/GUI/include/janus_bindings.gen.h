#ifndef JANUS_GEN_BINDINGS_H
#define JANUS_GEN_BINDINGS_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    const char * ch0_state_label;
    int ch0_enabled;
    int ch0_inverting;
    int ch0_duty_percent;
    const char * ch0_inverting_label;
    const char * ch1_state_label;
    int ch1_enabled;
    int ch1_inverting;
    int ch1_duty_percent;
    const char * ch1_inverting_label;
    const char * ch2_state_label;
    int ch2_a_enabled;
    int ch2_a_duty_percent;
    int ch2_b_enabled;
    int ch2_b_duty_percent;
    int ch2_c_enabled;
    int ch2_c_duty_percent;
    const char * ch3_state_label;
    int ch3_a_enabled;
    int ch3_a_duty_percent;
    int ch3_b_enabled;
    int ch3_b_duty_percent;
    int ch3_c_enabled;
    int ch3_c_duty_percent;
} pwm_t;

extern pwm_t pwm_instance;

typedef struct {
    bool ch0_state_label;
    bool ch0_enabled;
    bool ch0_inverting;
    bool ch0_duty_percent;
    bool ch0_inverting_label;
    bool ch1_state_label;
    bool ch1_enabled;
    bool ch1_inverting;
    bool ch1_duty_percent;
    bool ch1_inverting_label;
    bool ch2_state_label;
    bool ch2_a_enabled;
    bool ch2_a_duty_percent;
    bool ch2_b_enabled;
    bool ch2_b_duty_percent;
    bool ch2_c_enabled;
    bool ch2_c_duty_percent;
    bool ch3_state_label;
    bool ch3_a_enabled;
    bool ch3_a_duty_percent;
    bool ch3_b_enabled;
    bool ch3_b_duty_percent;
    bool ch3_c_enabled;
    bool ch3_c_duty_percent;
} pwm_dirty_t;

extern pwm_dirty_t pwm_dirty;

typedef struct {
    int can0_enabled;
    int64_t can0_id;
    int can0_dlc;
    int can0_extended;
    int64_t can0_period_ms;
    const char * can0_repeat_label;
    int can0_byte_index;
    int can0_byte_value;
    int can1_enabled;
    int64_t can1_id;
    int can1_dlc;
    int can1_extended;
    int64_t can1_period_ms;
    const char * can1_repeat_label;
    int can1_byte_index;
    int can1_byte_value;
    int rs485_enabled;
    int rs485_length;
    int64_t rs485_period_ms;
    const char * rs485_repeat_label;
    int rs485_byte_index;
    int rs485_byte_value;
} bus_status_t;

extern bus_status_t bus_status_instance;

typedef struct {
    bool can0_enabled;
    bool can0_id;
    bool can0_dlc;
    bool can0_extended;
    bool can0_period_ms;
    bool can0_repeat_label;
    bool can0_byte_index;
    bool can0_byte_value;
    bool can1_enabled;
    bool can1_id;
    bool can1_dlc;
    bool can1_extended;
    bool can1_period_ms;
    bool can1_repeat_label;
    bool can1_byte_index;
    bool can1_byte_value;
    bool rs485_enabled;
    bool rs485_length;
    bool rs485_period_ms;
    bool rs485_repeat_label;
    bool rs485_byte_index;
    bool rs485_byte_value;
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
