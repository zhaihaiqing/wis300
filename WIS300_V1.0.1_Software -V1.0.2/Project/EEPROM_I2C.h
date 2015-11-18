#include <stm32l1xx_i2c.h>

extern void I2C1_Configuration(void);
extern void I2C1_ReadS_24C(unsigned short addr ,unsigned char * pBuffer,unsigned short Length);
//extern void I2C_Standby_24C(void);
extern unsigned char I2C_PageWrite_24C(unsigned short addr,unsigned char* pBuffer, unsigned char Length);
extern unsigned char I2C_WriteS_24C(unsigned short addr,unsigned char* pBuffer,  unsigned short Length);
extern void EEPROM_Check(void);
extern void EEPROM_EraseFullChip(void);
