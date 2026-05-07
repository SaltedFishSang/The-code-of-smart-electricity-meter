#ifndef _EPROM_H
#define _EPROM_H
#include "BSP.h"
typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned short u16;
#define AT24C01 127
#define AT24C02 255
#define AT24C04 511
#define AT24C08 1023
#define AT24C16 2047
#define AT24C32 4095
#define AT24C64 8191
#define AT24C128 16383
#define AT24C256 32767

#define EE_TYPE AT24C16      // EEPROM
#define AT24CXX_Page_Size 16 //
#define DEBUG 0              //
#define QuickWR 0            //
u8 AT24CXX_ReadOneByte(u16 ReadAddr);
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite);
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite);
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead);
void AT24CXX_Check(void);
void AT24CXX_Init(void);
#define SCL_HIGH GPIO_SetBits(GPIOB, GPIO_PIN_13);
#define SCL_LOW GPIO_ResetBits(GPIOB, GPIO_PIN_13);
#define SDA_HIGH GPIO_SetBits(GPIOB, GPIO_PIN_12);
#define SDA_LOW GPIO_ResetBits(GPIOB, GPIO_PIN_12);
#define READ_SDA GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_12)
extern void Eprom_WirteModbusDI(u8 address, u8 value);
extern void Eprom_ReadModbusDI(void);
extern void Eprom_WirteResetModbusDI(void);
extern void Eprom_WirteModbusRemain(u8 address, u8 num);
extern void Eprom_ReadModbusRemain(void);
extern void Eprom_WirteResetModbusRemain(void);
extern void Eprom_write_Energy(void);
extern void Eprom_read_Energy(void);
extern void Eprom_write_resetEnergy(void) ;
extern void Eprom_paraInit(void);
extern void sysparainit(void);


#endif
