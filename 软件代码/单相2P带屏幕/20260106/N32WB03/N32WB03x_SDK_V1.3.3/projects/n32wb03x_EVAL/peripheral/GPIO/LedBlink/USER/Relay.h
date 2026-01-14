#ifndef _Relay_H
#define _Relay_H
#include "BSP.h"
extern void RelayInit(void);
extern void RelayControl(u8 onoff);
extern bool GateStatus(void);
#endif
