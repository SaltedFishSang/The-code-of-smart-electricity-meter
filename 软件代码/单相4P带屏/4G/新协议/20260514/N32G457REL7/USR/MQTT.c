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
    Vtimer_SetTimer(VTIM10, 60000, HeartInfoBeatflag);
}
/*!
    \brief      HeartBegin 发起定时30s间隔的心跳
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HeartInfoBegin(void)
{
    Vtimer_SetTimer(VTIM10, 60000, HeartInfoBeatflag);
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
    \brief      HeartTimerPoint()
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HeartTimerPoint(void)
{
    char send1[50];
    char send3[200];
    char send4[200];
    char send2[1000];
    static u8 flag = 0;
    u8 i = 0;
    // char *send2 = NULL;
    sprintf(send1, "status/%s", g_Equipment_number);
    // send2 = (char *)malloc(500 * sizeof(char));
    // if (send2 != NULL)
    {
        strcpy(send2, "");
        strcat(send2, "[");
        for (i = 0; i < 19; i++)
        {
            // printf("%d\r\n", i);
            // sprintf(send3, "{\"k\":\"Timer.N1%d\",\"v\":\"%s-%d:%d-%d\"}", i, TimerPoint[i].weekday,
            //         TimerPoint[i].hour, TimerPoint[i].min, TimerPoint[i].onoff);

            sprintf(send3, "{\"k\":\"Timer.N%d\",\"v\":\"", i + 1);
            strcat(send3, TimerPoint[i].weekday);
            sprintf(send4, "-%02d:%02d-%d\"},", TimerPoint[i].hour, TimerPoint[i].min, TimerPoint[i].onoff);
            strcat(send3, send4);
            strcat(send2, send3);
        }

        sprintf(send3, "{\"k\":\"Timer.N%d\",\"v\":\"", 19 + 1);
        strcat(send3, TimerPoint[i].weekday);
        sprintf(send4, "-%02d:%02d-%d\"}", TimerPoint[19].hour, TimerPoint[19].min, TimerPoint[19].onoff);
        strcat(send3, send4);
        strcat(send2, send3);

        strcat(send2, "]");
        Set_LTE_SendData(1, send1, strlen(send1), send2, strlen(send2));
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
    char send2[1000];
    u8 i = 0;
    // char *send2 = NULL;
    sprintf(send1, "status/%s", g_Equipment_number);
    // send2 = (char *)malloc(500 * sizeof(char));
    // if (send2 != NULL)
    {
        strcpy(send2, "");
        strcat(send2, "[");
        // sprintf(send3, "{ \"k\":\"On\",\"v\":\"%d\"},", (Switch_Status.SwitchOnvalue == 1) ? "on" : "off");
        // strcat(send2, send3);
        // sprintf(send3, "{ \"k\":\"Lock\",\"v\":\"%d\"},", (Switch_Status.SwitchLockvalue == 1) ? "on" : "off");
        sprintf(send3, "{\"k\":\"On\",\"v\":\"%d\"},", Switch_Status.SwitchOnvalue);
        strcat(send2, send3);
        sprintf(send3, "{\"k\":\"Local\",\"v\":\"%d\"},", Switch_Status.SwitchLockvalue);
        strcat(send2, send3);
        sprintf(send3, "{\"k\":\"V\",\"v\":\"%d\"},", BL0942_EleData.vol);
        strcat(send2, send3);
        sprintf(send3, "{\"k\":\"C\",\"v\":\"%d\"},", BL0942_EleData.current);
        strcat(send2, send3);
        sprintf(send3, "{ \"k\":\"P\",\"v\":\"%d\"},", BL0942_EleData.power);
        strcat(send2, send3);
        sprintf(send3, "{\"k\":\"Pf\",\"v\":\"%d\"},", BL0942_EleData.freq);
        strcat(send2, send3);
        sprintf(send3, "{\"k\":\"E\",\"v\":\"%d\"},", BL0942_EleData.energy);
        strcat(send2, send3);
        sprintf(send3, "{\"k\":\"SafeParam.OverV\",\"v\":\"%d\"},", Switch_Status.OverVValue);
        strcat(send2, send3);
        sprintf(send3, "{\"k\":\"SafeParam.UnderV\",\"v\":\"%d\"},", Switch_Status.UnderVValue);
        strcat(send2, send3);
        sprintf(send3, "{\"k\":\"SafeParam.OverC\",\"v\":\"%d\"},", Switch_Status.OverCValue);
        strcat(send2, send3);
        sprintf(send3, "{\"k\":\"SafeParam.OverP\",\"v\":\"%d\"},", Switch_Status.OverPValue);
        strcat(send2, send3);
        sprintf(send3, "{\"k\":\"Timer.On\",\"v\":\"%d\"}", Switch_Status.SwitchTimevalue);
        strcat(send2, send3);
        strcat(send2, "]");
        Set_LTE_SendData(1, send1, strlen(send1), send2, strlen(send2));
        // free(send2);
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
    char *p = NULL;
    char k_str[20];
    char v_str[100];
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
        memcpy(MqttRecBuf, data, len);
        MqttReclen = len;
        // if (strcmp(MqttRecBuf, "[{\"k\":\"On\",\"v\":\"0\"}]") == 0)
        // {
        //     Relay_Control(0);
        //     DisplayOutputState(0);
        //     Switch_Status.SwitchOnvalue = 0;
        //     EEP_Write8(SwitchOn, Switch_Status.SwitchOnvalue);

        //     DisplayClockState(0); // 退出定时模式
        //     Switch_Status.SwitchTimevalue = 0;
        //     EEP_Write8(SwitchTime, Switch_Status.SwitchTimevalue);

        //     HeartInfoLimit();
        // }
        // if (strcmp(MqttRecBuf, "[{\"k\":\"On\",\"v\":\"1\"}]") == 0)
        // {
        //     Relay_Control(1);
        //     DisplayOutputState(1);
        //     Switch_Status.SwitchOnvalue = 1;
        //     EEP_Write8(SwitchOn, Switch_Status.SwitchOnvalue);

        //     DisplayClockState(0); // 退出定时模式
        //     Switch_Status.SwitchTimevalue = 0;
        //     EEP_Write8(SwitchTime, Switch_Status.SwitchTimevalue);

        //     HeartInfoLimit();
        // }
        // if (strcmp(MqttRecBuf, "[{\"k\":\"Local\",\"v\":\"0\"}]") == 0)
        // {
        //     DisplayLockState(0);
        //     Switch_Status.SwitchLockvalue = 0;
        //     EEP_Write8(SwichLock, Switch_Status.SwitchLockvalue);
        //     HeartInfoLimit();
        // }
        // if (strcmp(MqttRecBuf, "[{\"k\":\"Local\",\"v\":\"1\"}]") == 0)
        // {
        //     DisplayLockState(1);
        //     Switch_Status.SwitchLockvalue = 1;
        //     EEP_Write8(SwichLock, Switch_Status.SwitchLockvalue);
        //     HeartInfoLimit();
        // }
        // if (strcmp(MqttRecBuf, "[{\"k\":\"Timer.On\",\"v\":\"0\"}]") == 0)
        // {
        //     DisplayClockState(0);
        //     Switch_Status.SwitchTimevalue = 0;
        //     EEP_Write8(SwitchTime, Switch_Status.SwitchTimevalue);
        //     HeartInfoLimit();
        // }
        // if (strcmp(MqttRecBuf, "[{\"k\":\"Timer.On\",\"v\":\"1\"}]") == 0)
        // {
        //     DisplayClockState(1);
        //     Switch_Status.SwitchTimevalue = 1;
        //     EEP_Write8(SwitchTime, Switch_Status.SwitchTimevalue);
        //     HeartInfoLimit();
        // }

        // p = strstr((const char *)MqttRecBuf, (const char *)"[{\"k\":\"Timer.N");
        // if (p != NULL)
        // {
        //     printf("get timer point\r\n");
        //     p += 14;
        //     printf("%c\r\n", *p);
        //     GetTimerPoint(p);
        // }
        sscanf(MqttRecBuf, "[{\"k\":\"%[^\"]\",\"v\":\"%[^\"]\"}]", k_str, v_str);
        // printf("k=%s v=%s\r\n", k_str, v_str);
        if (strcmp(k_str, "On") == 0)
        {
            if (strcmp(v_str, "0") == 0)
            {
                Relay_Control(0);
                DisplayOutputState(0);
                Switch_Status.SwitchOnvalue = 0;
            }
            if (strcmp(v_str, "1") == 0)
            {
                Relay_Control(1);
                DisplayOutputState(1);
                Switch_Status.SwitchOnvalue = 1;
            }
            EEP_Write8(SwitchOn, Switch_Status.SwitchOnvalue);
            DisplayClockState(0); // 退出定时模式
            Switch_Status.SwitchTimevalue = 0;
            EEP_Write8(SwitchTime, Switch_Status.SwitchTimevalue);
            HeartInfoLimit();
        }
        else if (strcmp(k_str, "Local") == 0)
        {
            if (strcmp(v_str, "0") == 0)
            {
                DisplayLockState(0);
                Switch_Status.SwitchLockvalue = 0;
            }
            if (strcmp(v_str, "1") == 0)
            {
                DisplayLockState(1);
                Switch_Status.SwitchLockvalue = 1;
            }
            EEP_Write8(SwichLock, Switch_Status.SwitchLockvalue);
            HeartInfoLimit();
        }

        else if (strcmp(k_str, "Timer.On") == 0)
        {
            if (strcmp(v_str, "0") == 0)
            {
                DisplayClockState(0);
                Switch_Status.SwitchTimevalue = 0;
            }
            if (strcmp(v_str, "1") == 0)
            {
                DisplayClockState(1);
                Switch_Status.SwitchTimevalue = 1;
            }
            EEP_Write8(SwitchTime, Switch_Status.SwitchTimevalue);
            HeartInfoLimit();
        }
        else if (strcmp(k_str, "SafeParam.OverV") == 0)
        {
            Switch_Status.OverVValue = atoi(v_str);
            EEP_Write16(OverV, Switch_Status.OverVValue);
            HeartInfoLimit();
        }
        else if (strcmp(k_str, "SafeParam.UnderV") == 0)
        {
            Switch_Status.UnderVValue = atoi(v_str);
            EEP_Write16(UnderV, Switch_Status.UnderVValue);
            HeartInfoLimit();
        }
        else if (strcmp(k_str, "SafeParam.OverC") == 0)
        {
            Switch_Status.OverCValue = atoi(v_str);
            EEP_Write32(OverC, Switch_Status.OverCValue);
            HeartInfoLimit();
        }
        else if (strcmp(k_str, "SafeParam.OverP") == 0)
        {
            Switch_Status.OverPValue = atoi(v_str);
            EEP_Write32(OverP, Switch_Status.OverPValue);
            HeartInfoLimit();
        }
        /*定时点下发处理*/
        {
            p = strstr((const char *)MqttRecBuf, (const char *)"[{\"k\":\"Timer.N");
            if (p != NULL)
            {
                printf("get timer point\r\n");
                p += 14;
                //  printf("%c\r\n", *p);
                GetTimerPoint(p);
            }
        }
        memset(MqttRecBuf, 0, MqttReclen);
        break;
    default:
        break;
    }
}
