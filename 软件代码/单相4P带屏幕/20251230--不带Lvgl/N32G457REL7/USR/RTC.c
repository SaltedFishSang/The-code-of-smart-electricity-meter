
#include "BSP.h"

/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file User_RTC_Config.c
 * @author Nations 
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */



RTC_DateType RTC_DateStructure;
RTC_TimeType RTC_TimeStructure;
RTC_InitType RTC_InitStructure;
uint32_t SynchPrediv, AsynchPrediv;



/**
 * @brief  Display the current Date on the Hyperterminal.
 */
void RTC_DateShow(void) {
    /* Get the current Date */
    RTC_GetDate(RTC_FORMAT_BIN, &RTC_DateStructure);
    log_info("\n\r //=========== Current Date Display ==============// \n\r");
    log_info("\n\r The current date (WeekDay-Date-Month-Year) is :  %0.2d-%0.2d-%0.2d-%0.2d \n\r",
             RTC_DateStructure.WeekDay,
             RTC_DateStructure.Date,
             RTC_DateStructure.Month,
             RTC_DateStructure.Year);

}

/**
 * @brief  Display the current time on the Hyperterminal.
 */
void RTC_TimeShow(void) {
    /* Get the current Time and Date */
    RTC_GetTime(RTC_FORMAT_BIN, &RTC_TimeStructure);
    log_info("\n\r //============ Current Time Display ===============// \n\r");
    log_info("\n\r The current time (Hour-Minute-Second) is :  %0.2d:%0.2d:%0.2d \n\r",
             RTC_TimeStructure.Hours,
             RTC_TimeStructure.Minutes,
             RTC_TimeStructure.Seconds);
    /* Unfreeze the RTC DAT Register */
    (void) RTC->DATE;
}

/**
 * @brief  RTC prescaler config.
 */
void RTC_PrescalerConfig(void) {
    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_24HOUR_FORMAT;
    /* Check on RTC init */
    if (RTC_Init(&RTC_InitStructure) == ERROR) {
        log_info("\r\n //******* RTC Prescaler Config failed **********// \r\n");
    }
}

/**
 * @brief  Configures the RTC Source Clock Type.
 * @param Clk_Src_Type specifies RTC Source Clock Type.
 *   This parameter can be on of the following values:
 *     @arg RTC_CLK_SRC_TYPE_HSE128
 *     @arg RTC_CLK_SRC_TYPE_LSE
 *     @arg RTC_CLK_SRC_TYPE_LSI
 * @param Is_First_Cfg_RCC specifies Is First Config RCC Module.
 *   This parameter can be on of the following values:
 *     @arg true
 *     @arg false
 * @param Is_Rst_Bkp specifies Whether Reset The Backup Area
 *   This parameter can be on of the following values:
 *     @arg true
 *     @arg false
 */
void RTC_CLKSourceConfig(RTC_CLK_SRC_TYPE Clk_Src_Type, bool Is_First_Cfg_RCC, bool Is_Rst_Bkp) {
//    assert_param(IS_CLKSRC_VALUE(ClkSrc));
    //  assert_param(IS_FLCFG_VALUE(FirstLastCfg));
    /* Enable the PWR clock */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR | RCC_APB1_PERIPH_BKP, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);
    /* Allow access to RTC */
    PWR_BackupAccessEnable(ENABLE);
    /* Reset Backup */
    if (true == Is_Rst_Bkp) {
        BKP_DeInit();
    }
    /* Disable RTC clock */
    RCC_EnableRtcClk(DISABLE);
    if (RTC_CLK_SRC_TYPE_HSE128 == Clk_Src_Type) {
        log_info("\r\n RTC_ClkSrc Is Set HSE128! \r\n");
        if (true == Is_First_Cfg_RCC) {
            /* Enable HSE */
            RCC_EnableLsi(DISABLE);
            RCC_ConfigHse(RCC_HSE_ENABLE);
            while (RCC_WaitHseStable() == ERROR) {
            }
            RCC_ConfigRtcClk(RCC_RTCCLK_SRC_HSE_DIV128);
        } else {
            RCC_EnableLsi(DISABLE);
            RCC_ConfigRtcClk(RCC_RTCCLK_SRC_HSE_DIV128);
            /* Enable HSE */
            RCC_ConfigHse(RCC_HSE_ENABLE);
            while (RCC_WaitHseStable() == ERROR) {
            }
        }
        SynchPrediv = 0x1E8; // 8M/128 = 62.5KHz
        AsynchPrediv = 0x7F;  // value range: 0-7F
    } else if (RTC_CLK_SRC_TYPE_LSE == Clk_Src_Type) {
        log_info("\r\n RTC_ClkSrc Is Set LSE! \r\n");
        if (true == Is_First_Cfg_RCC) {
            /* Enable the LSE OSC32_IN PC14 */
            RCC_EnableLsi(DISABLE); // LSI is turned off here to ensure that only one clock is turned on
#if (_TEST_LSE_BYPASS_)
            RCC_ConfigLse(RCC_LSE_BYPASS);
#else
            RCC_ConfigLse(RCC_LSE_ENABLE);
#endif
            while (RCC_GetFlagStatus(RCC_FLAG_LSERD) == RESET) {
            }
            RCC_ConfigRtcClk(RCC_RTCCLK_SRC_LSE);
        } else {
            /* Enable the LSE OSC32_IN PC14 */
            RCC_EnableLsi(DISABLE);
            RCC_ConfigRtcClk(RCC_RTCCLK_SRC_LSE);
#if (_TEST_LSE_BYPASS_)
            RCC_ConfigLse(RCC_LSE_BYPASS);
#else
            RCC_ConfigLse(RCC_LSE_ENABLE);
#endif
            while (RCC_GetFlagStatus(RCC_FLAG_LSERD) == RESET) {
            }
        }
        SynchPrediv = 0xFF; // 32.768KHz
        AsynchPrediv = 0x7F; // value range: 0-7F
    } else if (RTC_CLK_SRC_TYPE_LSI == Clk_Src_Type) {
        log_info("\r\n RTC_ClkSrc Is Set LSI! \r\n");
        if (true == Is_First_Cfg_RCC) {
            /* Enable the LSI OSC */
            RCC_EnableLsi(ENABLE);
            while (RCC_GetFlagStatus(RCC_FLAG_LSIRD) == RESET) {
            }
            RCC_ConfigRtcClk(RCC_RTCCLK_SRC_LSI);
        } else {
            RCC_ConfigRtcClk(RCC_RTCCLK_SRC_LSI);
            /* Enable the LSI OSC */
            RCC_EnableLsi(ENABLE);
            while (RCC_GetFlagStatus(RCC_FLAG_LSIRD) == RESET) {
            }
        }
        SynchPrediv = 0x136; // 39.64928KHz
        AsynchPrediv = 0x7F;  // value range: 0-7F
    } else {
        log_info("\r\n RTC_ClkSrc Value is error! \r\n");
    }
    /* Enable the RTC Clock */
    RCC_EnableRtcClk(ENABLE);
    RTC_WaitForSynchro();
}
void RTC_DataAndTimeSetValue(u8 year, u8 month, u8 date, u8 hour, u8 minute, u8 second)
{
    u8 month1;
    u16 year1;
    u8 weekday;
    RTC_DateStructure.Year = year;
    RTC_DateStructure.Month = month;
    RTC_DateStructure.Date = date;
    RTC_TimeStructure.H12 = RTC_PM_H12;
    RTC_TimeStructure.Hours = hour;
    RTC_TimeStructure.Minutes = minute;
    RTC_TimeStructure.Seconds = second;
    month1 = month;
    year1 = year + 2000;
    if ((month == 1) || (month == 2))
    {
        month1 = month + 12;
        year1 = (u16)year + 2000 - 1;
    }
    weekday = ((date + 2 * month1 + 3 * (month1 + 1) / 5 + year1 + year1 / 4 - year1 / 100 + year1 / 400) % 7) + 1;
    RTC_DateStructure.WeekDay = weekday;
    /* Configure the RTC date register */
    if (RTC_SetDate(RTC_FORMAT_BIN, &RTC_DateStructure) == ERROR)
    {
        log_info("\n\r>> !! RTC Set Date failed. !! <<\n\r");
    }
    else
    {
        log_info("\n\r>> !! RTC Set Date success. !! <<\n\r");
        RTC_DateShow();
    }

    /* Configure the RTC time register */
    if (RTC_ConfigTime(RTC_FORMAT_BIN, &RTC_TimeStructure) == ERROR)
    {
        log_info("\n\r>> !! RTC Set Time failed. !! <<\n\r");
    }
    else
    {
        log_info("\n\r>> !! RTC Set Time success. !! <<\n\r");
        RTC_TimeShow();
    }
}

//void Set_Rtc_Time(void) {
//    log_info("\r\n\n RTC set....\r\n");


//    RTC_DateDefault.WeekDay = 4;
//    RTC_DateDefault.Date = g_Alatable[0x0017];
//    RTC_DateDefault.Month = g_Alatable[0x0016];
//    RTC_DateDefault.Year = g_Alatable[0x0015] - 2000;
//    // Time
//    if (g_Alatable[0x0018] < 12) {
//        RTC_TimeDefault.H12 = RTC_AM_H12;
//    } else {
//        RTC_TimeDefault.H12 = RTC_PM_H12;
//    }
//    RTC_TimeDefault.Hours = g_Alatable[0x0018];
//    RTC_TimeDefault.Minutes = g_Alatable[0x0019];
//    RTC_TimeDefault.Seconds = g_Alatable[0x001a];
//    /* RTC clock source select */
//    RTC_CLKSourceConfig(RTC_CLK_SRC_TYPE_LSE, true, true);
//    RTC_PrescalerConfig();
//    log_info("\r\n RTC configured....");
//    /* Adjust time by values entered by the user on the hyperterminal */
////    RTC_DateRegulate();
////    RTC_TimeRegulate();
//    BKP_WriteBkpData(BKP_DAT1, USER_WRITE_BKP_DAT1_DATA);
//}

void RealTime_Init(void) {
    log_info("RTC Init");
    /* RTC date time alarm default value*/
   // RTC_DateAndTimeDefaultVale();
    /* Enable the PWR clock */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR | RCC_APB1_PERIPH_BKP, ENABLE);
    /* Allow access to RTC */
    PWR_BackupAccessEnable(ENABLE);
    /// Set_Rtc_Time();
    if (USER_WRITE_BKP_DAT1_DATA != BKP_ReadBkpData(BKP_DAT1) )
    {
        /* Backup data register value is not correct or not yet programmed (when
           the first time the program is executed) */
        log_info("\r\n\n RTC not yet configured....");
        /* RTC clock source select */
        RTC_CLKSourceConfig(RTC_CLK_SRC_TYPE_LSE, true, true);
        RTC_PrescalerConfig();
        log_info("\r\n RTC configured....");
        /* Adjust time by values entered by the user on the hyperterminal */
        BKP_WriteBkpData(BKP_DAT1, USER_WRITE_BKP_DAT1_DATA);
    }
    /* Calibrate output 1Hz signal */
    RTC_ConfigCalibOutput(RTC_CALIB_OUTPUT_1HZ);
    /* Calibrate output config,push pull */
    RTC_ConfigOutputType(RTC_OUTPUT_PUSHPULL);
    /* Calibrate output enable*/
    RTC_EnableCalibOutput(ENABLE);
    log_info("\r\n RTC Config end....");
}


