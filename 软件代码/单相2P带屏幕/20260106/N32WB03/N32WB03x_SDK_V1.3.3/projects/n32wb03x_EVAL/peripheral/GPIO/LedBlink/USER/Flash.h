#ifndef _Flash_H
#define _Flash_H
#include "BSP.h"
extern void Flash_paraInit(void);
extern void Flash_WirteModbusDI(void);
extern void sysparainit(void);
extern void Flash_WirteModbusRemain(void);
extern void Flash_write_resetEnergy(void);
#endif
