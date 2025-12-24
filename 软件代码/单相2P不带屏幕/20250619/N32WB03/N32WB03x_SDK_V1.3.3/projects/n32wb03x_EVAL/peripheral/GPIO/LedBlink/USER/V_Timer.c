#include "BSP.h"

/*
 *     序号         功能定义                  定时时间
 *     VTIM0       LED闪烁
 *     VTIM1       看门狗喂狗
 *     VTIM2       串口2数据接收超时处理
 *     VTIM3       定时获取BL0942全电参数
 *     VTIM4       串口1ss数据接收超时处理
 *     VTIM5       定时查看RTC数据
 */

static Vtimer_t sVtimer[VTIMER_NUM];

/*******************************************************************************
 * Function Name  : Vtimer_Init
 * Description    : 软定时器初始化函数
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Vtimer_Init(void)
{
    u8 i;
    for (i = 0; i < VTIMER_NUM; i++)
    {
        sVtimer[i].msec = 0;
        sVtimer[i].pCallback = 0;
    }
}

/*******************************************************************************
* Function Name  : Vtimer_SetTimer
* Description    : 软定时时间设置函数
* Input          : name 定时器序号
                   msec 定时器时间 单位ms
                   pCallback  软定时回调函数
* Output         : None
* Return         : None
*******************************************************************************/
void Vtimer_SetTimer(VtimerName_t name, timer_res_t msec, void *pCallback)
{
    sVtimer[name].msec = msec;
    sVtimer[name].pCallback = pCallback;
}

/*******************************************************************************
 * Function Name  : Vtimer_KillTimer
 * Description    : 定时取消函数
 * Input          : name 取消定时器序号
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Vtimer_KillTimer(VtimerName_t name)
{
    sVtimer[name].msec = 0;
    sVtimer[name].pCallback = 0;
}

u8 Vtimer_TimerElapsed(VtimerName_t name)
{
    if (sVtimer[name].msec == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

typedef void (*PFN_Callback_t)(void);

void Vtimer_UpdateHandler(void)
{
    // Enter each DELTAT_MS ms
    u8 i;

    for (i = 0; i < VTIMER_NUM; i++)
    {
        if (sVtimer[i].msec != 0)
        {
            sVtimer[i].msec--;
            if (sVtimer[i].pCallback != 0)
            {
                if (sVtimer[i].msec == 0)
                {
                    ((PFN_Callback_t)sVtimer[i].pCallback)();
                }
            }
        }
    }
}
