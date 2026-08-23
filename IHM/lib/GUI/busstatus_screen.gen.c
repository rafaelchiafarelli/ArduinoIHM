#include "busstatus_screen.gen.h"
#include "janus_actions.gen.h"
#include "janus_bindings.gen.h"
static const char busstatus_str1[] JANUS_PROGMEM = "SERIAL";

static const char busstatus_str2[] JANUS_PROGMEM = "bus_status_header";

static const char busstatus_str3[] JANUS_PROGMEM = "can0_led";

static const char busstatus_str4[] JANUS_PROGMEM = "CAN0";

static const char busstatus_str5[] JANUS_PROGMEM = "can0_label";

static const char busstatus_str6[] JANUS_PROGMEM = "can0_id";

static const char busstatus_str7[] JANUS_PROGMEM = "can0_dlc";

static const char busstatus_str8[] JANUS_PROGMEM = "can0_extended";

static const janus_widget_desc_t busstatus_arr1[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LED, .id = busstatus_str3, .static_text = NULL, .geometry = {0, 36, 10, 10}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can0_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str5, .static_text = busstatus_str4, .geometry = {14, 36, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str6, .static_text = NULL, .geometry = {78, 36, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can0_id), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str7, .static_text = NULL, .geometry = {142, 36, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can0_dlc), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str8, .static_text = NULL, .geometry = {206, 36, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can0_extended), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char busstatus_str9[] JANUS_PROGMEM = "can0_box";

static const char busstatus_str10[] JANUS_PROGMEM = "can1_led";

static const char busstatus_str11[] JANUS_PROGMEM = "CAN1";

static const char busstatus_str12[] JANUS_PROGMEM = "can1_label";

static const char busstatus_str13[] JANUS_PROGMEM = "can1_id";

static const char busstatus_str14[] JANUS_PROGMEM = "can1_dlc";

static const char busstatus_str15[] JANUS_PROGMEM = "can1_extended";

static const janus_widget_desc_t busstatus_arr2[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LED, .id = busstatus_str10, .static_text = NULL, .geometry = {0, 68, 10, 10}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can1_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str12, .static_text = busstatus_str11, .geometry = {14, 68, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str13, .static_text = NULL, .geometry = {78, 68, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can1_id), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str14, .static_text = NULL, .geometry = {142, 68, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can1_dlc), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str15, .static_text = NULL, .geometry = {206, 68, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, can1_extended), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char busstatus_str16[] JANUS_PROGMEM = "can1_box";

static const char busstatus_str17[] JANUS_PROGMEM = "rs485_led";

static const char busstatus_str18[] JANUS_PROGMEM = "RS485";

static const char busstatus_str19[] JANUS_PROGMEM = "rs485_label";

static const char busstatus_str20[] JANUS_PROGMEM = "rs485_length";

static const char busstatus_str21[] JANUS_PROGMEM = "rs485_period_ms";

static const janus_widget_desc_t busstatus_arr3[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_LED, .id = busstatus_str17, .static_text = NULL, .geometry = {0, 100, 10, 10}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, rs485_enabled), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str19, .static_text = busstatus_str18, .geometry = {14, 100, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str20, .static_text = NULL, .geometry = {78, 100, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, rs485_length), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_LABEL, .id = busstatus_str21, .static_text = NULL, .geometry = {142, 100, 60, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(bus_status_t, rs485_period_ms), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 }
};

static const char busstatus_str22[] JANUS_PROGMEM = "rs485_box";

static const janus_widget_desc_t busstatus_widgets[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_HEADER, .id = busstatus_str2, .static_text = busstatus_str1, .geometry = {0, 0, 80, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = NULL, .child_count = 0 },
    { .kind = JANUS_WIDGET_BOX, .id = busstatus_str9, .static_text = NULL, .geometry = {0, 20, 266, 28}, .geometry_collapsed = {0, 20, 266, 16}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 0, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = busstatus_arr1, .child_count = 5 },
    { .kind = JANUS_WIDGET_BOX, .id = busstatus_str16, .static_text = NULL, .geometry = {0, 52, 266, 28}, .geometry_collapsed = {0, 52, 266, 16}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 1, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = busstatus_arr2, .child_count = 5 },
    { .kind = JANUS_WIDGET_BOX, .id = busstatus_str22, .static_text = NULL, .geometry = {0, 84, 202, 28}, .geometry_collapsed = {0, 84, 202, 16}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 2, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .children = busstatus_arr3, .child_count = 4 }
};

static const char busstatus_str23[] JANUS_PROGMEM = "BusStatus";

const janus_screen_desc_t busstatus_screen JANUS_PROGMEM = {
    .name = busstatus_str23,
    .widgets = busstatus_widgets,
    .widget_count = 4,
    .bound_struct = &bus_status_instance,
};
