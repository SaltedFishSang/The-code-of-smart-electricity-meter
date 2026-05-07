#ifndef _RTC_H
#define _RTC_H
#include "BSP.h"


#define    USER_WRITE_BKP_DAT1_DATA   0xA5A5

typedef enum {
    RTC_CLK_SRC_TYPE_HSE128=0x01,
    RTC_CLK_SRC_TYPE_LSE=0x02,
    RTC_CLK_SRC_TYPE_LSI=0x03,
}RTC_CLK_SRC_TYPE;
extern void RealTime_Init(void);
extern void RTC_TimeShow(void);
extern void RTC_DateShow(void);
extern void Set_Rtc_Time(void);
#endif
