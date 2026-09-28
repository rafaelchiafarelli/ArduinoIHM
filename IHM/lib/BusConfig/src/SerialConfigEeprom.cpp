#ifdef __AVR__
#include "SerialConfigEeprom.h"
#include "SerialConfigImage.h"
#include <avr/eeprom.h>

bool loadSerialConfig(SerialConfig* out)
{
    // The image written by this firmware is SERIAL_CONFIG_IMAGE_LEN bytes;
    // a longer (newer) one only ever adds fields this firmware can't read.
    uint8_t buf[SERIAL_CONFIG_IMAGE_LEN];
    eeprom_read_block(buf, (const void*)SERIAL_CONFIG_EEPROM_ADDR, sizeof(buf));
    // A newer image's CRC sits past our buffer, so read its real length
    // first and only trust a same-size image here.
    uint16_t payload = (uint16_t)(buf[3] | ((uint16_t)buf[4] << 8));
    if (payload > SERIAL_CONFIG_PAYLOAD_LEN) {
        uint16_t total = (uint16_t)(SERIAL_CONFIG_IMAGE_HEADER + payload + SERIAL_CONFIG_IMAGE_CRC);
        if (total > SERIAL_CONFIG_EEPROM_RESERVED) {
            serialConfigDefaults(*out);
            return false;
        }
        uint8_t big[SERIAL_CONFIG_EEPROM_RESERVED];
        eeprom_read_block(big, (const void*)SERIAL_CONFIG_EEPROM_ADDR, total);
        return serialConfigDeserialize(big, total, out);
    }
    return serialConfigDeserialize(buf, sizeof(buf), out);
}

void saveSerialConfig(const SerialConfig& c)
{
    uint8_t buf[SERIAL_CONFIG_IMAGE_LEN];
    uint16_t len = serialConfigSerialize(c, buf);
    eeprom_update_block(buf, (void*)SERIAL_CONFIG_EEPROM_ADDR, len);
}
#endif
