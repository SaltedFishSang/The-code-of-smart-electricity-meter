#include "BSP.h"
u8 displayflag = 0;

void disflag(void)
{
	displayflag = 1;
	Vtimer_SetTimer(VTIM17, 2000, disflag);
}
/*!
	\brief      OLEDDisplay(void)
	\param[in]  None
	\param[out] none
	\retval     none
*/
void OLEDDisplay(void)
{
	static u8 num = 0;
	if (displayflag == 1)
	{
		displayflag = 0;
		num++;
		
		OLED_ShowChinese(0,0,0,16);//电
		OLED_ShowChinese(16,0,1,16);//压
		OLED_ShowString(32, 0, ": ", 16);
		OLED_ShowNum(48, 0, 220, 3, 16);
		OLED_ShowString(80, 0, "V", 16);

		OLED_ShowChinese(0,2,0,16);//电
		OLED_ShowChinese(16,2,2,16);//流
		OLED_ShowString(32, 2, ": ", 16);
		OLED_ShowNum(48, 2, 80, 2, 16);
		OLED_ShowString(80, 2, "A", 16);
		
//	  OLED_ShowChinese(0,4,3,16);//功
//		OLED_ShowChinese(16,4,4,16);//率
//		OLED_ShowString(32, 4, ": ", 16);
//		OLED_ShowNum(48, 4, 17600, 5, 16);
//		OLED_ShowString(80, 4, "W", 16);

		Vtimer_SetTimer(VTIM17, 2000, disflag);
	}
}
