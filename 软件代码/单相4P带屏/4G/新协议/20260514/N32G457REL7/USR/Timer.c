#include "BSP.h"
#define Timer_DBG 0
struct _TimerPoint TimerPoint[20];
u8 TimerpointNumber; // 定时点数
/*!
    \brief      EPSavPoint 在EPROM里面保持下发的定时点
    \param[in]  number
    \param[out] none
    \retval     none
*/

void EPSavPoint(u8 number)
{
    EEP_Write8(SwitchTimePoint + 11 * (number - 1), TimerPoint[number - 1].weekday[0]);
    EEP_Write8(SwitchTimePoint + 11 * (number - 1) + 1, TimerPoint[number - 1].weekday[1]);
    EEP_Write8(SwitchTimePoint + 11 * (number - 1) + 2, TimerPoint[number - 1].weekday[2]);
    EEP_Write8(SwitchTimePoint + 11 * (number - 1) + 3, TimerPoint[number - 1].weekday[3]);
    EEP_Write8(SwitchTimePoint + 11 * (number - 1) + 4, TimerPoint[number - 1].weekday[4]);
    EEP_Write8(SwitchTimePoint + 11 * (number - 1) + 5, TimerPoint[number - 1].weekday[5]);
    EEP_Write8(SwitchTimePoint + 11 * (number - 1) + 6, TimerPoint[number - 1].weekday[6]);
    EEP_Write8(SwitchTimePoint + 11 * (number - 1) + 7, TimerPoint[number - 1].weekday[7]);
    EEP_Write8(SwitchTimePoint + 11 * (number - 1) + 8, TimerPoint[number - 1].hour);
    EEP_Write8(SwitchTimePoint + 11 * (number - 1) + 9, TimerPoint[number - 1].min);
    EEP_Write8(SwitchTimePoint + 11 * (number - 1) + 10, TimerPoint[number - 1].onoff);
}

void GetTimerPoint(char *p)
{
    u8 flag = 0;
    u8 pointnumber = 0;
    if (*(p + 1) == '"')
    {
        pointnumber = (*(p)-0x30);
        flag = 1;
    }
    else
    {
        pointnumber = (*(p)-0x30) * 10 + (*(p + 1) - 0x30);
        flag = 2;
    }

    p += flag + 6;
    //   printf("%s\r\n", p);
    sscanf(p, "\"%[^-]-%d:%d-%d}]",
           TimerPoint[pointnumber - 1].weekday,
           &TimerPoint[pointnumber - 1].hour,
           &TimerPoint[pointnumber - 1].min,
           &TimerPoint[pointnumber - 1].onoff);
#if Timer_DBG
    printf("pointnumber == %d\r\n", pointnumber);
    printf("%s\r\n", TimerPoint[pointnumber - 1].weekday);
    printf("%d\r\n", TimerPoint[pointnumber - 1].hour);
    printf("%d\r\n", TimerPoint[pointnumber - 1].min);
    printf("%d\r\n", TimerPoint[pointnumber - 1].onoff);
#endif
    EPSavPoint(pointnumber);
    HeartTimerPoint();
}
