#include "relay_screen.gen.h"
#include "janus_actions.gen.h"
#include "janus_bindings.gen.h"
static const char relay_str1[] JANUS_PROGMEM = "relay_0";

static const char relay_str2[] JANUS_PROGMEM = "Relay 0";

static const char relay_str3[] JANUS_PROGMEM = "relay_0_label";

static const char relay_str4[] JANUS_PROGMEM = "relay_0_led";

static const janus_widget_desc_t relay_arr1[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str1, .static_text = NULL, .text_is_format = false, .geometry = {6, 64, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_0), .dirty_offset = offsetof(relay_dirty_t, relay_0), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_0, .navigate_target = -1, .focus_order = 0, .focus_ring = 1, .knob_off_color = 0xf800, .knob_on_color = 0x07e0, .knob_flags = JANUS_KNOB_OFF_SET | JANUS_KNOB_ON_SET, .color = 0xd69a, .bg_color = 0xd69a, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str3, .static_text = relay_str2, .text_is_format = false, .geometry = {34, 54, 260, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LED, .id = relay_str4, .static_text = NULL, .text_is_format = false, .geometry = {298, 62, 16, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_0), .dirty_offset = offsetof(relay_dirty_t, relay_0), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = 0x07e0, .bg_color = 0x8410, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str5[] JANUS_PROGMEM = "";

static const char relay_str6[] JANUS_PROGMEM = "relay_1";

static const char relay_str7[] JANUS_PROGMEM = "Relay 1";

static const char relay_str8[] JANUS_PROGMEM = "relay_1_label";

static const char relay_str9[] JANUS_PROGMEM = "relay_1_led";

static const janus_widget_desc_t relay_arr2[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str6, .static_text = NULL, .text_is_format = false, .geometry = {6, 112, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_1), .dirty_offset = offsetof(relay_dirty_t, relay_1), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_1, .navigate_target = -1, .focus_order = 1, .focus_ring = 2, .knob_off_color = 0xf800, .knob_on_color = 0x07e0, .knob_flags = JANUS_KNOB_OFF_SET | JANUS_KNOB_ON_SET, .color = 0xd69a, .bg_color = 0xd69a, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str8, .static_text = relay_str7, .text_is_format = false, .geometry = {34, 102, 260, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LED, .id = relay_str9, .static_text = NULL, .text_is_format = false, .geometry = {298, 110, 16, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_1), .dirty_offset = offsetof(relay_dirty_t, relay_1), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = 0x07e0, .bg_color = 0x8410, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str10[] JANUS_PROGMEM = "";

static const char relay_str11[] JANUS_PROGMEM = "relay_2";

static const char relay_str12[] JANUS_PROGMEM = "Relay 2";

static const char relay_str13[] JANUS_PROGMEM = "relay_2_label";

static const char relay_str14[] JANUS_PROGMEM = "relay_2_led";

static const janus_widget_desc_t relay_arr3[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str11, .static_text = NULL, .text_is_format = false, .geometry = {6, 160, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_2), .dirty_offset = offsetof(relay_dirty_t, relay_2), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_2, .navigate_target = -1, .focus_order = 2, .focus_ring = 3, .knob_off_color = 0xf800, .knob_on_color = 0x07e0, .knob_flags = JANUS_KNOB_OFF_SET | JANUS_KNOB_ON_SET, .color = 0xd69a, .bg_color = 0xd69a, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str13, .static_text = relay_str12, .text_is_format = false, .geometry = {34, 150, 260, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LED, .id = relay_str14, .static_text = NULL, .text_is_format = false, .geometry = {298, 158, 16, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_2), .dirty_offset = offsetof(relay_dirty_t, relay_2), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = 0x07e0, .bg_color = 0x8410, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str15[] JANUS_PROGMEM = "";

static const char relay_str16[] JANUS_PROGMEM = "relay_3";

static const char relay_str17[] JANUS_PROGMEM = "Relay 3";

static const char relay_str18[] JANUS_PROGMEM = "relay_3_label";

static const char relay_str19[] JANUS_PROGMEM = "relay_3_led";

static const janus_widget_desc_t relay_arr4[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str16, .static_text = NULL, .text_is_format = false, .geometry = {6, 208, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_3), .dirty_offset = offsetof(relay_dirty_t, relay_3), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_3, .navigate_target = -1, .focus_order = 3, .focus_ring = 4, .knob_off_color = 0xf800, .knob_on_color = 0x07e0, .knob_flags = JANUS_KNOB_OFF_SET | JANUS_KNOB_ON_SET, .color = 0xd69a, .bg_color = 0xd69a, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str18, .static_text = relay_str17, .text_is_format = false, .geometry = {34, 198, 260, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LED, .id = relay_str19, .static_text = NULL, .text_is_format = false, .geometry = {298, 206, 16, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_3), .dirty_offset = offsetof(relay_dirty_t, relay_3), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = 0x07e0, .bg_color = 0x8410, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str20[] JANUS_PROGMEM = "";

static const char relay_str21[] JANUS_PROGMEM = "relay_4";

static const char relay_str22[] JANUS_PROGMEM = "Relay 4";

static const char relay_str23[] JANUS_PROGMEM = "relay_4_label";

static const char relay_str24[] JANUS_PROGMEM = "relay_4_led";

static const janus_widget_desc_t relay_arr5[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str21, .static_text = NULL, .text_is_format = false, .geometry = {6, 256, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_4), .dirty_offset = offsetof(relay_dirty_t, relay_4), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_4, .navigate_target = -1, .focus_order = 4, .focus_ring = 5, .knob_off_color = 0xf800, .knob_on_color = 0x07e0, .knob_flags = JANUS_KNOB_OFF_SET | JANUS_KNOB_ON_SET, .color = 0xd69a, .bg_color = 0xd69a, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str23, .static_text = relay_str22, .text_is_format = false, .geometry = {34, 246, 260, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LED, .id = relay_str24, .static_text = NULL, .text_is_format = false, .geometry = {298, 254, 16, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_4), .dirty_offset = offsetof(relay_dirty_t, relay_4), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = 0x07e0, .bg_color = 0x8410, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str25[] JANUS_PROGMEM = "";

static const char relay_str26[] JANUS_PROGMEM = "relay_5";

static const char relay_str27[] JANUS_PROGMEM = "Relay 5";

static const char relay_str28[] JANUS_PROGMEM = "relay_5_label";

static const char relay_str29[] JANUS_PROGMEM = "relay_5_led";

static const janus_widget_desc_t relay_arr6[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str26, .static_text = NULL, .text_is_format = false, .geometry = {6, 304, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_5), .dirty_offset = offsetof(relay_dirty_t, relay_5), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_5, .navigate_target = -1, .focus_order = 5, .focus_ring = 6, .knob_off_color = 0xf800, .knob_on_color = 0x07e0, .knob_flags = JANUS_KNOB_OFF_SET | JANUS_KNOB_ON_SET, .color = 0xd69a, .bg_color = 0xd69a, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str28, .static_text = relay_str27, .text_is_format = false, .geometry = {34, 294, 260, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LED, .id = relay_str29, .static_text = NULL, .text_is_format = false, .geometry = {298, 302, 16, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_5), .dirty_offset = offsetof(relay_dirty_t, relay_5), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = 0x07e0, .bg_color = 0x8410, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str30[] JANUS_PROGMEM = "";

static const char relay_str31[] JANUS_PROGMEM = "relay_6";

static const char relay_str32[] JANUS_PROGMEM = "Relay 6";

static const char relay_str33[] JANUS_PROGMEM = "relay_6_label";

static const char relay_str34[] JANUS_PROGMEM = "relay_6_led";

static const janus_widget_desc_t relay_arr7[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str31, .static_text = NULL, .text_is_format = false, .geometry = {6, 352, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_6), .dirty_offset = offsetof(relay_dirty_t, relay_6), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_6, .navigate_target = -1, .focus_order = 6, .focus_ring = 7, .knob_off_color = 0xf800, .knob_on_color = 0x07e0, .knob_flags = JANUS_KNOB_OFF_SET | JANUS_KNOB_ON_SET, .color = 0xd69a, .bg_color = 0xd69a, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str33, .static_text = relay_str32, .text_is_format = false, .geometry = {34, 342, 260, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LED, .id = relay_str34, .static_text = NULL, .text_is_format = false, .geometry = {298, 350, 16, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_6), .dirty_offset = offsetof(relay_dirty_t, relay_6), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = 0x07e0, .bg_color = 0x8410, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str35[] JANUS_PROGMEM = "";

static const char relay_str36[] JANUS_PROGMEM = "relay_7";

static const char relay_str37[] JANUS_PROGMEM = "Relay 7";

static const char relay_str38[] JANUS_PROGMEM = "relay_7_label";

static const char relay_str39[] JANUS_PROGMEM = "relay_7_led";

static const janus_widget_desc_t relay_arr8[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_TOGGLE, .id = relay_str36, .static_text = NULL, .text_is_format = false, .geometry = {6, 400, 24, 12}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_7), .dirty_offset = offsetof(relay_dirty_t, relay_7), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_TOGGLE_RELAY_7, .navigate_target = -1, .focus_order = 7, .focus_ring = 8, .knob_off_color = 0xf800, .knob_on_color = 0x07e0, .knob_flags = JANUS_KNOB_OFF_SET | JANUS_KNOB_ON_SET, .color = 0xd69a, .bg_color = 0xd69a, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LABEL, .id = relay_str38, .static_text = relay_str37, .text_is_format = false, .geometry = {34, 390, 260, 32}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_LED, .id = relay_str39, .static_text = NULL, .text_is_format = false, .geometry = {298, 398, 16, 16}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_offset = offsetof(relay_t, relay_7), .dirty_offset = offsetof(relay_dirty_t, relay_7), .field_type = JANUS_FIELD_INT, .range_min = 0, .range_max = 0 }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = 0x07e0, .bg_color = 0x8410, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = NULL, .child_count = 0, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str40[] JANUS_PROGMEM = "";

static const janus_widget_desc_t relay_arr9[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_ROW, .id = relay_str5, .static_text = NULL, .text_is_format = false, .geometry = {0, 48, 320, 44}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr1, .child_count = 3, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str10, .static_text = NULL, .text_is_format = false, .geometry = {0, 96, 320, 44}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr2, .child_count = 3, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str15, .static_text = NULL, .text_is_format = false, .geometry = {0, 144, 320, 44}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr3, .child_count = 3, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str20, .static_text = NULL, .text_is_format = false, .geometry = {0, 192, 320, 44}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr4, .child_count = 3, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str25, .static_text = NULL, .text_is_format = false, .geometry = {0, 240, 320, 44}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr5, .child_count = 3, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str30, .static_text = NULL, .text_is_format = false, .geometry = {0, 288, 320, 44}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr6, .child_count = 3, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str35, .static_text = NULL, .text_is_format = false, .geometry = {0, 336, 320, 44}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr7, .child_count = 3, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false },
    { .kind = JANUS_WIDGET_ROW, .id = relay_str40, .static_text = NULL, .text_is_format = false, .geometry = {0, 384, 320, 44}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr8, .child_count = 3, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const char relay_str41[] JANUS_PROGMEM = "relay_list";

static const janus_widget_desc_t relay_widgets[] JANUS_PROGMEM = {
    { .kind = JANUS_WIDGET_COLUMN, .id = relay_str41, .static_text = NULL, .text_is_format = false, .geometry = {0, 48, 320, 432}, .geometry_collapsed = {0, 0, 0, 0}, .initial_expanded = true, .bind = { .field_type = JANUS_FIELD_NONE }, .action = JANUS_ACTION_NONE, .navigate_target = -1, .focus_order = 255, .color = JANUS_COLOR_DEFAULT_FG, .bg_color = JANUS_COLOR_DEFAULT_BG, .font_size = JANUS_FONT_SIZE_LARGE, .font_scale = 1, .children = relay_arr9, .child_count = 8, .summary_children = NULL, .summary_child_count = 0, .image_slot = 0, .image_w = 0, .image_h = 0, .image_error = false }
};

static const janus_focus_ring_t relay_focus_rings[] JANUS_PROGMEM = {
    { .rect = {0, 48, 320, 44}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {0, 96, 320, 44}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {0, 144, 320, 44}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {0, 192, 320, 44}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {0, 240, 320, 44}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {0, 288, 320, 44}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {0, 336, 320, 44}, .bg_color = JANUS_COLOR_DEFAULT_BG },
    { .rect = {0, 384, 320, 44}, .bg_color = JANUS_COLOR_DEFAULT_BG }
};

static const char relay_str42[] JANUS_PROGMEM = "Relay";

const janus_screen_desc_t relay_screen JANUS_PROGMEM = {
    .name = relay_str42,
    .widgets = relay_widgets,
    .widget_count = 1,
    .bound_struct = &relay_instance,
    .bound_dirty = &relay_dirty,
    .resolve_images = NULL,
    .image_far = NULL,
    .focus_rings = relay_focus_rings,
};
