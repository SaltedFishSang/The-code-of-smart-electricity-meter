#include "BSP.h"
#define BLUETOOTH_ENABLE GPIO_ResetBits(GPIOB, GPIO_PIN_3) // 使能蓝牙模块
#define BLUETOOTH_DISABLE GPIO_SetBits(GPIOB, GPIO_PIN_3)  // 禁止蓝牙模块
typedef enum
{
    PowerON,            // 打开蓝牙模块电源
    PowerONWait,        // 蓝牙开机等待
    Config_Name,        // 配置蓝牙模块名称
    Config_Network,     // 获取wifi配网信息
    PowerOFF,           // 关闭蓝牙模块电源
    Waitreceive,        // 等待接收数据
    In_at_module,       // 进入AT模式
    In_at_module_check, // 进入AT模式检查
    In_at_module_ok,    // 进入AT模式成功
    In_at_module_fail,  // 进入AT模式失败
} _BluetoothState;
_BluetoothState BluetoothState = PowerON;
static u8 m_TimeOut = 0; // 蓝牙配网超时标志
/*!
    \brief     Bluetoot_Set_TimeOut 蓝牙配网超时设置
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Bluetoot_Set_TimeOut(void)
{
    m_TimeOut = 1;
}
/*!
    \brief     Bluetoot_Check_TimeOutTimer 蓝牙配网超时检查
    \param[in]  none
    \param[out] none
    \retval     none
*/
u8 Bluetoot_Check_TimeOutTimer(void)
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
/*!
    \brief     Bluetoot_Start_TimeOutTimer 蓝牙配网超时定时器启动
    \param[in]  t:超时时间，单位ms
    \param[out] none
    \retval     none
*/
void Bluetoot_Start_TimeOutTimer(u16 t)
{
    m_TimeOut = 0;
    Vtimer_SetTimer(VTIM15, t, Bluetoot_Set_TimeOut); // 超时判断为8s
}
/*!
    \brief     Bluetoot_ReSet_TimeOutTimer 蓝牙配网超时定时器重置
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Bluetoot_ReSet_TimeOutTimer(void)
{
    m_TimeOut = 0;
    Vtimer_KillTimer(VTIM15);
}
/*!
    \brief     BlueTooth_Init  蓝牙初始化（1、蓝牙使能GPIO口初始化；2、跟蓝牙通讯模块串口初始化）
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BlueTooth_Init(void)
{
    /*蓝牙使能GPIO口初始化 PB3*/
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
    BLUETOOTH_DISABLE; // 默认禁止蓝牙模块
}
/*!
    \brief     BlueTooth_Task 蓝牙任务
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BlueTooth_Task(void)
{
    char *p = NULL;
    static u8 num = 0;
    switch (BluetoothState)
    {
    case PowerON:                          // 蓝牙模式使能
        BLUETOOTH_ENABLE;                  // 打开蓝牙模块电源
        BluetoothState = PowerONWait;      // 蓝牙模块开机等待2s
        Bluetoot_Start_TimeOutTimer(2000); // 创建2s的蓝牙开机等待定时器，等待蓝牙模块开机完成
        break;
    case PowerONWait:                      // 蓝牙开机等待
        if (Bluetoot_Check_TimeOutTimer()) // 2s开机等待时间到
        {
            Bluetoot_ReSet_TimeOutTimer();
            Uart5ClearRxBuf();
            BluetoothState = In_at_module; // 配置蓝牙模块进入AT模式
        }
        break;
    case In_at_module: // 进入AT模式
        Uart5ClearRxBuf();
        Uart5Send("AT...\r\n", 7);           // 发送AT命令，进入AT模式
        BluetoothState = In_at_module_check; // 进入AT模式检查，确认蓝牙模块是否进入AT模式
        Bluetoot_Start_TimeOutTimer(1000);   // 创建1s的蓝牙进入AT模式检查定时器，等待蓝牙模块进入AT模式
        num++;
        break;
    case In_at_module_check:               // 进入AT模式检查
        if (Bluetoot_Check_TimeOutTimer()) // 1s时间到
        {
            Bluetoot_ReSet_TimeOutTimer();
            p = strstr((const char *)Uart5.RxBuf, (const char *)"OK"); // 检查蓝牙模块是否返回OK，确认进入AT模式成功
            if (p != NULL)
            {
                BluetoothState = In_at_module_ok; // 回复OK，进入AT模式成功
            }
            else
            {
                if (num == 10) // 进入AT模式检查失败10次，认为进入AT模式失败，关机重启蓝牙模块重新尝试进入AT模式
                {
                    num = 0;
                    BluetoothState = In_at_module_fail;
                    BLUETOOTH_DISABLE;                 // 禁止蓝牙模块
                    Bluetoot_Start_TimeOutTimer(1000); // 进入AT模式失败，蓝牙模块先关机，1s后重新尝试进入AT模式
                }
                else
                {
                    BluetoothState = In_at_module; // 回复OK失败，继续尝试进入AT模式
                }
            }
        }
        break;
    case In_at_module_ok: // 进入AT模式成功 配置蓝牙模块名称
        Uart5ClearRxBuf();
        Uart5Send("AT+NAME=", 8); // 发送配置蓝牙名称命令
        Uart5Send(g_Equipment_number, 16);
        Uart5Send("\r\n", 2);
        Bluetoot_Start_TimeOutTimer(1000); // 创建1s的蓝牙配置名称检查定时器，等待蓝牙模块返回配置结果
        BluetoothState = Config_Name;
        num++;
        break;
    case Config_Name:
        if (Bluetoot_Check_TimeOutTimer()) // 等待蓝牙模块返回配置结果，1s时间到
        {
            Bluetoot_ReSet_TimeOutTimer();
            p = strstr((const char *)Uart5.RxBuf, (const char *)"OK"); // 检查蓝牙模块是否返回OK，确认配置蓝牙名称成功
            if (p != NULL)
            {
                BluetoothState = Config_Network;
                num = 0;
            }
            else
            {
                if (num == 10) // 配置蓝牙名称失败10次，认为配置蓝牙名称失败，关机重启蓝牙模块重新尝试进入AT模式后 重新配置蓝牙名称
                {
                    num = 0;
                    BluetoothState = In_at_module_fail;
                    BLUETOOTH_DISABLE;                 // 禁止蓝牙模块
                    Bluetoot_Start_TimeOutTimer(1000); // 配置蓝牙名称失败，蓝牙模块先关机，1s后重新尝试进入AT模式
                }
                else
                {
                    BluetoothState = In_at_module_ok; // 重新配置蓝牙名称
                }
            }
        }
        break;
    case In_at_module_fail:                // 进入AT模式失败
        if (Bluetoot_Check_TimeOutTimer()) // 1s超时时间到
        {
            Bluetoot_ReSet_TimeOutTimer();
            BluetoothState = PowerON;
        }
        break;
    case Config_Network: // 配置蓝牙模块获取wifi配网信息
        break;
    }
}
