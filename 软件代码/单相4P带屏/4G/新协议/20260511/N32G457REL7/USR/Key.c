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
        keybuttonvaluereset();      // 按键按下处理程序
        if (gettftblkstatus() == 1) // 亮屏情况下，处理按键功能
        {
            if (Switch_Status.SwitchLockvalue == 0) // 没有远程锁本地按键的情况下 才可以一键分合闸
            {
                DisplayClockState(0);                 // 退出定时模式
                if (Switch_Status.SwitchOnvalue == 1) // 在合闸情况下，分闸
                {
                    Relay_Control(0);
                    DisplayOutputState(0);
                    Switch_Status.SwitchOnvalue = 0;
                    EEP_Write8(SwitchOn, Switch_Status.SwitchOnvalue);
                    HeartInfoLimit();
                }
                else // 合闸
                {
                    Relay_Control(1);
                    DisplayOutputState(1);
                    Switch_Status.SwitchOnvalue = 1;
                    EEP_Write8(SwitchOn, Switch_Status.SwitchOnvalue);
                    HeartInfoLimit();
                }
            }
        }
        else
        {
            TFTBLK_EN(); // 开启屏幕背光
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
