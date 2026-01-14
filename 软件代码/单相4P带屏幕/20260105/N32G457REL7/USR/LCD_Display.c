#include "BSP.h"
/*!
    \brief     Display_page1init显示界面1
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Display_pageinit(void)
{
    u8 current[4];
    u8 energy[10];
    TFTLCD_Fill(0, 0, TFTLCD_W, TFTLCD_H, LGRAY);
    TFTLCD_DrawLine(0, 48, TFTLCD_W, 48, RED);
    /*显示日期*/
    TFTLCD_ShowIntNum(0, 8, RtcTime.years, 4, BLACK, LGRAY, 24);
    TFTLCD_ShowString(48, 8, "/", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowIntNum(60, 8, RtcTime.months, 2, BLACK, LGRAY, 24);
    TFTLCD_ShowString(84, 8, "/", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowIntNum(96, 8, RtcTime.days, 2, BLACK, LGRAY, 24);
    /*显示时间*/
    TFTLCD_ShowIntNum(132, 8, RtcTime.hours, 2, BLACK, LGRAY, 24);
    TFTLCD_ShowString(156, 8, ":", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowIntNum(168, 8, RtcTime.minutes, 2, BLACK, LGRAY, 24);

    /*显示是否定时*/
    DisplayClockState(Modbus_ReadDI(4));
    /*显示图标*/
    TFTLCD_ShowString(290, 8, "4G", BLACK, LGRAY, 24, 0);

    /*显示电压*/
    TFTLCD_ShowCN(0, 64, 0, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowCN(32, 64, 1, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowString(64, 64, ":", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowIntNum(88, 64, BL0942_EleData.vol, 4, BLACK, LGRAY, 24);
    TFTLCD_ShowString(160, 64, "V", BLACK, LGRAY, 24, 0);

    /*显示在线*/
    TFTLCD_ShowCN(224, 64, 18, RED, LGRAY, 32, 0);
    TFTLCD_ShowCN(256, 64, 19, RED, LGRAY, 32, 0);

    /*显示电流*/
    TFTLCD_ShowCN(0, 96, 0, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowCN(32, 96, 2, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowString(64, 96, ":", BLACK, LGRAY, 24, 0);
    //    /*显示电流*/
    if (BL0942_EleData.current >= 100)
    {
        sprintf((char *)current, "%.1f", (float)BL0942_EleData.current * 0.001f);
        TFTLCD_ShowString(100, 96, current, BLACK, LGRAY, 24, 0);
    }
    else
    {
        TFTLCD_ShowString(100, 96, "0", BLACK, LGRAY, 24, 0);
    }
    TFTLCD_ShowChar(160, 96, 'A', BLACK, LGRAY, 24, 0);
    /*显示功率*/
    TFTLCD_ShowCN(0, 128, 3, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowCN(32, 128, 4, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowString(64, 128, ":", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowIntNum(88, 128, BL0942_EleData.power, 6, BLACK, LGRAY, 24);
    TFTLCD_ShowString(208, 128, "W", BLACK, LGRAY, 24, 0);
    /*显示电能*/

    TFTLCD_ShowCN(0, 160, 7, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowCN(32, 160, 8, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowString(64, 160, ":", BLACK, LGRAY, 24, 0);
    if (BL0942_EleData.energy > 0)
    {
        if ((BL0942_EleData.energy % 100) == 0)
        {
            sprintf((char *)energy, "%.1f", (double)BL0942_EleData.energy * 0.001);
            TFTLCD_ShowString(100, 160, energy, BLACK, LGRAY, 24, 0);
        }
        else
        {
            if ((BL0942_EleData.energy % 10) == 0)
            {
                sprintf((char *)energy, "%.2f", (double)BL0942_EleData.energy * 0.001);
                TFTLCD_ShowString(100, 160, energy, BLACK, LGRAY, 24, 0);
            }
            else
            {
                sprintf((char *)energy, "%.3f", (double)BL0942_EleData.energy * 0.001);
                TFTLCD_ShowString(100, 160, energy, BLACK, LGRAY, 24, 0);
            }
        }
    }
    else
    {
        TFTLCD_ShowString(100, 160, "0", BLACK, LGRAY, 24, 0);
    }
    TFTLCD_ShowString(256, 160, "KW/H", BLACK, LGRAY, 24, 0);
    /*显示输出*/
    TFTLCD_ShowCN(0, 192, 9, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowCN(32, 192, 10, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowString(64, 192, ":", BLACK, LGRAY, 24, 0);
    DisplayOutputState(Modbus_ReadDI(1));
    /*显示锁扣*/
    DisplayLockState(Modbus_ReadDI(2));
}
/*!
    \brief     Display_page1 显示界面1
    \param[in]  none
    \param[out] none
    \retval     none
*/

void Display_page1(void)
{
    u8 current[4];
    u8 energy[10];
    static u8 state = 0;
    state++;
    switch (state)
    {
    case 1:
        /*显示日期*/
        TFTLCD_ShowIntNum(0, 8, RtcTime.years, 4, BLACK, LGRAY, 24);
        TFTLCD_ShowIntNum(60, 8, RtcTime.months, 2, BLACK, LGRAY, 24);
        TFTLCD_ShowIntNum(96, 8, RtcTime.days, 2, BLACK, LGRAY, 24);
        break;
    case 2:
        /*显示时间*/
        TFTLCD_ShowIntNum(132, 8, RtcTime.hours, 2, BLACK, LGRAY, 24);
        TFTLCD_ShowIntNum(168, 8, RtcTime.minutes, 2, BLACK, LGRAY, 24);
        break;
    case 3:
        /*显示电压*/
        TFTLCD_ShowIntNum(88, 64, BL0942_EleData.vol, 4, BLACK, LGRAY, 24);
        break;
    case 4:
        /*显示电流*/
        if (BL0942_EleData.current >= 100)
        {
            sprintf((char *)current, "%.1f", (float)BL0942_EleData.current * 0.001f);
            TFTLCD_ShowString(100, 96, current, BLACK, LGRAY, 24, 0);
        }
        else
        {
            TFTLCD_ShowString(100, 96, "0", BLACK, LGRAY, 24, 0);
        }
        break;
    case 5:
        /*显示功率*/
        TFTLCD_ShowIntNum(88, 128, BL0942_EleData.power, 6, BLACK, LGRAY, 24);
        break;
    case 6:
        /*显示电能*/
        if (BL0942_EleData.energy > 0)
        {
            if ((BL0942_EleData.energy % 100) == 0)
            {
                sprintf((char *)energy, "%.1f", (double)BL0942_EleData.energy * 0.001);
                TFTLCD_ShowString(100, 160, energy, BLACK, LGRAY, 24, 0);
            }
            else
            {
                if ((BL0942_EleData.energy % 10) == 0)
                {
                    sprintf((char *)energy, "%.2f", (double)BL0942_EleData.energy * 0.001);
                    TFTLCD_ShowString(100, 160, energy, BLACK, LGRAY, 24, 0);
                }
                else
                {
                    sprintf((char *)energy, "%.3f", (double)BL0942_EleData.energy * 0.001);
                    TFTLCD_ShowString(100, 160, energy, BLACK, LGRAY, 24, 0);
                }
            }
        }
        else
        {
            TFTLCD_ShowString(100, 160, "0", BLACK, LGRAY, 24, 0);
        }
        break;
    case 7:
        state = 0;
        break;
    default:
        state = 0;
        break;
    }
}

static char tftflag = 0;
/*!
    \brief     TFTTimer
    \param[in]  none
    \param[out] none
    \retval     none
*/

void TFTTimer(void)
{
    tftflag = 1;
    Vtimer_SetTimer(VTIM6, 500, TFTTimer);
}

void TFTTask(void)
{
    if (tftflag == 1)
    {
        tftflag = 0;
        Display_page1();
    }
}

/*!
    \brief     DisplayOutputState  显示输出打开还是关闭
    \param[in]  u8 state  0 关闭 1 打开
    \param[out] none
    \retval     none
*/
void DisplayOutputState(u8 state)
{
    switch (state)
    {
    case 0:
        LedSwitchButton(0);
        TFTLCD_ShowCN(88, 192, 13, RED, LGRAY, 32, 0);
        TFTLCD_ShowCN(120, 192, 14, RED, LGRAY, 32, 0);
        break;
    case 1:
        LedSwitchButton(1);
        TFTLCD_ShowCN(88, 192, 11, GREEN, LGRAY, 32, 0);
        TFTLCD_ShowCN(120, 192, 12, GREEN, LGRAY, 32, 0);
        break;
    default:
        break;
    }
}

/*!
    \brief     DisplayLockState  显示锁扣状态
    \param[in]  u8 state  0 关闭 1 打开
    \param[out] none
    \retval     none
*/
void DisplayLockState(u8 state)
{
    switch (state)
    {
    case 0:
        LedLockButton(0);
        TFTLCD_ShowCN(224, 192, 21, GREEN, LGRAY, 32, 0);
        TFTLCD_ShowCN(256, 192, 15, GREEN, LGRAY, 32, 0);
        break;
    case 1:
        LedLockButton(1);
        TFTLCD_ShowCN(224, 192, 20, RED, LGRAY, 32, 0);
        TFTLCD_ShowCN(256, 192, 15, RED, LGRAY, 32, 0);
        break;
    default:
        break;
    }
}

/*!
    \brief     DisplayNetState  显示网络状态
    \param[in]  u8 state  0 关闭 1 打开
    \param[out] none
    \retval     none
*/
void DisplayNetState(u8 state)
{
    switch (state)
    {
    case 0:
        LedNetButton(0);
        TFTLCD_ShowCN(224, 64, 18, RED, LGRAY, 32, 0);
        TFTLCD_ShowCN(256, 64, 19, RED, LGRAY, 32, 0);
        break;
    case 1:
        LedNetButton(1);
        TFTLCD_ShowCN(224, 64, 17, GREEN, LGRAY, 32, 0);
        TFTLCD_ShowCN(256, 64, 19, GREEN, LGRAY, 32, 0);
        break;
    default:
        break;
    }
}

/*!
    \brief     DisplayClockState  显示时钟状态
    \param[in]  u8 state  0 关闭 1 打开
    \param[out] none
    \retval     none
*/
void DisplayClockState(u8 state)
{
    switch (state)
    {
    case 0:

        TFTLCD_ShowString(216, 12, "    ", BLACK, LGRAY, 24, 0);
        break;
    case 1:
        TFTLCD_ShowCN(216, 12, 0, MAGENTA, LGRAY, 16, 0);
        TFTLCD_ShowCN(232, 12, 1, MAGENTA, LGRAY, 16, 0);
        TFTLCD_ShowCN(248, 12, 2, MAGENTA, LGRAY, 16, 0);
        break;
    default:
        break;
    }
}
