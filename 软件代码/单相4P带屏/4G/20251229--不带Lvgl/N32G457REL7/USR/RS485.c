#include "BSP.h"
/*!
    \brief     RS485Init RS485≥ı ºªØ
    \param[in]  none
    \param[out] none
    \retval     none
*/
void RS485Init(void)
{
    init_usart3();
    /*RSDIR  PA11*/
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
    RS485_DIR_RX;
}
/*!
    \brief     RS485Task RS485 Task
    \param[in]  none
    \param[out] none
    \retval     none
*/
void RS485Task(void)
{
    Uart3Task();
}
