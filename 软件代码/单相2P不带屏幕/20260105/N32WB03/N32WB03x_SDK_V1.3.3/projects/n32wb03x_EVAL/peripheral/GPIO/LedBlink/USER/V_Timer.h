#ifndef __V_Timer_H
#define __V_Timer_H
#include "BSP.h"

typedef enum
{
    VTIM0,
    VTIM1,
    VTIM2,
    VTIM3,
    VTIM4,
    VTIM5,
    VTIM6,
    VTIM7,
    VTIM8,
    VTIM9,
    VTIM10,
    VTIM11,
    VTIM12,
    VTIM13,
    VTIM14,
    VTIM15,
    VTIM16,
    VTIM17,
    VTIMER_NUM
} VtimerName_t; 

typedef UINT32 timer_res_t;
typedef struct
{
    timer_res_t msec;
    void *pCallback;
} Vtimer_t, *PVtimer;

void Vtimer_Init(void);
void Vtimer_SetTimer(VtimerName_t name, timer_res_t msec, void *pCallback);
void Vtimer_KillTimer(VtimerName_t name);
unsigned char Vtimer_TimerElapsed(VtimerName_t name);
void Vtimer_UpdateHandler(void);
#endif
