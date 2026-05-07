#include "BSP.h"
/*!
    \brief    u8  getkeybuttonvalue(void) 获取按键值
    \param[in]  none
    \param[out] u8 keypress
    \retval     none
*/
u8 keybuttonvalue = 0;
u8 getkeybuttonvalue(void)
{
    return keybuttonvalue;
}
/*!
    \brief    void   keybuttonvalueset(void) 输出开关按键置位
    \param[in]  none
    \param[out] u8 keypress
    \retval     none
*/
void keybuttonvalueset(void)
{
    keybuttonvalue = 1;
}
/*!
    \brief    void keybuttonvaluereset(void) 输出开关按键复位
    \param[in]  none
    \param[out] u8 keypress
    \retval     none
*/
void keybuttonvaluereset(void)
{
    keybuttonvalue = 0;
}
/*!
    \brief     keygpioInit 按键IO初始化
    \param[in]  none
    \param[out] none
    \retval     none
*/
void keygpioInit(void)
{
    /*keyswitch  PB2*/
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
    /*Configure key EXTI Line to key input Pin*/
    GPIO_ConfigEXTILine(GPIOB_PORT_SOURCE, GPIO_PIN_SOURCE2);

    /*Configure key EXTI line*/
    EXTI_InitStructure.EXTI_Line = EXTI_LINE2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

    /*button1 PA4*/
    GPIO_InitStructure.Pin = GPIO_PIN_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    /*button2 PA6*/
    GPIO_InitStructure.Pin = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    /*button3 PC5*/
    GPIO_InitStructure.Pin = GPIO_PIN_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);

    /*button4 PB1*/
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
}
/*!
    \brief     keybuttontask 输出开关按键检测
    \param[in]  none
    \param[out] none
    \retval     none
*/
void keybuttontask(void)
{
    if (getkeybuttonvalue() == 1)
    {
        keybuttonvaluereset();     // 按键按下处理程序
        if (Modbus_ReadDI(2) == 0) // 没有远程锁扣的情况下 才可以一??分合??
        {
            Modbus_WirteDI(4, 0);
            if (Modbus_ReadDI(1) == 1)
            {
                Relay_Control(0);
                DisplayOutputState(0);
                Modbus_WirteDI(1, 0);      // 分闸
                Modbus_WrieteInput(10, 9); // 再写入输入寄存器 最后一次分闸原因 本地控制
                Eprom_WirteModbusDI(1, 0);
                ModbusSendAll();
            }
            else
            {
                Relay_Control(1);
                DisplayOutputState(1);
                Modbus_WirteDI(1, 1); // 合闸
                Eprom_WirteModbusDI(1, 1);
                ModbusSendAll();
            }
        }
    }
}

/*!
    \brief     keytask 按键任务检测
    \param[in]  none
    \param[out] none
    \retval     none
*/
void keytask(void)
{
    keybuttontask();
}
