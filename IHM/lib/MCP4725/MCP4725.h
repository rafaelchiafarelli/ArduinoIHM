/**************************************************************************/
/*!
    @file     MCP4725.h
*/
/**************************************************************************/

#ifndef __MCP4725_H_
#define __MCP4725_H_

#include <BusIO_Register.h>
#include <I2CDevice.h>
#include <Wire.h>

#define MCP4725_I2CADDR_DEFAULT (0x62) ///< Default i2c address
#define MCP4725_CMD_WRITEDAC (0x40)    ///< Writes data to the DAC
#define MCP4725_CMD_WRITEDACEEPROM                                             \
  (0x60) ///< Writes data to the DAC and the EEPROM (persisting the assigned
         ///< value after reset)

/**************************************************************************/
/*!
    @brief  Class for communicating with an MCP4725 DAC
*/
/**************************************************************************/
class MCP4725 {
public:
  MCP4725();
  bool begin(uint8_t i2c_address = MCP4725_I2CADDR_DEFAULT,
             TwoWire *wire = &Wire);
  bool setVoltage(uint16_t output, bool writeEEPROM,
                  uint32_t dac_frequency = 400000);

private:
  I2CDevice *i2c_dev = NULL;
};

#endif
