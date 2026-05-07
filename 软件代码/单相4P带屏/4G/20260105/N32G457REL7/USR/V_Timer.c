/******************** (C) COPYRIGHT 2018 JIUVGH ***************************
 * 作者:sangjian
 * 文件名:V_Timer.c
 * 描述:VTimer定时器
 * 时间:2018/2/26
 *****************************************************************************/
#include "bsp.h"

/*
定时任务序号      任务内容                定时时间  备注
VTIM1             LED闪烁                 500ms
VTIM2             看门狗喂狗              30ms
VTIM3             串口2接送超时等待       50ms
VTIM4             定时1s获取BL0942数据    1s
VTIM5             串口3接收超时等待       100ms
VTIM6             TFT更新1次              1s
VTIM10            MQTT  Info              303s
VTIM11            MQTT  Heart             120s
VTIM12            过压检测
VTIM13            欠压检测
VTIM14            过流检测
VTIM15            NBIOT
VTIM16            过载检测
*/
static Vtimer_t sVtimer[VTIMER_NUM]; // 结构体类型

/*******************************************************************************
 * Function Name  : Vtimer_Init
 * Description    : 定时多任务 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Vtimer_Init()
{
	u8 i;
	for (i = 0; i < VTIMER_NUM; i++)
	{
		sVtimer[i].msec = 0;
		sVtimer[i].pCallback = 0;
	}
	TIM_Configuration();
}

/*******************************************************************************
* Function Name  : Vtimer_SetTimer
* Description    : 创建定时任务
* Input          : name：定时任务名字
				   msec 任务定时时间
				   pCallback  调用执行函数
* Output         : None
* Return         : None
*******************************************************************************/
void Vtimer_SetTimer(VtimerName_t name, timer_res_t msec, void *pCallback) // name 是enum VtimerName_t类型 msec是 u16类型
{
	sVtimer[name].msec = msec;
	sVtimer[name].pCallback = pCallback;
}
/*******************************************************************************
 * Function Name  : Vtimer_KillTimer
 * Description    : 关闭定时任务
 * Input          : None
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
		return TRUE_V;
	else
		return FALSE_V;
}

typedef void (*PFN_Callback_t)(void);

// 每隔 deltat ms 处理一次callback
void Vtimer_UpdateHandler(void)
{
	// Enter each DELTAT_MS ms
	u8 i;
	for (i = 0; i < VTIMER_NUM; i++) // 11
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
