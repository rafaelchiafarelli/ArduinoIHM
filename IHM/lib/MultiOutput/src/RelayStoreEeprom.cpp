#ifdef __AVR__
#include "RelayStoreEeprom.h"
#include "RelayStore.h"
#include <avr/eeprom.h>

static RelayStoreCursor g_cursor;

uint8_t relayStoreLoad()
{
    uint8_t image[RELAY_STORE_BYTES];
    eeprom_read_block(image, (const void*)RELAY_STORE_EEPROM_ADDR, sizeof(image));
    uint8_t mask;
    relayStoreFind(image, &mask, &g_cursor);
    return mask;
}

void relayStoreSave(uint8_t mask)
{
    uint8_t slot, bytes[RELAY_STORE_SLOT_BYTES];
    relayStoreNext(&g_cursor, mask, &slot, bytes);
    // value, ~value, then seq -- one byte at a time so the order is ours,
    // not the library's (see RelayStore.h on torn writes).
    uint8_t* at = (uint8_t*)(RELAY_STORE_EEPROM_ADDR + slot * RELAY_STORE_SLOT_BYTES);
    for (uint8_t i = 0; i < RELAY_STORE_SLOT_BYTES; i++)
        eeprom_update_byte(at + i, bytes[i]);
}
#endif
