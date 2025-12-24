#include "BSP.h"
u8 displayflag = 0;

void disflag(void)
{
    displayflag = 1;
    Vtimer_SetTimer(VTIM17, 10000, disflag);
}
/*!
    \brief      OLEDDisplay(void)
    \param[in]  None
    \param[out] none
    \retval     none
*/
void OLEDDisplay(void)
{
    if (displayflag == 1)
    {
        displayflag = 0;
		//OLED_Clear();
		OLED_ShowChinese(0,0,0,16);//中
		OLED_ShowChinese(18,0,1,16);//景
		OLED_ShowChinese(36,0,2,16);//园
		OLED_ShowChinese(54,0,3,16);//电
		OLED_ShowChinese(72,0,4,16);//子
		OLED_ShowChinese(90,0,5,16);//科
		OLED_ShowChinese(108,0,6,16);//技
		OLED_ShowString(8,2,"ZHONGJINGYUAN",16);
		OLED_ShowString(20,4,"2014/05/01",16);
		OLED_ShowString(0,6,"ASCII:",16);  
		OLED_ShowString(63,6,"CODE:",16);
        Vtimer_SetTimer(VTIM17, 10000, disflag);
    }
}
