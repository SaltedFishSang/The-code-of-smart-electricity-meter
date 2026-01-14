#include "BSP.h"
u8 displayflag = 0;

void disflag(void)
{
	displayflag = 1;
	Vtimer_SetTimer(VTIM17, 1000, disflag);
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

		OLED_ShowChinese(0, 0, 0, 16);	// 电
		OLED_ShowChinese(16, 0, 1, 16); // 压
		OLED_ShowString(32, 0, ": ", 16);
		OLED_ShowNum(40, 0, BL0942_EleData.vol, 3, 16);
		OLED_ShowString(72, 0, "V", 16);

		OLED_ShowChinese(0, 2, 0, 16);	// 电
		OLED_ShowChinese(16, 2, 2, 16); // 流
		OLED_ShowString(32, 2, ": ", 16);
		OLED_ShowNum(40, 2, BL0942_EleData.current * 0.001, 2, 16);
		OLED_ShowString(64, 2, "A", 16);

		OLED_ShowChinese(0, 4, 3, 16);	// 功
		OLED_ShowChinese(16, 4, 4, 16); // 率
		OLED_ShowString(32, 4, ": ", 16);
		OLED_ShowNum(40, 4, BL0942_EleData.power, 5, 16);
		OLED_ShowString(88, 4, "W", 16);

		OLED_ShowChinese(0, 6, 7, 16);	// 输出
		OLED_ShowChinese(16, 6, 8, 16); //
		OLED_ShowString(32, 6, ": ", 16);
		if (Modbus_ReadDI(1))
		{
			OLED_ShowChinese(40, 6, 11, 16); // 打
			OLED_ShowChinese(56, 6, 9, 16);	 // 开
		}
		else
		{
			OLED_ShowChinese(40, 6, 10, 16); // 关
			OLED_ShowChinese(56, 6, 12, 16); // 闭
		}

		if (Modbus_ReadDI(2) == 1) // 锁扣
		{
			OLED_ShowChinese(96, 6, 13, 16);  // 锁
			OLED_ShowChinese(112, 6, 14, 16); // 扣
		}
		else
		{
			OLED_ShowString(96, 6, "    ", 16);
		}

		if (get_mqttlink())
		{
			OLED_ShowChinese(112, 0, 15, 16); // 在
			OLED_ShowChinese(112, 2, 17, 16); // 线
		}
		else
		{
			OLED_ShowChinese(112, 0, 16, 16); // 离
			OLED_ShowChinese(112, 2, 17, 16); // 线
		}
		Vtimer_SetTimer(VTIM17, 1000, disflag);
	}
}
