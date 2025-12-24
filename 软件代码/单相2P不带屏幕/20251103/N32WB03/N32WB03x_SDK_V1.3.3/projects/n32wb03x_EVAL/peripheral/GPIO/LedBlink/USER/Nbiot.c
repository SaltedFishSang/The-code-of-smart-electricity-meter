#include "BSP.h"
#include "string.h"
u8 g_NBbuf[NBBUFLEN];
u16 g_NBRxCnt;
//_LTE LTE_Status;
typedef enum
{
    REBOOT = 0,
    Query_Sim_Card_Status,
    Network_Reg_Status,
    ATTACH,
    CCLK,
    PDP_Context,
    Activate_PDP,
    GETCSQ,
    Creat_MQTT,
    Get_Tcp_State,
    NBSTANDBY,
    SENDNMGS
} LTESTATES;
_LTE LTE;
u8 m_NeedReboot = 0;
u8 m_TopicTxBuf[200] = {0}; // NB发送缓存
u16 m_TopicTxCnt = 0;       // NB发送计数器
u8 m_DataTxBuf[1000] = {0}; // NB发送缓存
u16 m_DataTxCnt = 0;        // NB发送计数器
u8 m_NBTxReslt = 0;         // NB发送完成标志
u8 m_TimeOut = 0;
u8 m_Delay = 0;
u8 m_mqttlinkstate = 0;
u8 m_senddataflag = 0;
u8 QueryLteState(void)
{
    return m_mqttlinkstate;
}

u8 GetLTESendstatus(void)
{
    if (m_senddataflag)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void Set_LTE_SendData(u8 sendflag, char *topic, u16 topiclen, char *data, u16 datalen)
{
    m_senddataflag = sendflag;
    m_TopicTxCnt = topiclen;
    memset(m_TopicTxBuf, 0, 200);
    memcpy(m_TopicTxBuf, topic, m_TopicTxCnt);
    m_DataTxCnt = datalen;
    memset(m_DataTxBuf, 0, 1000);
    memcpy(m_DataTxBuf, data, m_DataTxCnt);
}

void NB_Set_TimeOut(void)
{
    m_TimeOut = 1;
}

u8 NB_Check_TimeOutTimer(void)
{
    u8 temp;
    if (m_TimeOut)
    {
        temp = 1;
        m_TimeOut = 0;
    }
    else
        temp = 0;
    return temp;
}

void NB_Start_TimeOutTimer(u16 t)
{
    m_TimeOut = 0;
    Vtimer_SetTimer(VTIM12, t, NB_Set_TimeOut); // 超时判断为8s
}

void NB_ReSet_TimeOutTimer(void)
{
    m_TimeOut = 0;
    Vtimer_KillTimer(VTIM12);
}

void NB_ClearRxBuf(void)
{
    u16 i;
    for (i = 0; i < 1000; i++)
        g_NBbuf[i] = 0;
    g_NBRxCnt = 0;
}

void LTE_REBOOT(void)
{
    m_NeedReboot = 1;
    m_mqttlinkstate = 0;
}
/*!
    \brief     LTE_ResetGpioInit()
    \param[in]  none
    \param[out] none
    \retval     none
*/
void LTE_ResetGpioInit(void)
{
    /*PB10*/
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_PIN_10);
}

#define LTE_ResetLow GPIO_SetBits(GPIOB, GPIO_PIN_10)
#define LTE_ResetHigh GPIO_ResetBits(GPIOB, GPIO_PIN_10)

u8 LTE_Reset(void)
{
    static u8 state = 0;
    switch (state)
    {
    case 0:
        m_mqttlinkstate = 0;
        Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
        NB_ClearRxBuf();            // 清除上电开机模组返回的信息
        LTE_ResetLow;               // 拉低复位脚
        NB_Start_TimeOutTimer(500); // 拉低500ms
        state = 1;
        return 0;
    case 1:
        if (NB_Check_TimeOutTimer())
        {
            LTE_ResetHigh;               // 500ms到 拉高复位脚
            NB_Start_TimeOutTimer(5000); // 等待10s 查看收到RDY 信息没有
            state = 2;
        }
        return 0;
    case 2:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            state = 0; // 时间到 仍未收到RDY信息 复位失败
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}

/*!
    \brief     查询SIM卡状态并获取SIM卡号和IMEI号
    \param[in]  none
    \param[out] none
    \retval     none
*/
u8 Query_Sim(void)
{
    static u8 state = 0;
    char *p = NULL;
    switch (state)
    {
    case 0:
        Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
        NB_ClearRxBuf();
        Uart1_SendData("AT\r\n", 4); // 关闭回显
        NB_Start_TimeOutTimer(2000);
        state = 1;
        return 0;
    case 1:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"OK"); // 检查关键字
            if (p != NULL)
            {
                state = 2;
                return 0;
            }
            else
            {
                state = 0;
                return 2;
            }
        }
        return 0;
    case 2:
        NB_Start_TimeOutTimer(2000);
        state = 3;
        return 0;
    case 3:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            state = 4;
        }
        return 0;
    case 4:
        NB_ClearRxBuf();
        Uart1_SendData("AT+CPIN?\r\n", 10);
        NB_Start_TimeOutTimer(100);
        state = 5;
        return 0;
    case 5:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"+CPIN: READY"); // 检查关键字
            if (p != NULL)
            {
                state = 6;
                return 0;
            }
            else
            {
                state = 0;
                return 2;
            }
        }
        return 0;
    case 6:
        NB_ClearRxBuf();
        Uart1_SendData("AT+CGSN=1\r\n", 11);
        NB_Start_TimeOutTimer(100);
        state = 7;
        return 0;
    case 7:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"+CGSN: "); // 检查关键字
            if (p != NULL)
            {
                memcpy(LTE.imei, (p + 7), 15);
                LTE.imei[15] = '\0';
                // printf("LTE.imei==%s\r\n", LTE.imei);
                state = 8;
                return 0;
            }
            else
            {
                state = 0;
                return 2;
            }
        }
        return 0;
    case 8:
        NB_ClearRxBuf();
        Uart1_SendData("AT+ICCID\r\n", 10);
        NB_Start_TimeOutTimer(100);
        state = 9;
        return 0;
    case 9:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"+ICCID: "); // 检查关键字
            if (p != NULL)
            {
                memcpy(LTE.qccid, (p + 8), 20);
                LTE.qccid[20] = '\0';
                state = 0;
                return 1;
            }
            else
            {
                state = 0;
                return 2;
            }
        }
        return 0;
    default:
        return 0;
    }
}

/*!
    \brief     查询网络注册状态
    \param[in]  none
    \param[out] none
    \retval     none
*/
u8 querynum = 0;

u8 Query_Network_Reg_Status(void)
{
    static u8 state = 0;
    char *p = NULL;
    switch (state)
    {
    case 0:
        querynum++;
        NB_ClearRxBuf();
        Uart1_SendData("AT+CEREG?\r\n", 11); // 关闭回显
        NB_Start_TimeOutTimer(100);
        state = 1;
        return 0;
    case 1:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"+CEREG: 0,1"); // 本地网络已经注册上
            // p1 = strstr((const char *)g_NBbuf, (const char *)"+CREG: 0,2"); // 漫游注册上
            // if ((p != NULL) || (p1 != NULL))
            // {
            //     state = 0;
            //     querynum = 0;
            //     return 1;
            // }
            if ((p != NULL))
            {
                state = 0;
                querynum = 0;
                return 1;
            }
            else
            {
                if (querynum <= 90)
                {
                    state = 0;
                    return 0;
                }
                else
                {
                    querynum = 0;
                    state = 0;
                    return 2;
                }
            }
        }
        return 0;
    default:
        return 0;
    }
}

/*!
    \brief     查询网络附着状态
    \param[in]  none
    \param[out] none
    \retval     none
*/
u8 Query_Network_Attach_Status(void)
{
    static u8 state = 0;
    char *p = NULL;
    switch (state)
    {
    case 0:
        querynum++;
        NB_ClearRxBuf();
        Uart1_SendData("AT+CGATT?\r\n", 11); // 关闭回显
        NB_Start_TimeOutTimer(100);
        state = 1;
        return 0;
    case 1:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"+CGATT: 1");
            if ((p != NULL))
            {
                state = 0;
                querynum = 0;
                return 1;
            }
            else
            {
                if (querynum <= 90)
                {
                    state = 0;
                    return 0;
                }
                else
                {
                    querynum = 0;
                    state = 0;
                    return 2;
                }
            }
        }
        return 0;
    default:
        return 0;
    }
}

/*!
    \brief     场景配置
    \param[in]  none
    \param[out] none
    \retval     none
*/
u8 SetPDP_Context(void)
{
    static u8 state = 0;
    char *p = NULL;
    switch (state)
    {
    case 0:
        querynum++;
        NB_ClearRxBuf();
        Uart1_SendData("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1\r\n", 31); // 关闭回显
        NB_Start_TimeOutTimer(100);
        state = 1;
        return 0;
    case 1:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"OK");
            if (p != NULL)
            {
                state = 0;
                return 1;
            }
            else
            {
                if (querynum <= 10)
                {
                    state = 0;
                    return 0;
                }
                else
                {
                    querynum = 0;
                    state = 0;
                    return 2;
                }
            }
        }
        return 0;
    default:
        return 0;
    }
}

/*!
    \brief     激活场景配置
    \param[in]  none
    \param[out] none
    \retval     none
*/
u8 Activate_PDP_Context(void)
{
    static u8 state = 0;
    char *p = NULL;
    switch (state)
    {
    case 0:
        querynum++;
        NB_ClearRxBuf();
        Uart1_SendData("AT+QIACT=1\r\n", 12); // 关闭回显
        NB_Start_TimeOutTimer(100);
        state = 1;
        return 0;
    case 1:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"OK");
            if (p != NULL)
            {
                state = 0;
                return 1;
            }
            else
            {
                if (querynum <= 10)
                {
                    state = 0;
                    return 0;
                }
                else
                {
                    querynum = 0;
                    state = 0;
                    return 2;
                }
            }
        }
        return 0;
    default:
        return 0;
    }
}

u8 getcclknum = 0;

u8 LTE_CCLK(void)
{
    static u8 state = 0;
    char *p = NULL;
    char *p1 = NULL;
    (void)(*p1);
    char *t;
    u8 h, m, s;
    (void)h;
    (void)m;
    (void)s;
    switch (state)
    {
    case 0:
        getcclknum++;
        NB_ClearRxBuf();
        Uart1_SendData("AT+CCLK?\r\n", 10); // 关闭回显
        NB_Start_TimeOutTimer(200);
        state = 1;
        return 0;
    case 1:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"+CCLK:");
            if (p != NULL)
            {
                p1 = strchr((const char *)g_NBbuf, ',');
                if (p1 != NULL)
                {
                    getcclknum = 0;
                    p1 += 1;
                    t = p1 + 1;
                    h = Ascii2Dec(p1) * 10 + Ascii2Dec(t);
                    if (h < 16)
                        h += 8;
                    else
                        h = h + 8 - 24;
                    p1 += 3;
                    t = p1 + 1;
                    m = Ascii2Dec(p1) * 10 + Ascii2Dec(t);
                    p += 3;
                    t = p + 1;
                    s = Ascii2Dec(p1) * 10 + Ascii2Dec(t);
                    Rtc_Timeset(h, m, s);
#if 1
                    printf("%d:%d:%d\r\n", h, m, s);
#endif
                    state = 0;
                    return 1;
                }
                else
                {
                    if (getcclknum == 10)
                    {
                        getcclknum = 0;
                        state = 0;
                        return 2;
                    }
                    else
                    {
                        state = 0;
                        return 0;
                    }
                }
            }
            else
            {
                if (getcclknum == 10)
                {
                    getcclknum = 0;
                    state = 0;
                    return 2;
                }
                else
                {
                    state = 0;
                    return 0;
                }
            }
        }
        return 0;
    default:
        return 0;
    }
}

/*!
    \brief     创建Tcp
    \param[in]  none
    \param[out] none
    \retval     none
*/
u8 senddatanum = 0;

u8 MQTT_Creat(void)
{
    static u8 state = 0;
    char *p = NULL;
    char *p1 = NULL;
    char stringlen[40];
    switch (state)
    {
    case 0:
        NB_ClearRxBuf();
        Uart1_SendData("AT+MQTTCFG=\"cached\",0,1\r\n", 25);
        NB_Start_TimeOutTimer(100);
        state = 1;
        return 0;
    case 1:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"OK");
            if (p != NULL)
            {
                state = 2;
                return 0;
            }
            else
            {
                state = 0;
                return 2;
            }
        }
        return 0;
    case 2:
        NB_ClearRxBuf();
        Uart1_SendData("AT+MQTTCFG=\"keepalive\",0,120\r\n", 30);
        NB_Start_TimeOutTimer(100);
        state = 3;
        return 0;
    case 3:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"OK");
            if (p != NULL)
            {
                state = 4;
                return 0;
            }
            else
            {
                state = 0;
                return 2;
            }
        }
        return 0;
    case 4:
        NB_ClearRxBuf();
        Uart1_SendData("AT+MQTTCONN=0,\"", 15);
        // sprintf(stringlen, "%d.%d.%d.%d\",", 8, 134, 155, 133);
        // sprintf(stringlen, "%d.%d.%d.%d\",", 39, 98, 162, 116);
        sprintf(stringlen, "%d.%d.%d.%d\",", 49, 234, 184, 198);
        Uart1_SendData((u8 *)stringlen, strlen(stringlen));
        sprintf(stringlen, "%d", 11080);
        Uart1_SendData((u8 *)stringlen, strlen(stringlen));
        Uart1_SendData(",\"", 2);
        Uart1_SendData(g_Equipment_number, 16);
        Uart1_SendData("\",", 2);
        Uart1_SendData("\"user\",\"0.123456\"\r\n", 19);
        NB_Start_TimeOutTimer(2000);
        state = 5;
        return 0;
    case 5:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"+MQTTURC: \"conn\",0,0");
            if (p != NULL)
            {
                state = 6;
                return 0;
            }
            else
            {
                state = 0;
                return 2;
            }
        }
        return 0;
    case 6:
        NB_ClearRxBuf();
        Uart1_SendData("AT+MQTTSUB=0,\"modbus_control/", 29);
        sprintf(stringlen, "%s", g_Equipment_number);
        Uart1_SendData((u8 *)stringlen, strlen(stringlen));
        Uart1_SendData("\",0\r\n", 5);
        NB_Start_TimeOutTimer(5000);
        state = 7;
        return 0;
    case 7:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"OK");
            p1 = strstr((const char *)g_NBbuf, (const char *)"+MQTTSUB: 0");
            if ((p != NULL) && (p1 != NULL))
            {
                state = 8;
                return 0;
            }
            else
            {
                state = 0;
                return 2;
            }
        }
        return 0;
    case 8:
        NB_ClearRxBuf();
        Uart1_SendData("AT+MQTTSUB=0,\"modbus_time/", 26);
        sprintf(stringlen, "%s", g_Equipment_number);
        Uart1_SendData((u8 *)stringlen, strlen(stringlen));
        Uart1_SendData("\",0\r\n", 5);
        NB_Start_TimeOutTimer(5000);
        state = 9;
        return 0;
    case 9:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"OK");
            p1 = strstr((const char *)g_NBbuf, (const char *)"+MQTTSUB: 0");
            if ((p != NULL) && (p1 != NULL))
            {
                state = 0;
                return 1;
            }
            else
            {
                state = 0;
                return 2;
            }
        }
        return 0;
    default:
        return 0;
    }
}

// LTE模块接收数据，并判断数据类型
void LTE_ReceiveSendData(void)
{
    static u8 state = 0;
    char *p = NULL;
    char *p1 = NULL;
    char *p2 = NULL;
    char *p3 = NULL;
    char *q = NULL;
    unsigned int length = 0;
    u8 rec[1500];
    char table[20];
    char stringlen[40];
    u16 i = 0;
    switch (state)
    {
    case 0:
        if (m_senddataflag == 1)
        {
            m_senddataflag = 0;
            state = 4; // 去发送数据
        }
        else
        {
            state = 1;
        }
        break;
    case 1:
        NB_Start_TimeOutTimer(100);
        state = 2;
        break;
    case 2:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            NB_ClearRxBuf();
            Uart1_SendData("AT+MQTTREAD=0,1\r\n", 17);
            NB_Start_TimeOutTimer(100);
            state = 3;
        }
        break;
    case 3:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p1 = strstr((const char *)g_NBbuf, (const char *)"+MQTTREAD:");
            if (p1 != NULL)
            {
                p2 = strstr((const char *)g_NBbuf, (const char *)"+MQTTREAD: 0,0,");
                p2 = p2 + 16;
                q = p2;
                while (*p2 != 0x2c)
                {
                    p2++;
                }
                length = p2 - q - 1;

                memset(rec, 0, 200);
                memcpy(rec, q, length);
                // printf("topic:\r\n");
                // printf("length == %d\r\n", length);
                // UART5Send(rec, length);
                // printf("\r\n");
                q = q + length;
                p3 = strstr((const char *)q, (const char *)"\",");
                {
                    // printf("&&&&\r\n");
                    // printf("%s\r\n", p3);
                    if (p3 != NULL)
                    {
                        p3 = p3 + 2;

                        while (*p3 != ',')
                        {
                            table[i] = *p3;
                            i++;
                            p3++;
                        }
                        table[i] = '\0';
                        // printf("%s\r\n", table);
                        length = atoi(table);
                        memset(rec, 0, 200);
                        memcpy(rec, p3 + 1, length);
                        // printf("length == %d\r\n", length);
                        // UART5Send(rec, length);
                        // printf("\r\n");
                        // //  MqttrecedataHandle(rec, length, flag);

                        if (length == 14)
                        {
                            /*RTC时间校准*/
                            MqttrecedataHandle(rec, length, 0);
                        }
                        if (length > 14)
                        {
                            /*下发485控制命令*/
                            MqttrecedataHandle(rec, length, 1);
                        }
                        state = 0;
                    }
                }
            }
            state = 0;
        }
        break;
    case 4:
        NB_Start_TimeOutTimer(100);
        state = 5;
        break;
    case 5: // 先获取最新的4G模块信号强度CSQ
        if (NB_Check_TimeOutTimer())
        { // 超时时间到
            NB_ClearRxBuf();
            Uart1_SendData("AT+CSQ\r\n", 8); // 关闭回显
            NB_Start_TimeOutTimer(100);
            state = 6;
        }
        break;
    case 6:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p1 = strstr((const char *)g_NBbuf, (const char *)"+CSQ: "); // 本地网络已经注册上
            if (p1 != NULL)
            {
                p1 += 6;
                while (*p1 != ',')
                {
                    length++;
                    p1++;
                }
                p1 = p1 - length;
                u8 m_csq = 0;
                char csq[4];
                if (length == 1)
                {

                    m_csq = Ascii2Dec(p1);
                    m_csq = 113 - (m_csq << 1);
                    sprintf(csq, "%d", m_csq);
                    memcpy(LTE.csq, csq, 1);
                    LTE.csq[1] = '\0';
                }
                if (length == 2)
                {
                    m_csq = Ascii2Dec(p1) * 10 + Ascii2Dec(p1 + 1);
                    m_csq = 113 - (m_csq << 1);
                    sprintf(csq, "%d", m_csq);
                    memcpy(LTE.csq, csq, 2);
                    LTE.csq[2] = '\0';
                }
                // printf("csq == %s\r\n", LTE.csq);
            }
            state = 7;
        }
        break;
    case 7:
        Uart1_SendData("AT+MQTTPUB=0,\"", 14);
        Uart1_SendData(m_TopicTxBuf, m_TopicTxCnt);
        Uart1_SendData("\",", 2);
        Uart1_SendData("0,0,0,", 6);
        sprintf(stringlen, "%d", m_DataTxCnt);
        Uart1_SendData((u8 *)stringlen, strlen(stringlen));
        Uart1_SendData(",\"", 2);
        Uart1_SendData(m_DataTxBuf, m_DataTxCnt);
        Uart1_SendData("\"\r\n", 3);
        // UART5Send(m_DataTxBuf, m_DataTxCnt);
        m_senddataflag = 0;
        NB_Start_TimeOutTimer(200);
        state = 8;
        break;
    case 8:
        if (NB_Check_TimeOutTimer()) // 超时时间到
        {
            Fiforead(g_NBbuf, Queue_len(MyQueue), MyQueue);
            p = strstr((const char *)g_NBbuf, (const char *)"OK");
            if (p != NULL)
            {
                // g_mqtt_errnum = 0;
                // g_mqtt_errnumsum = 0;
                state = 0;
            }
            else
            {
                LTE_REBOOT();
                state = 0;
                // m_senddataflag = 0;
            }
        }
        break;

    default:
        state = 0;
        break;
    }
}

void LTE_process(void)
{
    static LTESTATES process_state = REBOOT;
    switch (process_state)
    {
    case REBOOT: // 模组重启
        switch (LTE_Reset())
        {
        case 0:
            break;
        case 1:
            process_state = Query_Sim_Card_Status;
            break;
        case 2:
            process_state = REBOOT;
            break;
        }
        break;
    case Query_Sim_Card_Status: // 查询SIM卡状态
        switch (Query_Sim())
        {
        case 0:
            break;
        case 1:
            process_state = Network_Reg_Status;
            break;
        case 2:
            process_state = REBOOT;
            break;
        }
        break;
    case Network_Reg_Status: // 查询网络注册情况
        switch (Query_Network_Reg_Status())
        {
        case 0:
            break;
        case 1:
            process_state = ATTACH;
            break;
        case 2:
            process_state = REBOOT;
            break;
        }
        break;
    case ATTACH: // 查看网络附着
        switch (Query_Network_Attach_Status())
        {
        case 0:
            break;
        case 1:
            // process_state = Creat_MQTT;
            process_state = CCLK;
            break;
        case 2:
            process_state = REBOOT;
            break;
        }
        break;
    case CCLK:
        switch (LTE_CCLK())
        {
        case 0:
            break;
        case 1:
            // process_state = PDP_Context;
            process_state = Creat_MQTT;
            break;
        case 2:
            process_state = REBOOT;
            break;
        }
        break;
    case PDP_Context: // 场景配置
        switch (SetPDP_Context())
        {
        case 0:
            break;
        case 1:
            process_state = Activate_PDP;
            break;
        case 2:
            process_state = REBOOT;
            break;
        }
        break;
    case Activate_PDP: // 场景激活
        switch (Activate_PDP_Context())
        {
        case 0:
            break;
        case 1:
            process_state = Creat_MQTT;
            break;
        case 2:
            process_state = REBOOT;
            break;
        }
        break;
    case Creat_MQTT: // 连接MQTT
        switch (MQTT_Creat())
        {
        case 0:
            break;
        case 1:
            process_state = NBSTANDBY;
            m_mqttlinkstate = 1;
            HeartInfoLimit();
            HeartInfoLimit();
            break;
        case 2:
            process_state = REBOOT;
            break;
        }
        break;
    case NBSTANDBY: // 进入待机模式
        if (m_NeedReboot == 1)
        {
            m_NeedReboot = 0;
            process_state = REBOOT;
        }
        LTE_ReceiveSendData();
        break;
    default:
        break;
    }
}
