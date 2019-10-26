#ifndef __LCD_I2C_H
#define __LCD_I2C_H

#include <stdint.h>

void I2C_config_B2(uint8_t isMaster, uint8_t ownAddr, uint32_t baudRate_kHz);
uint8_t I2C_Write(uint8_t , uint8_t *, uint16_t );
uint8_t I2C_Read(uint8_t , uint8_t *, uint16_t );
void i2cSendByte(uint8_t , uint8_t );
void LcdWriteByte(uint8_t , uint8_t , uint8_t );
void LcdWriteNibble(uint8_t , uint8_t , uint8_t );
void Config_LCD();
void LcdDisplay(char *palavra);
void LcdPosicao(uint8_t cursor, uint8_t linha);

#endif
