#include "mini_test.h"
#include "SerialConfigImage.h"
#include <string.h>

// SerialConfig's EEPROM image (serial_config / config_model task 2),
// including the append-only extension rule.

namespace {
SerialConfig sample() {
    SerialConfig c;
    serialConfigDefaults(c);
    c.can[0].gen.enable = 1;
    c.can[0].gen.id = 0x123;
    c.can[0].gen.dlc = 3;
    c.can[0].gen.data[2] = 0xEE;
    c.can[1].gen.extended = 1;
    c.can[1].gen.id = 0x18FEF100;
    c.can[1].gen.period_ms = 1000;
    c.can[1].gen.repeat_count = 42;
    c.rs485.gen.enable = 1;
    c.rs485.gen.length = 32;
    c.rs485.gen.data[31] = 0x7F;
    c.rs485.gen.period_ms = 20;
    return c;
}

// Rewrites the payload length and the CRC after a test edits an image.
void reseal(uint8_t* buf, uint16_t payload) {
    buf[3] = (uint8_t)payload;
    buf[4] = (uint8_t)(payload >> 8);
    uint16_t at = (uint16_t)(SERIAL_CONFIG_IMAGE_HEADER + payload);
    uint16_t crc = serialConfigCrc16(buf, at);
    buf[at] = (uint8_t)crc;
    buf[at + 1] = (uint8_t)(crc >> 8);
}
}  // namespace

TEST(SerialConfigImage, LengthFitsTheReservedArea) {
    SerialConfig c = sample();
    uint8_t buf[SERIAL_CONFIG_IMAGE_LEN];
    CHECK_EQ((int)serialConfigSerialize(c, buf), (int)SERIAL_CONFIG_IMAGE_LEN);
    CHECK_TRUE(SERIAL_CONFIG_IMAGE_LEN <= SERIAL_CONFIG_EEPROM_RESERVED);
}

TEST(SerialConfigImage, Crc16KnownVector) {
    const uint8_t v[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    CHECK_EQ((int)serialConfigCrc16(v, 9), 0x29B1);   // CRC-16/CCITT-FALSE check value
}

TEST(SerialConfigImage, RoundTripKeepsEveryField) {
    SerialConfig c = sample();
    uint8_t buf[SERIAL_CONFIG_IMAGE_LEN];
    uint16_t len = serialConfigSerialize(c, buf);
    SerialConfig back;
    CHECK_TRUE(serialConfigDeserialize(buf, len, &back));
    CHECK_TRUE(memcmp(&c, &back, sizeof(c)) == 0);
}

TEST(SerialConfigImage, BlankEepromGivesDefaults) {
    uint8_t buf[SERIAL_CONFIG_IMAGE_LEN];
    memset(buf, 0xFF, sizeof(buf));
    SerialConfig back, d;
    serialConfigDefaults(d);
    CHECK_TRUE(!serialConfigDeserialize(buf, sizeof(buf), &back));
    CHECK_TRUE(memcmp(&d, &back, sizeof(d)) == 0);
}

TEST(SerialConfigImage, CorruptCrcGivesDefaults) {
    SerialConfig c = sample();
    uint8_t buf[SERIAL_CONFIG_IMAGE_LEN];
    uint16_t len = serialConfigSerialize(c, buf);
    buf[10] ^= 0x01;
    SerialConfig back;
    CHECK_TRUE(!serialConfigDeserialize(buf, len, &back));
    CHECK_EQ((int)back.can[0].gen.enable, 0);
}

TEST(SerialConfigImage, OtherVersionGivesDefaults) {
    SerialConfig c = sample();
    uint8_t buf[SERIAL_CONFIG_IMAGE_LEN];
    uint16_t len = serialConfigSerialize(c, buf);
    buf[2] = SERIAL_CONFIG_IMAGE_VERSION + 1;
    reseal(buf, SERIAL_CONFIG_PAYLOAD_LEN);
    SerialConfig back;
    CHECK_TRUE(!serialConfigDeserialize(buf, len, &back));
    CHECK_EQ((int)back.rs485.gen.enable, 0);
}

TEST(SerialConfigImage, OlderShorterPayloadKeepsItsFieldsAndDefaultsTheRest) {
    // An image saved before the RS-485 section existed: CAN only (38 B).
    SerialConfig c = sample();
    uint8_t buf[SERIAL_CONFIG_IMAGE_LEN];
    serialConfigSerialize(c, buf);
    const uint16_t canOnly = 38;
    reseal(buf, canOnly);
    SerialConfig back;
    CHECK_TRUE(serialConfigDeserialize(buf, (uint16_t)(SERIAL_CONFIG_IMAGE_HEADER + canOnly + SERIAL_CONFIG_IMAGE_CRC), &back));
    CHECK_EQ((uint32_t)back.can[1].gen.id, (uint32_t)0x18FEF100);
    CHECK_EQ((int)back.can[0].gen.data[2], 0xEE);
    CHECK_EQ((int)back.rs485.gen.enable, 0);   // default
    CHECK_EQ((int)back.rs485.gen.length, 8);   // default
}

TEST(SerialConfigImage, NewerLongerPayloadLoadsTheKnownPrefix) {
    SerialConfig c = sample();
    uint8_t buf[SERIAL_CONFIG_IMAGE_LEN + 4];
    serialConfigSerialize(c, buf);
    const uint16_t longer = SERIAL_CONFIG_PAYLOAD_LEN + 4;
    for (int i = 0; i < 4; i++) buf[SERIAL_CONFIG_IMAGE_HEADER + SERIAL_CONFIG_PAYLOAD_LEN + i] = 0x99;
    reseal(buf, longer);
    SerialConfig back;
    CHECK_TRUE(serialConfigDeserialize(buf, sizeof(buf), &back));
    CHECK_TRUE(memcmp(&c, &back, sizeof(c)) == 0);
}

TEST(SerialConfigImage, OutOfRangeSectionFallsBackToItsDefaults) {
    SerialConfig c = sample();
    uint8_t buf[SERIAL_CONFIG_IMAGE_LEN];
    serialConfigSerialize(c, buf);
    buf[SERIAL_CONFIG_IMAGE_HEADER + 6] = 9;   // CAN0 dlc (after enable + id + ext)
    reseal(buf, SERIAL_CONFIG_PAYLOAD_LEN);
    SerialConfig back;
    CHECK_TRUE(serialConfigDeserialize(buf, sizeof(buf), &back));
    CHECK_EQ((int)back.can[0].gen.dlc, 8);      // CAN0 defaulted
    CHECK_EQ((int)back.can[0].gen.enable, 0);
    CHECK_EQ((int)back.can[1].gen.repeat_count, 42);   // CAN1 kept
}

TEST(SerialConfigImage, TruncatedBufferGivesDefaults) {
    SerialConfig c = sample();
    uint8_t buf[SERIAL_CONFIG_IMAGE_LEN];
    serialConfigSerialize(c, buf);
    SerialConfig back;
    CHECK_TRUE(!serialConfigDeserialize(buf, 20, &back));
    CHECK_TRUE(!serialConfigDeserialize(buf, 3, &back));
}
