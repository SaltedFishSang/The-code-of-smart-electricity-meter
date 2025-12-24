#include "BSP.h"
#include "lcdfont.h"
/********************���ݳ�����**************************
LCD:T200H7-C14-05
IC: ST7789P3
***** PB6    SCK
	  PB7    SDA
	  PB9    RESET
	  PB8    D/C
	  PB5    CS
******************************************************/

void Delay(uint32_t count)
{
	for (; count > 0; count--)
		;
}
void delay_us(unsigned int _us_time)
{
	u32 i = 0;
	for (i = 0; i < _us_time; i++)
		Delay(25);
}

void delay_ms(unsigned int _ms_time)
{
	u32 i = 0;
	for (i = 0; i < _ms_time; i++)
		Delay(25000);
}
void TFT_IO_init(void)
{
	/*
	   PB4----LIGHT
	   PB5----CS
	   PB6----CLK
	   PB7----SDA
	   PB8----DC
	   PB9----RESET
	   */
	GPIO_InitType GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
	// ����JTAG��ʹ��SW��Ч��Ϊ����PB3Ϊ��ͨIO
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);
	GPIO_ConfigPinRemap(GPIO_RMP_SW_JTAG_SW_ENABLE, ENABLE);
}
void TFTLCD_Writ_Bus(u8 dat)
{
	u8 i;
	TFTLCD_CS_RESET;
	for (i = 0; i < 8; i++)
	{
		TFTLCD_SCL_RESET;
		if (dat & 0x80)
		{
			TFTLCD_SDA_SET;
		}
		else
		{
			TFTLCD_SDA_RESET;
		}
		TFTLCD_SCL_SET;
		dat <<= 1;
	}
	TFTLCD_CS_SET;
}

void TFTLCD_WR_DATA8(u8 dat)
{
	TFTLCD_Writ_Bus(dat);
}

void TFTLCD_WR_DATA(u16 dat)
{
	TFTLCD_Writ_Bus(dat >> 8);
	TFTLCD_Writ_Bus(dat);
}

void TFTLCD_WR_REG(u8 dat)
{
	TFTLCD_DC_RESET;
	TFTLCD_Writ_Bus(dat);
	TFTLCD_DC_SET;
}

void TFTLCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2)
{
	if (USE_HORIZONTAL == 0)
	{
		TFTLCD_WR_REG(0x2a);
		TFTLCD_WR_DATA(x1 + TFT_COLUMN_OFFSET);
		TFTLCD_WR_DATA(x2 + TFT_COLUMN_OFFSET);
		TFTLCD_WR_REG(0x2b);
		TFTLCD_WR_DATA(y1 + TFT_LINE_OFFSET);
		TFTLCD_WR_DATA(y2 + TFT_LINE_OFFSET);
		TFTLCD_WR_REG(0x2c);
	}
	else if (USE_HORIZONTAL == 1)
	{
		TFTLCD_WR_REG(0x2a);
		TFTLCD_WR_DATA(x1 + TFT_COLUMN_OFFSET);
		TFTLCD_WR_DATA(x2 + TFT_COLUMN_OFFSET);
		TFTLCD_WR_REG(0x2b);
		TFTLCD_WR_DATA(y1 + TFT_LINE_OFFSET);
		TFTLCD_WR_DATA(y2 + TFT_LINE_OFFSET);
		TFTLCD_WR_REG(0x2c);
	}
	else if (USE_HORIZONTAL == 2)
	{
		TFTLCD_WR_REG(0x2a);
		TFTLCD_WR_DATA(x1 + TFT_COLUMN_OFFSET);
		TFTLCD_WR_DATA(x2 + TFT_COLUMN_OFFSET);
		TFTLCD_WR_REG(0x2b);
		TFTLCD_WR_DATA(y1 + TFT_LINE_OFFSET);
		TFTLCD_WR_DATA(y2 + TFT_LINE_OFFSET);
		TFTLCD_WR_REG(0x2c);
	}
	else
	{
		TFTLCD_WR_REG(0x2a);
		TFTLCD_WR_DATA(x1 + TFT_COLUMN_OFFSET);
		TFTLCD_WR_DATA(x2 + TFT_COLUMN_OFFSET);
		TFTLCD_WR_REG(0x2b);
		TFTLCD_WR_DATA(y1 + TFT_LINE_OFFSET);
		TFTLCD_WR_DATA(y2 + TFT_LINE_OFFSET);
		TFTLCD_WR_REG(0x2c);
	}
}

void TFTLCD_Init(void)
{
	u8 i = 0;
	TFT_IO_init();

	TFTLCD_RST_RESET;
	delay_ms(100);
	TFTLCD_RST_SET;
	delay_ms(100);

	TFTLCD_BLK_SET;
	delay_ms(100);

	if (TFT == 180)
	{
		//************* Start Initial Sequence **********//
		TFTLCD_WR_REG(0x11); // Sleep out
		delay_ms(120);		 // Delay 120ms
		//------------------------------------ST7735S Frame Rate-----------------------------------------//
		TFTLCD_WR_REG(0xB1);
		TFTLCD_WR_DATA8(0x05);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_REG(0xB2);
		TFTLCD_WR_DATA8(0x05);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_REG(0xB3);
		TFTLCD_WR_DATA8(0x05);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_DATA8(0x05);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_DATA8(0x3C);
		//------------------------------------End ST7735S Frame Rate---------------------------------//
		TFTLCD_WR_REG(0xB4); // Dot inversion
		TFTLCD_WR_DATA8(0x03);
		//------------------------------------ST7735S Power Sequence---------------------------------//
		TFTLCD_WR_REG(0xC0);
		TFTLCD_WR_DATA8(0x28);
		TFTLCD_WR_DATA8(0x08);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_REG(0xC1);
		TFTLCD_WR_DATA8(0XC0);
		TFTLCD_WR_REG(0xC2);
		TFTLCD_WR_DATA8(0x0D);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_REG(0xC3);
		TFTLCD_WR_DATA8(0x8D);
		TFTLCD_WR_DATA8(0x2A);
		TFTLCD_WR_REG(0xC4);
		TFTLCD_WR_DATA8(0x8D);
		TFTLCD_WR_DATA8(0xEE);
		//---------------------------------End ST7735S Power Sequence-------------------------------------//
		TFTLCD_WR_REG(0xC5); // VCOM
		TFTLCD_WR_DATA8(0x1A);
		TFTLCD_WR_REG(0x36); // MX, MY, RGB mode
		if (USE_HORIZONTAL == 0)
			TFTLCD_WR_DATA8(0x00);
		else if (USE_HORIZONTAL == 1)
			TFTLCD_WR_DATA8(0xC0);
		else if (USE_HORIZONTAL == 2)
			TFTLCD_WR_DATA8(0x70);
		else
			TFTLCD_WR_DATA8(0xA0);
		//------------------------------------ST7735S Gamma Sequence---------------------------------//
		TFTLCD_WR_REG(0xE0);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x22);
		TFTLCD_WR_DATA8(0x07);
		TFTLCD_WR_DATA8(0x0A);
		TFTLCD_WR_DATA8(0x2E);
		TFTLCD_WR_DATA8(0x30);
		TFTLCD_WR_DATA8(0x25);
		TFTLCD_WR_DATA8(0x2A);
		TFTLCD_WR_DATA8(0x28);
		TFTLCD_WR_DATA8(0x26);
		TFTLCD_WR_DATA8(0x2E);
		TFTLCD_WR_DATA8(0x3A);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x01);
		TFTLCD_WR_DATA8(0x03);
		TFTLCD_WR_DATA8(0x13);
		TFTLCD_WR_REG(0xE1);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x16);
		TFTLCD_WR_DATA8(0x06);
		TFTLCD_WR_DATA8(0x0D);
		TFTLCD_WR_DATA8(0x2D);
		TFTLCD_WR_DATA8(0x26);
		TFTLCD_WR_DATA8(0x23);
		TFTLCD_WR_DATA8(0x27);
		TFTLCD_WR_DATA8(0x27);
		TFTLCD_WR_DATA8(0x25);
		TFTLCD_WR_DATA8(0x2D);
		TFTLCD_WR_DATA8(0x3B);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x01);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x13);
		//------------------------------------End ST7735S Gamma Sequence-----------------------------//
		TFTLCD_WR_REG(0x3A); // 65k mode
		TFTLCD_WR_DATA8(0x05);
		TFTLCD_WR_REG(0x29); // Display on
	}
	else if (TFT == 130)
	{
		//************* Start Initial Sequence **********//
		TFTLCD_WR_REG(0x36);
		if (USE_HORIZONTAL == 0)
			TFTLCD_WR_DATA8(0x00);
		else if (USE_HORIZONTAL == 1)
			TFTLCD_WR_DATA8(0x60);
		else if (USE_HORIZONTAL == 2)
			TFTLCD_WR_DATA8(0xa0);
		else
			TFTLCD_WR_DATA8(0xc0);

		TFTLCD_WR_REG(0x3A);
		TFTLCD_WR_DATA8(0x05);

		TFTLCD_WR_REG(0xB2);
		TFTLCD_WR_DATA8(0x0C);
		TFTLCD_WR_DATA8(0x0C);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x33);
		TFTLCD_WR_DATA8(0x33);

		TFTLCD_WR_REG(0xB7);
		TFTLCD_WR_DATA8(0x35);

		TFTLCD_WR_REG(0xBB);
		TFTLCD_WR_DATA8(0x19);

		TFTLCD_WR_REG(0xC0);
		TFTLCD_WR_DATA8(0x2C);

		TFTLCD_WR_REG(0xC2);
		TFTLCD_WR_DATA8(0x01);

		TFTLCD_WR_REG(0xC3);
		TFTLCD_WR_DATA8(0x12);

		TFTLCD_WR_REG(0xC4);
		TFTLCD_WR_DATA8(0x20);

		TFTLCD_WR_REG(0xC6);
		TFTLCD_WR_DATA8(0x0F);

		TFTLCD_WR_REG(0xD0);
		TFTLCD_WR_DATA8(0xA4);
		TFTLCD_WR_DATA8(0xA1);

		TFTLCD_WR_REG(0xE0);
		TFTLCD_WR_DATA8(0xD0);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x0D);
		TFTLCD_WR_DATA8(0x11);
		TFTLCD_WR_DATA8(0x13);
		TFTLCD_WR_DATA8(0x2B);
		TFTLCD_WR_DATA8(0x3F);
		TFTLCD_WR_DATA8(0x54);
		TFTLCD_WR_DATA8(0x4C);
		TFTLCD_WR_DATA8(0x18);
		TFTLCD_WR_DATA8(0x0D);
		TFTLCD_WR_DATA8(0x0B);
		TFTLCD_WR_DATA8(0x1F);
		TFTLCD_WR_DATA8(0x23);

		TFTLCD_WR_REG(0xE1);
		TFTLCD_WR_DATA8(0xD0);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x0C);
		TFTLCD_WR_DATA8(0x11);
		TFTLCD_WR_DATA8(0x13);
		TFTLCD_WR_DATA8(0x2C);
		TFTLCD_WR_DATA8(0x3F);
		TFTLCD_WR_DATA8(0x44);
		TFTLCD_WR_DATA8(0x51);
		TFTLCD_WR_DATA8(0x2F);
		TFTLCD_WR_DATA8(0x1F);
		TFTLCD_WR_DATA8(0x1F);
		TFTLCD_WR_DATA8(0x20);
		TFTLCD_WR_DATA8(0x23);

		TFTLCD_WR_REG(0x21);

		TFTLCD_WR_REG(0x11);
		// Delay (120);

		TFTLCD_WR_REG(0x29);

		// ?????
		TFTLCD_WR_REG(0x2A);   // ?????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0xEF); // ???????(240 ?)

		// ?????
		TFTLCD_WR_REG(0x2B);   // ?????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0xC5); // ???????(198 ?)

		TFTLCD_WR_REG(0x2C); // ????
	}
	else if (TFT == 2) // ST7789 200
	{
		TFTLCD_WR_REG(0x36);
		if (USE_HORIZONTAL == 0)
			TFTLCD_WR_DATA8(0x00);
		else if (USE_HORIZONTAL == 1)
			TFTLCD_WR_DATA8(0xc0);
		else if (USE_HORIZONTAL == 2)
			TFTLCD_WR_DATA8(0xa0);
		else
			TFTLCD_WR_DATA8(0x60);

		TFTLCD_WR_REG(0x3A);
		TFTLCD_WR_DATA8(0x05);

		TFTLCD_WR_REG(0xB2);
		TFTLCD_WR_DATA8(0x0C);
		TFTLCD_WR_DATA8(0x0C);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x33);
		TFTLCD_WR_DATA8(0x33);

		TFTLCD_WR_REG(0xB7);
		TFTLCD_WR_DATA8(0x35);

		TFTLCD_WR_REG(0xBB);
		TFTLCD_WR_DATA8(0x19);

		TFTLCD_WR_REG(0xC0);
		TFTLCD_WR_DATA8(0x2C);

		TFTLCD_WR_REG(0xC2);
		TFTLCD_WR_DATA8(0x01);

		TFTLCD_WR_REG(0xC3);
		TFTLCD_WR_DATA8(0x12);

		TFTLCD_WR_REG(0xC4);
		TFTLCD_WR_DATA8(0x20);

		TFTLCD_WR_REG(0xC6);
		TFTLCD_WR_DATA8(0x0F);

		TFTLCD_WR_REG(0xD0);
		TFTLCD_WR_DATA8(0xA4);
		TFTLCD_WR_DATA8(0xA1);

		TFTLCD_WR_REG(0xE0);
		TFTLCD_WR_DATA8(0xD0);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x0D);
		TFTLCD_WR_DATA8(0x11);
		TFTLCD_WR_DATA8(0x13);
		TFTLCD_WR_DATA8(0x2B);
		TFTLCD_WR_DATA8(0x3F);
		TFTLCD_WR_DATA8(0x54);
		TFTLCD_WR_DATA8(0x4C);
		TFTLCD_WR_DATA8(0x18);
		TFTLCD_WR_DATA8(0x0D);
		TFTLCD_WR_DATA8(0x0B);
		TFTLCD_WR_DATA8(0x1F);
		TFTLCD_WR_DATA8(0x23);

		TFTLCD_WR_REG(0xE1);
		TFTLCD_WR_DATA8(0xD0);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x0C);
		TFTLCD_WR_DATA8(0x11);
		TFTLCD_WR_DATA8(0x13);
		TFTLCD_WR_DATA8(0x2C);
		TFTLCD_WR_DATA8(0x3F);
		TFTLCD_WR_DATA8(0x44);
		TFTLCD_WR_DATA8(0x51);
		TFTLCD_WR_DATA8(0x2F);
		TFTLCD_WR_DATA8(0x1F);
		TFTLCD_WR_DATA8(0x1F);
		TFTLCD_WR_DATA8(0x20);
		TFTLCD_WR_DATA8(0x23);

		TFTLCD_WR_REG(0x21);

		TFTLCD_WR_REG(0x11);
		// Delay (120);

		TFTLCD_WR_REG(0x29);

		// ?????
		TFTLCD_WR_REG(0x2A);   // ?????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0xEF); // ???????(240 ?)

		// ?????
		TFTLCD_WR_REG(0x2B);   // ?????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0xC5); // ???????(198 ?)

		TFTLCD_WR_REG(0x2C); // ????
	}
	else if (TFT == 200) // ST7789
	{

		//		TFTLCD_WR_REG(0x36);
		//		TFTLCD_WR_DATA8(0x60);
		for (i = 0; i < 2; i++)
		{
			TFTLCD_WR_REG(0x36);
			//		if(USE_HORIZONTAL==0)TFTLCD_WR_DATA8(0x00);
			//		else if(USE_HORIZONTAL==1)TFTLCD_WR_DATA8(0x60);
			//		else if(USE_HORIZONTAL==2)TFTLCD_WR_DATA8(0xA8);
			//		else TFTLCD_WR_DATA8(0xc0);
			if (USE_HORIZONTAL == 0)
				TFTLCD_WR_DATA8(0x00);
			else if (USE_HORIZONTAL == 1)
				TFTLCD_WR_DATA8(0xc0);
			else if (USE_HORIZONTAL == 2)
				TFTLCD_WR_DATA8(0xa0);
			else
				TFTLCD_WR_DATA8(0x60);

			TFTLCD_WR_REG(0x3A);
			TFTLCD_WR_DATA8(0x05);

			TFTLCD_WR_REG(0xB2);
			TFTLCD_WR_DATA8(0x0C);
			TFTLCD_WR_DATA8(0x0C);
			TFTLCD_WR_DATA8(0x00);
			TFTLCD_WR_DATA8(0x33);
			TFTLCD_WR_DATA8(0x33);

			TFTLCD_WR_REG(0xB7);
			TFTLCD_WR_DATA8(0x35);

			TFTLCD_WR_REG(0xBB);
			TFTLCD_WR_DATA8(0x19);

			TFTLCD_WR_REG(0xC0);
			TFTLCD_WR_DATA8(0x2C);

			TFTLCD_WR_REG(0xC2);
			TFTLCD_WR_DATA8(0x01);

			TFTLCD_WR_REG(0xC3);
			TFTLCD_WR_DATA8(0x12);

			TFTLCD_WR_REG(0xC4);
			TFTLCD_WR_DATA8(0x20);

			TFTLCD_WR_REG(0xC6);
			TFTLCD_WR_DATA8(0x0F);

			TFTLCD_WR_REG(0xD0);
			TFTLCD_WR_DATA8(0xA4);
			TFTLCD_WR_DATA8(0xA1);

			TFTLCD_WR_REG(0xE0);
			TFTLCD_WR_DATA8(0xD0);
			TFTLCD_WR_DATA8(0x04);
			TFTLCD_WR_DATA8(0x0D);
			TFTLCD_WR_DATA8(0x11);
			TFTLCD_WR_DATA8(0x13);
			TFTLCD_WR_DATA8(0x2B);
			TFTLCD_WR_DATA8(0x3F);
			TFTLCD_WR_DATA8(0x54);
			TFTLCD_WR_DATA8(0x4C);
			TFTLCD_WR_DATA8(0x18);
			TFTLCD_WR_DATA8(0x0D);
			TFTLCD_WR_DATA8(0x0B);
			TFTLCD_WR_DATA8(0x1F);
			TFTLCD_WR_DATA8(0x23);

			TFTLCD_WR_REG(0xE1);
			TFTLCD_WR_DATA8(0xD0);
			TFTLCD_WR_DATA8(0x04);
			TFTLCD_WR_DATA8(0x0C);
			TFTLCD_WR_DATA8(0x11);
			TFTLCD_WR_DATA8(0x13);
			TFTLCD_WR_DATA8(0x2C);
			TFTLCD_WR_DATA8(0x3F);
			TFTLCD_WR_DATA8(0x44);
			TFTLCD_WR_DATA8(0x51);
			TFTLCD_WR_DATA8(0x2F);
			TFTLCD_WR_DATA8(0x1F);
			TFTLCD_WR_DATA8(0x1F);
			TFTLCD_WR_DATA8(0x20);
			TFTLCD_WR_DATA8(0x23);

			TFTLCD_WR_REG(0x21);

			TFTLCD_WR_REG(0x11);
			// Delay (120);

			TFTLCD_WR_REG(0x29);

			// ?????
			TFTLCD_WR_REG(0x2A);   // ?????
			TFTLCD_WR_DATA8(0x00); // ???????
			TFTLCD_WR_DATA8(0x00); // ???????
			TFTLCD_WR_DATA8(0x00); // ???????
			TFTLCD_WR_DATA8(0xEF); // ???????(240 ?)

			// ?????
			TFTLCD_WR_REG(0x2B);   // ?????
			TFTLCD_WR_DATA8(0x00); // ???????
			TFTLCD_WR_DATA8(0x00); // ???????
			TFTLCD_WR_DATA8(0x00); // ???????
			TFTLCD_WR_DATA8(0xC5); // ???????(198 ?)

			TFTLCD_WR_REG(0x2C); // ????
		}
		//
		//		TFTLCD_WR_REG(0x36);
		//		TFTLCD_WR_DATA8(0x60);
	}
	else if (TFT == 145)
	{
		TFTLCD_WR_REG(0x36);
		if (USE_HORIZONTAL == 0)
			TFTLCD_WR_DATA8(0x00);
		else if (USE_HORIZONTAL == 1)
			TFTLCD_WR_DATA8(0xc0);
		else if (USE_HORIZONTAL == 2)
			TFTLCD_WR_DATA8(0x60);
		else
			TFTLCD_WR_DATA8(0xa0);

		TFTLCD_WR_REG(0xFE);
		TFTLCD_WR_REG(0xEF);

		TFTLCD_WR_REG(0xB0);
		TFTLCD_WR_DATA8(0xC0);

		TFTLCD_WR_REG(0xB2);
		TFTLCD_WR_DATA8(0x2F);
		TFTLCD_WR_REG(0xB3);
		TFTLCD_WR_DATA8(0x03);
		TFTLCD_WR_REG(0xB6);
		TFTLCD_WR_DATA8(0x19);
		TFTLCD_WR_REG(0xB7);
		TFTLCD_WR_DATA8(0x01);

		TFTLCD_WR_REG(0xAC);
		TFTLCD_WR_DATA8(0xCB);
		TFTLCD_WR_REG(0xAB);
		TFTLCD_WR_DATA8(0x07);

		TFTLCD_WR_REG(0xB4);
		TFTLCD_WR_DATA8(0x00);

		TFTLCD_WR_REG(0xA8);
		TFTLCD_WR_DATA8(0x0C);

		TFTLCD_WR_REG(0x3A);
		TFTLCD_WR_DATA8(0x05);

		TFTLCD_WR_REG(0xb8);
		TFTLCD_WR_DATA8(0x08);

		TFTLCD_WR_REG(0xE8);
		TFTLCD_WR_DATA8(0x23);

		TFTLCD_WR_REG(0xE9);
		TFTLCD_WR_DATA8(0x47);

		TFTLCD_WR_REG(0xea);
		TFTLCD_WR_DATA8(0x44);

		TFTLCD_WR_REG(0xeb);
		TFTLCD_WR_DATA8(0xE0);

		TFTLCD_WR_REG(0xED);
		TFTLCD_WR_DATA8(0x03);

		TFTLCD_WR_REG(0xc6);
		TFTLCD_WR_DATA8(0x19);

		TFTLCD_WR_REG(0xc7);
		TFTLCD_WR_DATA8(0x10);

		TFTLCD_WR_REG(0xF0);
		TFTLCD_WR_DATA8(0x0b);
		TFTLCD_WR_DATA8(0x2f);
		TFTLCD_WR_DATA8(0x10);
		TFTLCD_WR_DATA8(0x4b);
		TFTLCD_WR_DATA8(0x28);
		TFTLCD_WR_DATA8(0x3f);
		TFTLCD_WR_DATA8(0x3e);
		TFTLCD_WR_DATA8(0x60);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x12);
		TFTLCD_WR_DATA8(0x12);
		TFTLCD_WR_DATA8(0x0f);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x1F);

		TFTLCD_WR_REG(0xF1);
		TFTLCD_WR_DATA8(0x0e);
		TFTLCD_WR_DATA8(0x3d);
		TFTLCD_WR_DATA8(0x2a);
		TFTLCD_WR_DATA8(0x40);
		TFTLCD_WR_DATA8(0xfa);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x02);
		TFTLCD_WR_DATA8(0x60);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x03);
		TFTLCD_WR_DATA8(0x13);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x10);
		TFTLCD_WR_DATA8(0x1F);

		delay_ms(120);
		TFTLCD_WR_REG(0x11);
		delay_ms(120);
		TFTLCD_WR_REG(0x29);
		delay_ms(120);
	}
	else if (TFT == 154) // ST7789
	{
		TFTLCD_WR_REG(0x36);
		if (USE_HORIZONTAL == 0)
			TFTLCD_WR_DATA8(0x00);
		else if (USE_HORIZONTAL == 1)
			TFTLCD_WR_DATA8(0x60);
		else if (USE_HORIZONTAL == 2)
			TFTLCD_WR_DATA8(0xa0);
		else
			TFTLCD_WR_DATA8(0xc0);

		TFTLCD_WR_REG(0x3A);
		TFTLCD_WR_DATA8(0x05);

		TFTLCD_WR_REG(0xB2);
		TFTLCD_WR_DATA8(0x0C);
		TFTLCD_WR_DATA8(0x0C);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x33);
		TFTLCD_WR_DATA8(0x33);

		TFTLCD_WR_REG(0xB7);
		TFTLCD_WR_DATA8(0x35);

		TFTLCD_WR_REG(0xBB);
		TFTLCD_WR_DATA8(0x19);

		TFTLCD_WR_REG(0xC0);
		TFTLCD_WR_DATA8(0x2C);

		TFTLCD_WR_REG(0xC2);
		TFTLCD_WR_DATA8(0x01);

		TFTLCD_WR_REG(0xC3);
		TFTLCD_WR_DATA8(0x12);

		TFTLCD_WR_REG(0xC4);
		TFTLCD_WR_DATA8(0x20);

		TFTLCD_WR_REG(0xC6);
		TFTLCD_WR_DATA8(0x0F);

		TFTLCD_WR_REG(0xD0);
		TFTLCD_WR_DATA8(0xA4);
		TFTLCD_WR_DATA8(0xA1);

		TFTLCD_WR_REG(0xE0);
		TFTLCD_WR_DATA8(0xD0);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x0D);
		TFTLCD_WR_DATA8(0x11);
		TFTLCD_WR_DATA8(0x13);
		TFTLCD_WR_DATA8(0x2B);
		TFTLCD_WR_DATA8(0x3F);
		TFTLCD_WR_DATA8(0x54);
		TFTLCD_WR_DATA8(0x4C);
		TFTLCD_WR_DATA8(0x18);
		TFTLCD_WR_DATA8(0x0D);
		TFTLCD_WR_DATA8(0x0B);
		TFTLCD_WR_DATA8(0x1F);
		TFTLCD_WR_DATA8(0x23);

		TFTLCD_WR_REG(0xE1);
		TFTLCD_WR_DATA8(0xD0);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x0C);
		TFTLCD_WR_DATA8(0x11);
		TFTLCD_WR_DATA8(0x13);
		TFTLCD_WR_DATA8(0x2C);
		TFTLCD_WR_DATA8(0x3F);
		TFTLCD_WR_DATA8(0x44);
		TFTLCD_WR_DATA8(0x51);
		TFTLCD_WR_DATA8(0x2F);
		TFTLCD_WR_DATA8(0x1F);
		TFTLCD_WR_DATA8(0x1F);
		TFTLCD_WR_DATA8(0x20);
		TFTLCD_WR_DATA8(0x23);

		TFTLCD_WR_REG(0x21);

		TFTLCD_WR_REG(0x11);
		// Delay (120);

		TFTLCD_WR_REG(0x29);

		// ?????
		TFTLCD_WR_REG(0x2A);   // ?????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0xEF); // ???????(240 ?)

		// ?????
		TFTLCD_WR_REG(0x2B);   // ?????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0xC5); // ???????(198 ?)

		TFTLCD_WR_REG(0x2C); // ????
	}
	else if (TFT == 225) // ST7789
	{
		//		TFTLCD_WR_REG(0x36);
		//		if(USE_HORIZONTAL==0)TFTLCD_WR_DATA8(0x00);
		//		else if(USE_HORIZONTAL==1)TFTLCD_WR_DATA8(0xc0);
		//		else if(USE_HORIZONTAL==2)TFTLCD_WR_DATA8(0x60);
		//		else TFTLCD_WR_DATA8(0xa0);

		TFTLCD_WR_REG(0x11); // Sleep out
		delay_ms(120);		 // Delay 120ms

		TFTLCD_WR_REG(0xB1);
		TFTLCD_WR_DATA8(0x05);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_REG(0xB2);
		TFTLCD_WR_DATA8(0x05);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_REG(0xB3);
		TFTLCD_WR_DATA8(0x05);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_DATA8(0x05);
		TFTLCD_WR_DATA8(0x3C);
		TFTLCD_WR_DATA8(0x3C);

		TFTLCD_WR_REG(0xB4); // Dot inversion
		TFTLCD_WR_DATA8(0x03);

		TFTLCD_WR_REG(0xC0);
		TFTLCD_WR_DATA8(0x28);
		TFTLCD_WR_DATA8(0x08);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_REG(0xC1);
		TFTLCD_WR_DATA8(0XC0);
		TFTLCD_WR_REG(0xC2);
		TFTLCD_WR_DATA8(0x0D);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_REG(0xC3);
		TFTLCD_WR_DATA8(0x8D);
		TFTLCD_WR_DATA8(0x2A);
		TFTLCD_WR_REG(0xC4);
		TFTLCD_WR_DATA8(0x8D);
		TFTLCD_WR_DATA8(0xEE);

		TFTLCD_WR_REG(0xC5); // VCOM
		TFTLCD_WR_DATA8(0x1A);
		TFTLCD_WR_REG(0x36); // MX, MY, RGB mode
		if (USE_HORIZONTAL == 0)
			TFTLCD_WR_DATA8(0x00);
		else if (USE_HORIZONTAL == 1)
			TFTLCD_WR_DATA8(0xC0);
		else if (USE_HORIZONTAL == 2)
			TFTLCD_WR_DATA8(0x70);
		else
			TFTLCD_WR_DATA8(0xA0);

		TFTLCD_WR_REG(0xE0);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x22);
		TFTLCD_WR_DATA8(0x07);
		TFTLCD_WR_DATA8(0x0A);
		TFTLCD_WR_DATA8(0x2E);
		TFTLCD_WR_DATA8(0x30);
		TFTLCD_WR_DATA8(0x25);
		TFTLCD_WR_DATA8(0x2A);
		TFTLCD_WR_DATA8(0x28);
		TFTLCD_WR_DATA8(0x26);
		TFTLCD_WR_DATA8(0x2E);
		TFTLCD_WR_DATA8(0x3A);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x01);
		TFTLCD_WR_DATA8(0x03);
		TFTLCD_WR_DATA8(0x13);
		TFTLCD_WR_REG(0xE1);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x16);
		TFTLCD_WR_DATA8(0x06);
		TFTLCD_WR_DATA8(0x0D);
		TFTLCD_WR_DATA8(0x2D);
		TFTLCD_WR_DATA8(0x26);
		TFTLCD_WR_DATA8(0x23);
		TFTLCD_WR_DATA8(0x27);
		TFTLCD_WR_DATA8(0x27);
		TFTLCD_WR_DATA8(0x25);
		TFTLCD_WR_DATA8(0x2D);
		TFTLCD_WR_DATA8(0x3B);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x01);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x13);

		TFTLCD_WR_REG(0x3A); // 65k mode
		TFTLCD_WR_DATA8(0x05);
		TFTLCD_WR_REG(0x29); // Display on
	}
	else if (TFT == 240) // ST7789
	{
		TFTLCD_WR_REG(0x36);
		if (USE_HORIZONTAL == 0)
			TFTLCD_WR_DATA8(0x00);
		else if (USE_HORIZONTAL == 1)
			TFTLCD_WR_DATA8(0x60);
		else if (USE_HORIZONTAL == 2)
			TFTLCD_WR_DATA8(0xa0);
		else
			TFTLCD_WR_DATA8(0xc0);

		TFTLCD_WR_REG(0x3A);
		TFTLCD_WR_DATA8(0x05);

		TFTLCD_WR_REG(0xB2);
		TFTLCD_WR_DATA8(0x0C);
		TFTLCD_WR_DATA8(0x0C);
		TFTLCD_WR_DATA8(0x00);
		TFTLCD_WR_DATA8(0x33);
		TFTLCD_WR_DATA8(0x33);

		TFTLCD_WR_REG(0xB7);
		TFTLCD_WR_DATA8(0x35);

		TFTLCD_WR_REG(0xBB);
		TFTLCD_WR_DATA8(0x19);

		TFTLCD_WR_REG(0xC0);
		TFTLCD_WR_DATA8(0x2C);

		TFTLCD_WR_REG(0xC2);
		TFTLCD_WR_DATA8(0x01);

		TFTLCD_WR_REG(0xC3);
		TFTLCD_WR_DATA8(0x12);

		TFTLCD_WR_REG(0xC4);
		TFTLCD_WR_DATA8(0x20);

		TFTLCD_WR_REG(0xC6);
		TFTLCD_WR_DATA8(0x0F);

		TFTLCD_WR_REG(0xD0);
		TFTLCD_WR_DATA8(0xA4);
		TFTLCD_WR_DATA8(0xA1);

		TFTLCD_WR_REG(0xE0);
		TFTLCD_WR_DATA8(0xD0);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x0D);
		TFTLCD_WR_DATA8(0x11);
		TFTLCD_WR_DATA8(0x13);
		TFTLCD_WR_DATA8(0x2B);
		TFTLCD_WR_DATA8(0x3F);
		TFTLCD_WR_DATA8(0x54);
		TFTLCD_WR_DATA8(0x4C);
		TFTLCD_WR_DATA8(0x18);
		TFTLCD_WR_DATA8(0x0D);
		TFTLCD_WR_DATA8(0x0B);
		TFTLCD_WR_DATA8(0x1F);
		TFTLCD_WR_DATA8(0x23);

		TFTLCD_WR_REG(0xE1);
		TFTLCD_WR_DATA8(0xD0);
		TFTLCD_WR_DATA8(0x04);
		TFTLCD_WR_DATA8(0x0C);
		TFTLCD_WR_DATA8(0x11);
		TFTLCD_WR_DATA8(0x13);
		TFTLCD_WR_DATA8(0x2C);
		TFTLCD_WR_DATA8(0x3F);
		TFTLCD_WR_DATA8(0x44);
		TFTLCD_WR_DATA8(0x51);
		TFTLCD_WR_DATA8(0x2F);
		TFTLCD_WR_DATA8(0x1F);
		TFTLCD_WR_DATA8(0x1F);
		TFTLCD_WR_DATA8(0x20);
		TFTLCD_WR_DATA8(0x23);

		TFTLCD_WR_REG(0x21);

		TFTLCD_WR_REG(0x11);
		// Delay (120);

		TFTLCD_WR_REG(0x29);

		// ?????
		TFTLCD_WR_REG(0x2A);   // ?????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0xEF); // ???????(240 ?)

		// ?????
		TFTLCD_WR_REG(0x2B);   // ?????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0x00); // ???????
		TFTLCD_WR_DATA8(0xC5); // ???????(198 ?)

		TFTLCD_WR_REG(0x2C); // ????
	}
}

void TFTLCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 *color)
{
	//		u16 i, j;
	//		TFTLCD_Address_Set(xsta, ysta, xend - 1, yend - 1);
	//		for (i = ysta; i < yend; i++)
	//		{
	//			for (j = xsta; j < xend; j++)
	//			{
	//				TFTLCD_WR_DATA(color);
	//			}
	//		}

	u16 height, width;
	u16 i, j;
	width = xend - xsta + 1;
	height = yend - ysta + 1;
	for (i = 0; i < height; i++)
	{
		TFTLCD_WR_REG(0x2a);
		TFTLCD_WR_DATA(xsta + i);
		TFTLCD_WR_REG(0x2c);
		for (j = 0; j < width; j++)
		{
			TFTLCD_WR_REG(0x2b);
			TFTLCD_WR_DATA(ysta + j);
			TFTLCD_WR_REG(0x2c);
			TFTLCD_WR_DATA(color[i * width + j]);
		}
	}
}

void TFTLCD_DrawPoint(u16 x, u16 y, u16 color)
{
	TFTLCD_Address_Set(x, y, x, y);
	TFTLCD_WR_DATA(color);
}

void TFTLCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1;
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)
		incx = 1;
	else if (delta_x == 0)
		incx = 0;
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0;
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x;
	else
		distance = delta_y;
	for (t = 0; t < distance + 1; t++)
	{
		TFTLCD_DrawPoint(uRow, uCol, color);
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

void TFTLCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	TFTLCD_DrawLine(x1, y1, x2, y1, color);
	TFTLCD_DrawLine(x1, y1, x1, y2, color);
	TFTLCD_DrawLine(x1, y2, x2, y2, color);
	TFTLCD_DrawLine(x2, y1, x2, y2, color);
}

void TFTLCD_DrawCircle(u16 x0, u16 y0, u8 r, u16 color)
{
	int a, b;
	a = 0;
	b = r;
	while (a <= b)
	{
		TFTLCD_DrawPoint(x0 - b, y0 - a, color); // 3
		TFTLCD_DrawPoint(x0 + b, y0 - a, color); // 0
		TFTLCD_DrawPoint(x0 - a, y0 + b, color); // 1
		TFTLCD_DrawPoint(x0 - a, y0 - b, color); // 2
		TFTLCD_DrawPoint(x0 + b, y0 + a, color); // 4
		TFTLCD_DrawPoint(x0 + a, y0 - b, color); // 5
		TFTLCD_DrawPoint(x0 + a, y0 + b, color); // 6
		TFTLCD_DrawPoint(x0 - b, y0 + a, color); // 7
		a++;
		if ((a * a + b * b) > (r * r))
		{
			b--;
		}
	}
}

void TFTLCD_ShowChar(u16 x, u16 y, u8 num, u16 fc, u16 bc, u8 sizey, u8 mode)
{
	u8 temp, sizex, t, m = 0;
	u16 i, TypefaceNum;
	u16 x0 = x;
	sizex = sizey / 2;
	TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
	num = num - ' ';
	TFTLCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1);
	for (i = 0; i < TypefaceNum; i++)
	{
		if (sizey == 12)
			temp = ascii_1206[num][i];
		else if (sizey == 16)
			temp = ascii_1608[num][i];
		else if (sizey == 24)
			temp = ascii_2412[num][i];
		else
			return;
		for (t = 0; t < 8; t++)
		{
			if (!mode)
			{
				if (temp & (0x01 << t))
					TFTLCD_WR_DATA(fc);
				else
					TFTLCD_WR_DATA(bc);
				m++;
				if (m % sizex == 0)
				{
					m = 0;
					break;
				}
			}
			else
			{
				if (temp & (0x01 << t))
					TFTLCD_DrawPoint(x, y, fc);
				x++;
				if ((x - x0) == sizex)
				{
					x = x0;
					y++;
					break;
				}
			}
		}
	}
}

void TFTLCD_ShowString(u16 x, u16 y, const u8 *p, u16 fc, u16 bc, u8 sizey, u8 mode)
{
	while (*p != '\0')
	{
		TFTLCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
		x += sizey / 2;
		p++;
	}
}

u32 mypow(u8 m, u8 n)
{
	u32 result = 1;
	while (n--)
		result *= m;
	return result;
}

void TFTLCD_ShowIntNum(u16 x, u16 y, u16 num, u8 len, u16 fc, u16 bc, u8 sizey)
{
	u8 t, temp;
	u8 enshow = 0;
	u8 sizex = sizey / 2;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				TFTLCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
				continue;
			}
			else
				enshow = 1;
		}
		TFTLCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
	}
}

void TFTLCD_ShowFloatNum1(u16 x, u16 y, float num, u8 len, u16 fc, u16 bc, u8 sizey)
{
	u8 t, temp, sizex;
	u16 num1;
	sizex = sizey / 2;
	num1 = num * 100;
	for (t = 0; t < len; t++)
	{
		temp = (num1 / mypow(10, len - t - 1)) % 10;
		if (t == (len - 2))
		{
			TFTLCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
			t++;
			len += 1;
		}
		TFTLCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
	}
}

void TFTLCD_ShowPicture(u16 x, u16 y, u16 length, u16 width, const u8 pic[])
{
	u16 i, j;
	u32 k = 0;
	TFTLCD_Address_Set(x, y, x + length - 1, y + width - 1);
	for (i = 0; i < length; i++)
	{
		for (j = 0; j < width; j++)
		{
			TFTLCD_WR_DATA8(pic[k * 2]);
			TFTLCD_WR_DATA8(pic[k * 2 + 1]);
			k++;
		}
	}
}

// void TFTLCD_Image(u16 x, u16 y, u16 width, u16 height,  u16 *image)
//{
//	u16 colorData = 0;
//	u32 cnt = 0;
//	u16 i = 0, j = 0;
//	for (i = 0; i < height; i++)
//	{
//		TFTLCD_Address_Set(x, y + i, x + width, y + height);
//		for (j = 0; j < width; j++)
//		{
//			colorData = (image[cnt * 2 + 1] << 8) | image[cnt * 2];
//			TFTLCD_WR_DATA(colorData);
//			cnt++;
//		}
//	}
// }
void TFTLCD_Image(u16 x1, u16 y1, u16 x2, u16 y2, u16 *image)
{
	u16 colorData = 0;
	u32 cnt = 0;
	u16 i = 0, j = 0;
	u16 width;
	u16 height;
	width = x2 - x1 + 1;
	height = y2 - y1 + 1;
	for (i = 0; i < height; i++)
	{
		TFTLCD_Address_Set(x1, y1 + i, x1 + width, y1 + height);
		for (j = 0; j < width; j++)
		{
			//			colorData = (image[cnt * 2 + 1] << 8) | image[cnt * 2];
			colorData = image[i * width + j];
			TFTLCD_WR_DATA(colorData);
			cnt++;
		}
	}
}