#include "mini_test.h"
#include "mavlink.h"

// IHM_UI_STATE (308) carries janus_remote_state_t one field per member; the
// PC mirror decodes it and hands it to janus_remote_state_apply(). These
// check the generated pack/decode pair keeps every field, signed ones
// included (-1 = nothing focused).
namespace {
mavlink_ihm_ui_state_t roundTrip(uint16_t screen, int16_t focus, int16_t navFocus, uint16_t boxes) {
    mavlink_message_t msg;
    mavlink_msg_ihm_ui_state_pack(1, 1, &msg, screen, focus, navFocus, boxes);
    mavlink_ihm_ui_state_t out;
    mavlink_msg_ihm_ui_state_decode(&msg, &out);
    return out;
}
}  // namespace

TEST(UiStateMessage, IdIs308) {
    mavlink_message_t msg;
    mavlink_msg_ihm_ui_state_pack(1, 1, &msg, 0, 0, -1, 0);
    CHECK_EQ((uint32_t)msg.msgid, (uint32_t)308);
}

TEST(UiStateMessage, RoundTripsEveryField) {
    mavlink_ihm_ui_state_t s = roundTrip(2, 5, -1, 0xA5C3);
    CHECK_EQ((int)s.screen, 2);
    CHECK_EQ((int)s.focus, 5);
    CHECK_EQ((int)s.nav_focus, -1);
    CHECK_EQ((int)s.boxes_expanded, 0xA5C3);
}

TEST(UiStateMessage, KeepsNothingFocusedAndNavFocus) {
    mavlink_ihm_ui_state_t s = roundTrip(0, -1, 1, 0);
    CHECK_EQ((int)s.focus, -1);
    CHECK_EQ((int)s.nav_focus, 1);
}
