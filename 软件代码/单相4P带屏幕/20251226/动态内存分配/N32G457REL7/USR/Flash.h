#ifndef _Flash_H
#define _Flash_H
#include "BSP.h"
extern void FlashWriteData(void);
extern void FlashReadData(void);
extern void FlasgWriteResetData(void);
extern void FlashWrite_mac_id(void);
extern void Flash_Read_mac_id(void);
extern void FlashWritereset_mac_id(void);
extern void FlashInit(void);
#endif
