#include "BSP.h"
#define RTC_DBG 0
RTC_DateType RTC_DateStructure;
RTC_DateType RTC_DateDefault;
RTC_TimeType RTC_TimeStructure;
RTC_TimeType RTC_TimeDefault;
RTC_InitType RTC_InitStructure;
RTC_AlarmType RTC_AlarmStructure;
uint32_t SynchPrediv, AsynchPrediv;
/**
 * @brief  Display the current Date on the Hyperterminal.
 */
void RTC_DateShow(void)
{
    /* Get the current Date */
    RTC_GetDate(RTC_FORMAT_BIN, &RTC_DateStructure);
#if RTC_DBG
    log_info("\n\r //=========== Current Date Display ==============// \n\r");
    log_info("\n\r The current date (WeekDay-Date-Month-Year) is :  %0.2d-%0.2d-%0.2d-%0.2d \n\r",
             RTC_DateStructure.WeekDay,
             RTC_DateStructure.Date,
             RTC_DateStructure.Month,
             RTC_DateStructure.Year);
#endif
}

/**
 * @brief  Display the current time on the Hyperterminal.
 */
void RTC_TimeShow(void)
{
    /* Get the current Time and Date */
    RTC_GetTime(RTC_FORMAT_BIN, &RTC_TimeStructure);
//    log_info("\n\r //============ Current Time Display ===============// \n\r");
//    log_info("\n\r The current time (Hour-Minute-Second) is :  %0.2d:%0.2d:%0.2d \n\r",
//             RTC_TimeStructure.Hours,
//             RTC_TimeStructure.Minutes,
//             RTC_TimeStructure.Seconds);
#if RTC_DBG
    printf("\n\r The current time (Hour-Minute-Second) is :  %0.2d:%0.2d:%0.2d \n\r",
           RTC_TimeStructure.Hours,
           RTC_TimeStructure.Minutes,
           RTC_TimeStructure.Seconds);
#endif
    /* Unfreeze the RTC DAT Register */
    (void)RTC->DATE;
}
/**
 * @brief  RTC initalize default value.
 */
void RTC_DateAndTimeDefaultVale(void)
{ // Date
    RTC_DateDefault.WeekDay = 3;
    RTC_DateDefault.Date = 20;
    RTC_DateDefault.Month = 11;
    RTC_DateDefault.Year = 19;
    // Time
    RTC_TimeDefault.H12 = RTC_AM_H12;
    RTC_TimeDefault.Hours = 9;
    RTC_TimeDefault.Minutes = 51;
    RTC_TimeDefault.Seconds = 0;
}
/**
 * @brief  RTC date regulate with the default value.
 */
ErrorStatus RTC_DateRegulate(void)
{
    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;
#if RTC_DBG
    log_info("\n\r //=============Date Settings================// \n\r");

    log_info("\n\r Please Set WeekDay (01-07) \n\r");
#endif
    tmp_hh = RTC_DateDefault.WeekDay;
    if (tmp_hh == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_DateStructure.WeekDay = tmp_hh;
    }
#if RTC_DBG
    log_info(": %0.2d\n\r", tmp_hh);
#endif
    tmp_hh = 0xFF;
#if RTC_DBG
    log_info("\n\r Please Set Date (01-31) \n\r");
#endif
    tmp_hh = RTC_DateDefault.Date;
    if (tmp_hh == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_DateStructure.Date = tmp_hh;
    }
#if RTC_DBG
    log_info(": %0.2d\n\r", tmp_hh);
    log_info("\n\r Please Set Month (01-12)\n\r");
#endif
    tmp_mm = RTC_DateDefault.Month;
    if (tmp_mm == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_DateStructure.Month = tmp_mm;
    }
#if RTC_DBG
    log_info(": %0.2d\n\r", tmp_mm);

    log_info("\n\r Please Set Year (00-99)\n\r");
#endif
    tmp_ss = RTC_DateDefault.Year;
    if (tmp_ss == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_DateStructure.Year = tmp_ss;
    }
#if RTC_DBG
    log_info(": %0.2d\n\r", tmp_ss);
#endif
    /* Configure the RTC date register */
    if (RTC_SetDate(RTC_FORMAT_BIN, &RTC_DateStructure) == ERROR)
    {
#if RTC_DBG
        log_info("\n\r>> !! RTC Set Date failed. !! <<\n\r");
#endif
        return ERROR;
    }
    else
    {
#if RTC_DBG
        log_info("\n\r>> !! RTC Set Date success. !! <<\n\r");
#endif
        RTC_DateShow();
        return SUCCESS;
    }
}
/**
 * @brief  RTC time regulate with the default value.
 */
ErrorStatus RTC_TimeRegulate(void)
{
    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;
#if RTC_DBG
    log_info("\n\r //==============Time Settings=================// \n\r");
#endif
    RTC_TimeStructure.H12 = RTC_TimeDefault.H12;
#if RTC_DBG
    log_info("\n\r Please Set Hours \n\r");
#endif
    tmp_hh = RTC_TimeDefault.Hours;
    if (tmp_hh == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_TimeStructure.Hours = tmp_hh;
    }
#if RTC_DBG
    log_info(": %0.2d\n\r", tmp_hh);

    log_info("\n\r Please Set Minutes \n\r");
#endif
    tmp_mm = RTC_TimeDefault.Minutes;
    if (tmp_mm == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_TimeStructure.Minutes = tmp_mm;
    }
#if RTC_DBG
    log_info(": %0.2d\n\r", tmp_mm);

    log_info("\n\r Please Set Seconds \n\r");
#endif
    tmp_ss = RTC_TimeDefault.Seconds;
    if (tmp_ss == 0xff)
    {
        return ERROR;
    }
    else
    {
        RTC_TimeStructure.Seconds = tmp_ss;
    }
#if RTC_DBG
    log_info(": %0.2d\n\r", tmp_ss);
#endif
    /* Configure the RTC time register */
    if (RTC_ConfigTime(RTC_FORMAT_BIN, &RTC_TimeStructure) == ERROR)
    {
#if RTC_DBG
        log_info("\n\r>> !! RTC Set Time failed. !! <<\n\r");
#endif
        return ERROR;
    }
    else
    {

#if RTC_DBG
        log_info("\n\r>> !! RTC Set Time success. !! <<\n\r");
#endif
        RTC_TimeShow();
        return SUCCESS;
    }
}

/**
 * @brief  RTC prescaler config.
 */
static void RTC_PrescalerConfig(void)
{
    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_24HOUR_FORMAT;

    /* Check on RTC init */
    if (RTC_Init(&RTC_InitStructure) == ERROR)
    {
#if RTC_DBG
        log_info("\r\n //******* RTC Prescaler Config failed **********// \r\n");
#endif
    }
}

/**
 * @brief  Configures RTC.
 *   Configure the RTC peripheral by selecting the clock source
 */

void RTC_CLKSourceConfig(uint8_t ClkSrc)
{
    assert_param(IS_CLKSRC_VALUE(ClkSrc));

    /* Enable the PWR clock */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);

    /* Disable RTC clock */
    RCC_EnableRtcClk(DISABLE);

    if (ClkSrc == 0x01)
    {
#if RTC_DBG
        log_info("\r\n RTC_ClkSrc Is Set LSE! \r\n");
#endif

#if (_TEST_LSE_BYPASS_)
        RCC_ConfigLse(RCC_LSE_BYPASS);
#else
        RCC_ConfigLse(RCC_LSE_ENABLE);
#endif

        while (RCC_GetFlagStatus(RCC_LSCTRL_FLAG_LSERD) == RESET)
        {
        }

        RCC_ConfigLSXSEL(RCC_RTCCLK_SRC_LSE);
        RCC_EnableLsi(DISABLE);

        SynchPrediv = 0xFF;  // 32.768KHz
        AsynchPrediv = 0x7F; // value range: 0-7F
    }
    else if (ClkSrc == 0x02)
    {
#if RTC_DBG
        log_info("\r\n RTC_ClkSrc Is Set LSI! \r\n");
#endif
        /* Enable the LSI OSC */
        RCC_EnableLsi(ENABLE);
        while (RCC_GetFlagStatus(RCC_LSCTRL_FLAG_LSIRD) == RESET)
        {
        }

        RCC_ConfigLSXSEL(RCC_RTCCLK_SRC_LSI);

        SynchPrediv = 0xFF;  // 32.768KHz
        AsynchPrediv = 0x7F; // value range: 0-7F
    }
    else
    {
#if RTC_DBG
        log_info("\r\n RTC_ClkSrc Value is error! \r\n");
#endif
    }

    /* Enable the RTC Clock */
    RCC_EnableRtcClk(ENABLE);
    RTC_WaitForSynchro();
}
u8 rtcflag = 0;
void rtc_flag(void)
{
    rtcflag = 1;
    Vtimer_SetTimer(VTIM5, 500, rtc_flag);
}
/*!
    \brief      Rtc_Init(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Rtc_Init(void)
{
    /* RTC date time alarm default value*/
    //  RTC_DateAndTimeDefaultVale();

    /* RTC clock source select 1:LSE 2:LSI*/
    RTC_CLKSourceConfig(2);
    RTC_PrescalerConfig();
#if RTC_DBG
    log_info("RTC configured....");
#endif

    /* Adjust time by values entered by the user on the hyperterminal */
    /* Disable the RTC Wakeup Interrupt and delay more than 100us before set data and time */
    RTC_ConfigInt(RTC_INT_WUT, DISABLE);
    RTC_EnableWakeUp(DISABLE);
    //  RTC_DateRegulate();
    //  RTC_TimeRegulate();
    Vtimer_SetTimer(VTIM5, 500, rtc_flag);
}
/*!
    \brief      Rtc_Timeset(u8 hour,u8 min,u8 sec)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Rtc_Timeset(u8 hour, u8 min, u8 sec)
{
    // Time
    RTC_TimeDefault.Hours = hour;
    RTC_TimeDefault.Minutes = min;
    RTC_TimeDefault.Seconds = sec;
    RTC_TimeRegulate();
}
/*!
    \brief      Rtc_Task(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void Rtc_Task(void)
{
    if (1 == rtcflag)
    {
        rtcflag = 0;
        RTC_TimeShow();
    }
}

u8 m_rtcsetflag = 0;
/*!
    \brief      Rtc_Set(u8 set)
    \param[in]  u8 set
    \param[out] none
    \retval     none
*/
void Rtc_Set(u8 set)
{
    m_rtcsetflag = 1;
}

/*!
    \brief      u8 Rtc_SetStatus(void)
    \param[in]  none
    \param[out] none
    \retval     u8
*/
u8 Rtc_SetStatus(void)
{
    return m_rtcsetflag;
}
