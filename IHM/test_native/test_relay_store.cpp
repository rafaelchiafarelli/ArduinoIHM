#include "mini_test.h"
#include "RelayStore.h"
#include <string.h>

// Relay state across power cycles (fixes/000014): the wear-levelled ring.

namespace {
struct Eeprom {
    uint8_t img[RELAY_STORE_BYTES];
    RelayStoreCursor cur;
    Eeprom() { memset(img, 0xFF, sizeof(img)); uint8_t m; relayStoreFind(img, &m, &cur); }
    void save(uint8_t mask) {
        uint8_t slot, b[RELAY_STORE_SLOT_BYTES];
        relayStoreNext(&cur, mask, &slot, b);
        memcpy(img + slot * RELAY_STORE_SLOT_BYTES, b, RELAY_STORE_SLOT_BYTES);
    }
    // Power cycle: re-scan from the image only.
    bool boot(uint8_t* mask) { return relayStoreFind(img, mask, &cur); }
};
}  // namespace

TEST(RelayStore, BlankEepromRestoresAllOff) {
    Eeprom e;
    uint8_t m = 0xAA;
    CHECK_TRUE(!e.boot(&m));
    CHECK_EQ((int)m, 0);
}

TEST(RelayStore, RestoresTheLastSavedMask) {
    Eeprom e;
    e.save(0x01);
    e.save(0x81);
    e.save(0x80);
    uint8_t m;
    CHECK_TRUE(e.boot(&m));
    CHECK_EQ((int)m, 0x80);
}

TEST(RelayStore, SurvivesManyWrapsOfTheRingAndTheSeq) {
    Eeprom e;
    uint8_t m = 0;
    for (int i = 0; i < 5000; i++) {   // > 255 seqs and > 32 slots, many times
        e.save((uint8_t)i);
        if (i % 97 == 0) {             // power cycle now and then
            CHECK_TRUE(e.boot(&m));
            CHECK_EQ((int)m, i & 0xFF);
        }
    }
    CHECK_TRUE(e.boot(&m));
    CHECK_EQ((int)m, 4999 & 0xFF);
}

TEST(RelayStore, SpreadsWritesOverEverySlot) {
    Eeprom e;
    for (int i = 0; i < RELAY_STORE_SLOTS; i++) e.save(0x0F);
    for (int s = 0; s < RELAY_STORE_SLOTS; s++)
        CHECK_TRUE(e.img[s * RELAY_STORE_SLOT_BYTES + 2] != 0xFF);   // every slot written once
}

TEST(RelayStore, TornWriteKeepsThePreviousState) {
    Eeprom e;
    for (int i = 0; i < 40; i++) e.save(0x11);   // ring wrapped: old slots are valid
    e.save(0x22);
    uint8_t before[RELAY_STORE_BYTES];
    memcpy(before, e.img, sizeof(before));

    // Torn after value + ~value (seq not yet written): the slot keeps an old seq.
    uint8_t slot, b[RELAY_STORE_SLOT_BYTES];
    RelayStoreCursor c = e.cur;
    relayStoreNext(&c, 0x33, &slot, b);
    e.img[slot * 3 + 0] = b[0];
    e.img[slot * 3 + 1] = b[1];
    uint8_t m;
    CHECK_TRUE(e.boot(&m));
    CHECK_EQ((int)m, 0x22);

    // Torn after value only: check byte mismatch, slot invalid.
    memcpy(e.img, before, sizeof(before));
    e.img[slot * 3 + 0] = b[0];
    CHECK_TRUE(e.boot(&m));
    CHECK_EQ((int)m, 0x22);
}

TEST(RelayStore, NextWriteAfterBootContinuesTheRing) {
    Eeprom e;
    e.save(0x05);
    e.save(0x06);
    uint8_t m;
    e.boot(&m);
    e.save(0x07);   // must land after 0x06, not overwrite it as the newest
    CHECK_TRUE(e.boot(&m));
    CHECK_EQ((int)m, 0x07);
}
