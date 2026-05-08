#include "BSP.h"
u8 m_mqtt_errnum = 0;
u8 m_mqttlinkflag = 0;
void HeartInfoLimit(void);

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
    Vtimer_SetTimer(VTIM10, 10000, HeartInfoBeatflag);
}
/*!
    \brief      HeartBegin 发起定时30s间隔的心跳
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HeartInfoBegin(void)
{
    Vtimer_SetTimer(VTIM10, 10000, HeartInfoBeatflag);
}

/*!
    \brief      HeartInfo
    \param[in]  none
    \param[out] none
    \retval     none
*/

void HeartInfo(void)
{
    if (Heartinfoflag)
    {
        Heartinfoflag = 0;
        HeartInfoLimit();
        m_mqtt_errnum++;
        if (m_mqtt_errnum >= 10)
        {
            m_mqtt_errnum = 0;
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
    char send3[50];
    char *send2 = NULL;
    u8 flag = 0;
    sprintf(send1, "status/%s", g_Equipment_number);
    send2 = (char *)malloc(1000 * sizeof(char));
    if (send2 != NULL)
    {
        strcpy(send2, "");
        strcat(send2, "[{");

        sprintf(send3, "\"On\":\"%s\"", (flag == 1) ? "on" : "off");
        strcat(send2, send3);

        sprintf(send3, "\"Lock\":\"%s\"", (flag == 1) ? "on" : "off");
        strcat(send2, send3);

        sprintf(send3, "\"V\":\"%s\"", flag);
        strcat(send2, send3);

        sprintf(send3, "\"C\":\"%s\"", flag);
        strcat(send2, send3);

        sprintf(send3, "\"P\":\"%s\"", flag);
        strcat(send2, send3);

        sprintf(send3, "\"Pf\":\"%s\"", flag);
        strcat(send2, send3);

        sprintf(send3, "\"E\":\"%s\"", flag);
        strcat(send2, send3);

        sprintf(send3, "\"OverV\":\"%s\"", flag);
        strcat(send2, send3);

        sprintf(send3, "\"UnderV\":\"%s\"", flag);
        strcat(send2, send3);

        sprintf(send3, "\"OverC\":\"%s\"", flag);
        strcat(send2, send3);

        sprintf(send3, "\"OverP\":\"%s\"", flag);
        strcat(send2, send3);

        strcat(send2, "}]");

        // sprintf(send2, "[{\"On\":\"%s\",\"v\":\"%s\"}]", "on", "Normal");
        Set_LTE_SendData(1, send1, strlen(send1), send2, strlen(send2));
        free(send2);
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
        m_mqtt_errnum = 0;
        memcpy(recedata, data, 4); // 年
        recedata[4] = '\0';
        year = atoi(recedata);
        memcpy(recedata, (data + 5), 2); // 月
        recedata[2] = '\0';
        month = atoi(recedata);
        memcpy(recedata, (data + 8), 2); // 日
        recedata[2] = '\0';
        day = atoi(recedata);
        memcpy(recedata, (data + 11), 2); // 时
        recedata[2] = '\0';
        hour = atoi(recedata);
        memcpy(recedata, (data + 14), 2); // 分
        recedata[2] = '\0';
        min = atoi(recedata);
        memcpy(recedata, (data + 17), 2); // 秒
        recedata[2] = '\0';
        sec = atoi(recedata);
        // printf("MQTT Set Time:%d-%d-%d %d:%d:%d\r\n", year, month, day, hour, min, sec);
        Set_RTC_Time(year, month, day, hour, min, sec);
        break;
    case 1:
        m_mqttlinkflag = 1;
        DisplayNetState(1);
        m_mqtt_errnum = 0;
        MqttReclen = Ascii2Hex(MqttRecBuf, data, len);
        ModbusDataHandle();
        break;
    default:
        break;
    }
}
