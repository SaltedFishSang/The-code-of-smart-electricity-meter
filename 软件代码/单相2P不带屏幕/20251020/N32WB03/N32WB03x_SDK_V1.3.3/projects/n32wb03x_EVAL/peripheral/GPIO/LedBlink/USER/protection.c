#include "BSP.h"
/*!
    \brief      Over_Vol  过压
    \param[in]  none
    \param[out] none
    \retval    none
*/
u8 m_overflag = 0;
void overstatusflag(void)
{
    m_overflag = 1;
}
void Over_vol(void)
{
    u16 data = 0;
    u16 data1 = 0;
    data = Modbus_Readinput(11);  // 先读取目前的故障告警数据
    data1 = Modbus_Readremain(9); // 获取保护跳闸开关
    static u8 state = 0;
    switch (state)
    {
    case 0:
        if (Modbus_Readinput(4) >= Modbus_Readremain(3)) // 过压
        {
            state = 1;
            Vtimer_SetTimer(VTIM12, 2000, overstatusflag);
        }
        break;
    case 1:
        if (1 == m_overflag)
        {
            m_overflag = 0;
            state = 2;
        }
        break;
    case 2:
        if (Modbus_Readinput(4) >= Modbus_Readremain(3)) // 过压
        {
            state = 3;
        }
        else
        {
            state = 0;
        }
        break;
    case 3:
        data |= (1 << 1);             // 配上最新的告警数据
        Modbus_WrieteInput(11, data); // 再写入输入寄存器 故障原因过压
        Modbus_WirteDI(0, 1);         // 产生故障
        if (data1 & (1 << 0))
        {
            RelayControl(0);
            Modbus_WirteDI(1, 0);
            Modbus_WrieteInput(10, 1); // 再写入输入寄存器 最后一次分闸原因  过压
            ModbusSendAll();
        }
        state = 0;
        break;
    default:
        break;
    }
}
u8 m_loseflag = 0;
void losestatusflag(void)
{
    m_loseflag = 1;
}
/*!
    \brief      Lose_Vol  欠压
    \param[in]  none
    \param[out] none
    \retval    none
*/
void Lose_Vol(void)
{
    u16 data = 0;
    u16 data1 = 0;
    static u8 state = 0;
    data = Modbus_Readinput(11);  // 先读取目前的故障告警数据
    data1 = Modbus_Readremain(9); // 获取保护跳闸开关
    if (Modbus_ReadDI(1) == 0)    // 处于分闸位置
    {
        state = 0;
        Vtimer_KillTimer(VTIM13);
        m_loseflag = 0;
    }
    switch (state)
    {
    case 0:
        if (Modbus_Readinput(4) <= Modbus_Readremain(4)) // 欠压
        {
            state = 1;
            Vtimer_SetTimer(VTIM13, 3000, losestatusflag);
        }
        break;
    case 1:
        if (m_loseflag == 1) // 1000ms到
        {
            m_loseflag = 0;
            if (Modbus_Readinput(4) <= Modbus_Readremain(4)) // 欠压
            {
                state = 2;
            }
            else
            {
                state = 0;
            }
        }
        break;
    case 2:
        data |= (1 << 2);             // 配上最新的告警数据
        Modbus_WrieteInput(11, data); // 再写入输入寄存器 故障原因欠压
        Modbus_WirteDI(0, 1);         // 产生故障
        if (data1 & (1 << 1))
        {
            RelayControl(0);
            Modbus_WirteDI(1, 0);
            Modbus_WrieteInput(10, 2); // 再写入输入寄存器 最后一次分闸原因  欠压
            ModbusSendAll();
        }
        state = 0;
        break;
    default:
        break;
    }
}
u8 m_getstatusflag = 0;
void statusflag(void)
{
    m_getstatusflag = 1;
}
/*!
    \brief      Over_Cur  过流
    \param[in]  none
    \param[out] none
    \retval    none
*/
void Over_Cur(void)
{
    u16 data = 0;
    u16 data1 = 0;
    data = Modbus_Readinput(11);  // 先读取目前的故障告警数据
    data1 = Modbus_Readremain(9); // 获取保护跳闸开关
    static u8 state = 0;
    switch (state)
    {
    case 0:
        if ((Modbus_Readinput(5) * 10) >= (Modbus_Readremain(5) * 10)) // 过流
        {
            state = 1;
            Vtimer_SetTimer(VTIM14, 2000, statusflag);
        }
        break;
    case 1:
        if (1 == m_getstatusflag)
        {
            m_getstatusflag = 0;
            state = 2;
        }
        break;
    case 2:
        if ((Modbus_Readinput(5) * 10) >= (Modbus_Readremain(5) * 10)) // 依旧过流
        {
            state = 3;
        }
        else
        {
            state = 0;
        }
        break;
    case 3:
        data |= (1 << 3);             // 配上最新的告警数据
        Modbus_WrieteInput(11, data); // 再写入输入寄存器 故障原因过流
        Modbus_WirteDI(0, 1);         // 产生故障
        if (data1 & (1 << 2))
        {
            RelayControl(0);
            Modbus_WirteDI(1, 0);
            Modbus_WrieteInput(10, 3); // 再写入输入寄存器 最后一次分闸原因  过流
            ModbusSendAll();
        }
        state = 0;
        break;
    default:
        break;
    }
}

/*!
    \brief      Lose_Cur  漏电
    \param[in]  none
    \param[out] none
    \retval    none
*/
void Lose_Cur(void)
{
    // data = Modbus_Readinput(11);   // 先读取目前的故障告警数据
    //    if (Modbus_Readinput(1) >= 25) // 漏电
    //    {
    //        data |= (1 << 4);             // 配上最新的告警数据
    //        Modbus_WrieteInput(11, data); // 再写入输入寄存器 故障原因漏电
    //        Modbus_WirteDI(0, 1);         // 产生故障
    //    }
}

/*!
    \brief      Over_Temp  过温
    \param[in]  none
    \param[out] none
    \retval    none
*/
u8 m_a_tempflag = 0;
void a_tempstatusflag(void)
{
    m_a_tempflag = 1;
}

void Over_Temp(void)
{
    u16 data = 0;
    u16 data1 = 0;
    data = Modbus_Readinput(11);  // 先读取目前的故障告警数据
    data1 = Modbus_Readremain(9); // 获取保护跳闸开关
    static u8 state = 0;
    switch (state)
    {
    case 0:
        if (Modbus_Readinput(3) >= Modbus_Readremain(7)) // A过温
        {
            state = 1;
            Vtimer_SetTimer(VTIM15, 2000, a_tempstatusflag);
        }
        break;
    case 1:
        if (1 == m_a_tempflag)
        {
            m_a_tempflag = 0;
            state = 2;
        }
        break;
    case 2:
        if (Modbus_Readinput(3) >= Modbus_Readremain(7)) // A过温
        {
            state = 3;
        }
        else
        {
            state = 0;
        }
        break;
    case 3:
        data |= (1 << 5);             // 配上最新的告警数据
        Modbus_WrieteInput(11, data); // 再写入输入寄存器 故障原因过流
        Modbus_WirteDI(0, 1);         // 产生故障
        if (data1 & (1 << 3))
        {
            RelayControl(0);
            Modbus_WirteDI(1, 0);
            Modbus_WrieteInput(11, 5); // 再写入输入寄存器 最后一次分闸原因  过温
            ModbusSendAll();
        }
        state = 0;
        break;
    default:
        break;
    }
}
/*!
    \brief      Over_Power 过载
    \param[in]  none
    \param[out] none
    \retval    none
*/
u8 m_powerlag = 0;
void powerstatusflag(void)
{
    m_powerlag = 1;
}
void Over_Power(void)
{
    u16 data = 0;
    u16 data1 = 0;
    static u8 state = 0;
    data = Modbus_Readinput(11);  // 先读取目前的故障告警数据
    data1 = Modbus_Readremain(9); // 获取保护跳闸开关
    switch (state)
    {
    case 0:
        if (Modbus_Readinput(7) >= Modbus_Readremain(8)) // 过载
        {
            state = 1;
            Vtimer_SetTimer(VTIM16, 2000, powerstatusflag);
        }
        break;
    case 1:
        if (1 == m_powerlag)
        {
            m_powerlag = 0;
            state = 0;
        }
        break;
    case 2:
        if (Modbus_Readinput(7) >= Modbus_Readremain(8)) // 过载
        {
            state = 3;
        }
        else
        {
            state = 0;
        }
        break;
    case 3:
        data |= (1 << 6);             // 配上最新的告警数据
        Modbus_WrieteInput(11, data); // 再写入输入寄存器 故障原因过载
        Modbus_WirteDI(0, 1);         // 产生故障
        if (data1 & (1 << 4))
        {
            RelayControl(0);
            Modbus_WirteDI(1, 0);
            Modbus_WrieteInput(10, 6); // 再写入输入寄存器 最后一次分闸原因  过载
            ModbusSendAll();
        }
        state = 0;
        break;
    }
}
/*!
    \brief      Fault_tripping  故障跳闸
    \param[in]  none
    \param[out] none
    \retval    none
*/
void Fault_tripping(void)
{
    Over_vol();   // 过压
    Lose_Vol();   // 欠压
    Over_Cur();   // 过流
    Over_Temp();  // 过温
    Over_Power(); // 过载
}
void TimeControl(void) // 定时功能
{
    u8 buttonflag = 0;
    u8 i = 0;
    if (Modbus_ReadDI(4) == 1) // 定时模式下
    {
        // 先判断时间点是否一样
        // 再判断星期几是否开启 决定是否开启或者关闭继电器
        for (i = 0; i < Modbus_Readremain(13); i++) // 先看有多少个点
        {
            // 判断现在的时间点
            if ((RTC_TimeStructure.Hours == (Modbus_Readremain(15 + 2 * i) >> 8)) && (RTC_TimeStructure.Minutes == (Modbus_Readremain(15 + 2 * i) & 0xff))) // 在时间点上
            {
                //   if ((Modbus_Readremain(15 + 2 * i - 1) & 0xff) & (1 << (RTC_DateStructure.WeekDay - 1))) // 判断今天周几  这个点今天有没有选上
                {
                    // 选上了 判断这个点是开还是关
                    if ((Modbus_Readremain(15 + 2 * i - 1) & 0xff) & (1 << 7))
                    {
                        buttonflag = 1;
                    }
                    else
                    {
                        buttonflag = 2;
                    }
                }
                // else // 今天这个时间点 没有选上
                {
                }
            }
        }
        if (buttonflag == 1)
        {
            if (Modbus_ReadDI(1) == 0)
            {
                if (Modbus_ReadDI(0) == 0) // 无故障才可以定时成功
                {
                    Modbus_WirteDI(1, 1); // 合闸
                    RelayControl(1);
                    ModbusSendAll();
                }
            }
        }
        if (buttonflag == 2)
        {
            if (Modbus_ReadDI(1) == 1)
            {
                Modbus_WirteDI(1, 0); // 分闸
                RelayControl(0);
                Modbus_WrieteInput(10, 9); // 更新最后一次分闸原因 本地操作
                ModbusSendAll();
            }
        }
    }
}
