#include "RelayStore.h"

namespace {

const uint8_t kSeqModulo = 255;   // 0..254; 0xFF is erased EEPROM

bool slotValid(const uint8_t* image, uint8_t slot)
{
    const uint8_t* s = image + slot * RELAY_STORE_SLOT_BYTES;
    return s[2] != 0xFF && s[0] == (uint8_t)~s[1];
}

uint8_t slotSeq(const uint8_t* image, uint8_t slot)
{
    return image[slot * RELAY_STORE_SLOT_BYTES + 2];
}

}  // namespace

bool relayStoreFind(const uint8_t* image, uint8_t* mask, RelayStoreCursor* cursor)
{
    *mask = 0;
    cursor->slot = RELAY_STORE_SLOTS - 1;   // so the first write lands in slot 0
    cursor->seq = kSeqModulo - 1;           // ... with seq 0
    cursor->valid = 0;

    for (uint8_t i = 0; i < RELAY_STORE_SLOTS; i++) {
        if (!slotValid(image, i))
            continue;
        uint8_t next = (uint8_t)((i + 1) % RELAY_STORE_SLOTS);
        uint8_t want = (uint8_t)((slotSeq(image, i) + 1) % kSeqModulo);
        if (slotValid(image, next) && slotSeq(image, next) == want)
            continue;   // a newer write follows this one
        *mask = image[i * RELAY_STORE_SLOT_BYTES];
        cursor->slot = i;
        cursor->seq = slotSeq(image, i);
        cursor->valid = 1;
        return true;
    }
    return false;
}

void relayStoreNext(RelayStoreCursor* cursor, uint8_t mask, uint8_t* slot, uint8_t bytes[RELAY_STORE_SLOT_BYTES])
{
    cursor->slot = (uint8_t)((cursor->slot + 1) % RELAY_STORE_SLOTS);
    cursor->seq = (uint8_t)((cursor->seq + 1) % kSeqModulo);
    cursor->valid = 1;
    *slot = cursor->slot;
    bytes[0] = mask;
    bytes[1] = (uint8_t)~mask;
    bytes[2] = cursor->seq;
}
