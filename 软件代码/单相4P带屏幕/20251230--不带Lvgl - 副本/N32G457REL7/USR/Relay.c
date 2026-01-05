#include "BSP.h"
#define Relay_A_ON GPIO_SetBits(GPIOB, GPIO_PIN_15)
#define Relay_B_ON GPIO_SetBits(GPIOB, GPIO_PIN_14)
#define Relay_A_OFF GPIO_ResetBits(GPIOB, GPIO_PIN_15)
#define Relay_B_OFF GPIO_ResetBits(GPIOB, GPIO_PIN_14)
/*!
    \brief     RelaygpioInit 磁保持继电器GPIO口初始化
    \param[in]  none
    \param[out] none
    \retval     none
*/
void RelaygpioInit(void)
{
    /*Relay Conrtrol A---->PB15     B---->PB14*/
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_PIN_15);

    GPIO_InitStructure.Pin = GPIO_PIN_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_PIN_14);
}
/*!
    \brief     RelaygpioInit 磁保持继电器GPIO口初始化
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Relay_Control(u8 onoff)
{
    switch (onoff)
    {
    case 0:
        GPIO_SetBits(GPIOB, GPIO_PIN_15);
        GPIO_ResetBits(GPIOB, GPIO_PIN_14);
        delay_ms(20);
        GPIO_ResetBits(GPIOB, GPIO_PIN_15);
        GPIO_ResetBits(GPIOB, GPIO_PIN_14);
        break;
    case 1:
        GPIO_SetBits(GPIOB, GPIO_PIN_14);
        GPIO_ResetBits(GPIOB, GPIO_PIN_15);
        delay_ms(20);
        GPIO_ResetBits(GPIOB, GPIO_PIN_15);
        GPIO_ResetBits(GPIOB, GPIO_PIN_14);
        break;
    default:
        GPIO_ResetBits(GPIOB, GPIO_PIN_15);
        GPIO_ResetBits(GPIOB, GPIO_PIN_14);
        break;
    }
}
