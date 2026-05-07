#include "BSP.h"
u8 g_mqtt_errnum = 0;
u8 m_mqttlinkflag = 0;
u8 get_mqttlink(void)
{
    return m_mqttlinkflag;
}
void set_mqttlink(u8 state)
{
    m_mqttlinkflag = state;
    DisplayNetState(state);
}
u8 Heartinfoflag = 0;
/*!
    \brief      HeartBeatflag 30s到置心跳标致
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HeartInfoBeatflag(void)
{
    Heartinfoflag = 1;
    Vtimer_SetTimer(VTIM10, 303000, HeartInfoBeatflag);
}
/*!
    \brief      HeartBegin 发起定时30s间隔的心跳
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HeartInfoBegin(void)
{
    Vtimer_SetTimer(VTIM10, 303000, HeartInfoBeatflag);
}

/*!
    \brief      HeartInfo
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HeartInfo(void)
{
    char send1[50];
    char send2[100];
    if (Heartinfoflag)
    {
        Heartinfoflag = 0;
        sprintf(send1, "modbus_info/%s", g_Equipment_number);
        sprintf(send2, "%s_%s_%s", LTE.qccid, LTE.imei, LTE.csq);
        Set_LTE_SendData(1, send1, strlen(send1), send2, strlen(send2));
        g_mqtt_errnum++;
        if (g_mqtt_errnum >= 3)
        {
            g_mqtt_errnum = 0;
            NVIC_SystemReset();
            m_mqttlinkflag = 0;
            DisplayNetState(0);
        }
    }
}
/*!
    \brief      HeartLimit
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HeartInfoLimit(void)
{
    char send1[50];
    char send2[150];
    sprintf(send1, "modbus_info/%s", g_Equipment_number);
    sprintf(send2, "%s_%s_%s", LTE.qccid, LTE.imei, LTE.csq);
    Set_LTE_SendData(1, send1, strlen(send1), send2, strlen(send2));
}

u8 Heartstatusflag = 0;
/*!
    \brief      HeartBeatflag 30s到置心跳标致
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HeartStatusflag(void)
{
    Heartstatusflag = 1;
    Vtimer_SetTimer(VTIM11, 120000, HeartStatusflag);
}
/*!
    \brief      HeartStausBegin 发起定时间隔发送status
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HeartStausBegin(void)
{
    Vtimer_SetTimer(VTIM11, 120000, HeartStatusflag);
}
/*!
    \brief      HeartStatus
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HeartStatus(void)
{
    if (Heartstatusflag == 1)
    {
        Heartstatusflag = 0;
        if (QueryLteState())
        {
            // printf("心跳\r\n");
            ModbusSendAll();
        }
    }
}

/*!
    \brief      MqttrecedataHandle Mqtt收到数据处理
    \param[in]  data 收到数据缓存指针； len:收到数据长度 ； flag:0：设置时间 1：控制下发
    \param[out] none
    \retval     none
*/
void MqttrecedataHandle(u8 *data, u16 len, u8 flag)
{
    char recedata[100];
    u16 year;
    u8 month, day, hour, min, sec;
    (void)year;
    (void)month;
    (void)day;
    switch (flag)
    {
    case 0:
        m_mqttlinkflag = 1;
        DisplayNetState(1);
        g_mqtt_errnum = 0;
        memcpy(recedata, data, 4);
        recedata[4] = '\0';
        year = atoi(recedata);
        memcpy(recedata, (data + 4), 2);
        recedata[2] = '\0';
        month = atoi(recedata);
        memcpy(recedata, (data + 6), 2);
        recedata[2] = '\0';
        day = atoi(recedata);
        memcpy(recedata, (data + 8), 2);
        recedata[2] = '\0';
        hour = atoi(recedata);
        memcpy(recedata, (data + 10), 2);
        recedata[2] = '\0';
        min = atoi(recedata);
        memcpy(recedata, (data + 12), 2);
        recedata[2] = '\0';
        sec = atoi(recedata);
        //   printf("MQTT Set Time:%d-%d-%d %d:%d:%d\r\n", year, month, day, hour, min, sec);
        Set_RTC_Time(year, month, day, hour, min, sec);
        break;
    case 1:
        m_mqttlinkflag = 1;
        DisplayNetState(1);
        g_mqtt_errnum = 0;
        MqttReclen = Ascii2Hex(MqttRecBuf, data, len);
        ModbusDataHandle();
        break;
    default:
        break;
    }
}
