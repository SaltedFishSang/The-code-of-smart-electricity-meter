#ifndef _RTC_H
#define _RTC_H
#include "BSP.h"

typedef struct
{
    uint16_t years;
    uint8_t months;
    uint8_t days;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t weekday;
} RTC_Calendar;
extern RTC_Calendar RtcTime,RtcTime1;

extern void Rtc_Init(void);
extern void Get_RTC_Time(void);
extern void Set_RTC_Time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

#endif
