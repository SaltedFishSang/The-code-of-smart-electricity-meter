#include "BSP.h"
/*!
    \brief     keygpioInit 5个按键gpio口初始化
    \param[in]  none
    \param[out] none
    \retval     none
*/
void keygpioInit(void)
{
    /*keyswitch  PB2*/
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
}
