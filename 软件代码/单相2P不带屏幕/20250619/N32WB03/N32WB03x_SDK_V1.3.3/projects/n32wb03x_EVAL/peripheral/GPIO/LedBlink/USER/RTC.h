#ifndef _RTC_H
#define _RTC_H
#include "BSP.h"
extern void RTC_TimeShow(void);
extern void Rtc_Init(void);
extern void Rtc_Task(void);
extern void Rtc_Timeset(u8 hour, u8 min, u8 sec);
#endif
