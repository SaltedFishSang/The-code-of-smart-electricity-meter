#ifndef _Relay_H
#define _Relay_H
#include "BSP.h"

extern void RelaygpioInit(void);
extern void Relay_Control(u8 onoff);
#endif
