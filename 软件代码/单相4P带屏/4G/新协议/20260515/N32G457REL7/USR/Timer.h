#ifndef _Timer_H
#define _Timer_H
#include "BSP.h"
struct _TimerPoint
{
	char weekday[8];
	u8 hour;
	u8 min;
	u8 onoff;
};
extern struct _TimerPoint TimerPoint[20];
extern u8 TimerpointNumber;
extern void GetTimerPoint(char *p);
#endif
