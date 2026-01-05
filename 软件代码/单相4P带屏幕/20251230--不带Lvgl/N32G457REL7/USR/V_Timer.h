#ifndef __V_Timer_H
#define __V_Timer_H
#include "bsp.h"
typedef unsigned short u16;
typedef unsigned char  u8;

typedef enum
{
  FALSE_V = 0,
  TRUE_V = !FALSE_V
}bbool;

typedef enum {VTIM0,VTIM1,VTIM2,VTIM3,VTIM4,VTIM5,VTIM6,VTIM7,VTIM8,VTIM12,VTIMER_NUM} VtimerName_t;//VtimerName_t 是enum VtimerName的别名

typedef unsigned int timer_res_t;

typedef struct
{
	timer_res_t msec;
	void* pCallback;
} Vtimer_t,*PVtimer;

void 	Vtimer_Init(void);
void 	Vtimer_SetTimer(VtimerName_t name,timer_res_t  msec,void* pCallback);
void 	Vtimer_KillTimer(VtimerName_t name);
u8 		Vtimer_TimerElapsed(VtimerName_t name);
void 	Vtimer_UpdateHandler(void);

#endif
