#ifndef _Modbus_H
#define _Modbus_H
#include "BSP.h"
extern void ModbusDataHandle(void);
extern u16 m_DiState;
extern u16 m_remain_table[51]; // 保存寄存器
extern u16 m_input_table[39];  // 输入寄存器
extern u8 g_modbus_addr;       // modbus 从设备地址
extern u16 g_budrate;          // 485通讯波特率
extern u16 Modbus_Readinput(u8 address);
extern u8 Modbus_ReadDI(u8 num);
extern void Modbus_WirteDI(u8 num, u8 value);
extern void Eprom_ReadModbusDI(void);
extern u16 Modbus_Readremain(u8 address);
extern void Modbus_Wrietemain(u8 address, u16 value);
extern void Modbus_WrieteInput(u8 address, u16 value);

#endif
