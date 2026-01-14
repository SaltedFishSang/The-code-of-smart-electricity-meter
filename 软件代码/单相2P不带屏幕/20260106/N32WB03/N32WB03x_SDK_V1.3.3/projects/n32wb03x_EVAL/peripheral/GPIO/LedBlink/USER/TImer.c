#include "BSP.h"

/*!
    \brief      TIM3Init(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void TIM3Init(void) {
    TIM_TimeBaseInitType TIM_TimeBaseInitTypeStruct;
    TIM_InitTimBaseStruct(&TIM_TimeBaseInitTypeStruct);
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM3, ENABLE);
    TIM_TimeBaseInitTypeStruct.ClkDiv = TIM_CLK_DIV1;
    TIM_TimeBaseInitTypeStruct.CntMode = TIM_CNT_MODE_UP;
    TIM_TimeBaseInitTypeStruct.Period = 1000;
    TIM_TimeBaseInitTypeStruct.Prescaler = 32 - 1;
    TIM_TimeBaseInitTypeStruct.RepetCnt = 0;
    TIM_InitTimeBase(TIM3, &TIM_TimeBaseInitTypeStruct);
    TIM_ConfigInt(TIM3, TIM_INT_UPDATE, ENABLE);
    TIM_Enable(TIM3, ENABLE);
}

void TIM3_IRQHandler(void) {
    if (SET == TIM_GetIntStatus(TIM3, TIM_INT_UPDATE)) {
        TIM_ClrIntPendingBit(TIM3, TIM_INT_UPDATE);
        Vtimer_UpdateHandler();
    }
}
