#include "BSP.h"
#define Relay_A_ON GPIO_SetBits(GPIOB, GPIO_PIN_2)
#define Relay_A_OFF GPIO_ResetBits(GPIOB, GPIO_PIN_2)

#define Relay_B_ON GPIO_SetBits(GPIOB, GPIO_PIN_1)
#define Relay_B_OFF GPIO_ResetBits(GPIOB, GPIO_PIN_1)

#define RelayLEDOn GPIO_ResetBits(GPIOA, GPIO_PIN_3)
#define RelayLEDOff GPIO_SetBits(GPIOA, GPIO_PIN_3)
/**
 * @brief  Inserts a delay time.
 * @param count specifies the delay time length.
 */
void Delay(uint32_t count)
{
    int i = 0;
    for (; count > 0; count--)
    {
        i++;
    }
}

/*!
    \brief      RelayLedInit(void) 继电器指示灯 GPIO口初始化
    \param[in]  None
    \param[out] none
    \retval     none
*/
void RelayLedInit(void)
{
    /*
     *PA3----RelayLed
     */
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
    RelayLEDOff;
}
/*!
    \brief      RelayInit(void) 磁保持继电器 GPIO口初始化
    \param[in]  None
    \param[out] none
    \retval     none
*/
void RelayInit(void)
{
    /*
     *PB2-----Control_A
     *PB1-----Control_B
     */
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_2 | GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
    Relay_A_OFF;
    Relay_B_OFF;
    RelayLedInit();
}
/*!
    \brief      RelayControl(u8 onoff)继电器控制函数
    \param[in]  onoff 0:1 ----> 关闭:打开
    \param[out] none
    \retval     none
*/
void RelayControl(u8 onoff)
{
    switch (onoff)
    {
    case 0:
        Relay_A_ON;
        Relay_B_OFF;
        Delay_MS(20);
        Relay_A_OFF;
        Relay_B_OFF;
        RelayLEDOff;
        break;
    case 1:
        Relay_B_ON;
        Relay_A_OFF;
        Delay_MS(20);
        Relay_A_OFF;
        Relay_B_OFF;
        RelayLEDOn;
        break;
    default:
        Relay_A_OFF;
        Relay_B_OFF;
        break;
    }
}
/*!
    \brief      GateStatus(void)获取继电器状态
    \param[in]  none
    \param[out] 0:1 ---> 关闭:打开
    \retval     none
*/
bool GateStatus(void)
{
    if (Modbus_ReadDI(1))
        return 1;
    else
        return 0;
}
