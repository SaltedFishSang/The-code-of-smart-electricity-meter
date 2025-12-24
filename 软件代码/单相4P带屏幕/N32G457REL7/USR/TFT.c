#include "BSP.h"
/********************金逸晨电子**************************
LCD:T200H7-C14-05
IC: ST7789P3
***** PB6    SCK
      PB7    SDA
      PB9    RESET
      PB8    D/C
      PB5    CS
******************************************************/

#include "BSP.h"
#define DIS_DIR 0
#define USE_HORIZONTAL 1 // 设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#define TFT_COLUMN_NUMBER 240
#define TFT_LINE_NUMBER 320

#define TFT_COLUMN_OFFSET 0
#define TFT_LINE_OFFSET 0

/**********SPI引脚分配，连接TFT屏，更具实际情况修改*********/

#define TFT_SCK_0 GPIO_ResetBits(GPIOB, GPIO_PIN_6) // 设置sck接口到PB6
#define TFT_SCK_1 GPIO_SetBits(GPIOB, GPIO_PIN_6)   //

#define TFT_SDA_0 GPIO_ResetBits(GPIOB, GPIO_PIN_7) // 设置SDA接口到PB7
#define TFT_SDA_1 GPIO_SetBits(GPIOB, GPIO_PIN_7)

#define TFT_RST_0 GPIO_ResetBits(GPIOB, GPIO_PIN_9) // 设置RST接口到PB9
#define TFT_RST_1 GPIO_SetBits(GPIOB, GPIO_PIN_9)

#define TFT_DC_0 GPIO_ResetBits(GPIOB, GPIO_PIN_8) // 设置DC接口到PB8
#define TFT_DC_1 GPIO_SetBits(GPIOB, GPIO_PIN_8)

#define TFT_CS_0 GPIO_ResetBits(GPIOB, GPIO_PIN_5) // 设置CS接口到PB5
#define TFT_CS_1 GPIO_SetBits(GPIOB, GPIO_PIN_5)

#define TFT_Light_0 GPIO_SetBits(GPIOB, GPIO_PIN_4)
#define TFT_Light_1 GPIO_ResetBits(GPIOB, GPIO_PIN_4)
/*****************颜色数据**************************/
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define GREEN 0x07E0

unsigned int page = 0;
unsigned int autoplay = 0;

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
    // 禁用JTAG，使能SW，效果为配置PB3为普通IO
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);
    GPIO_ConfigPinRemap(GPIO_RMP_SW_JTAG_SW_ENABLE, ENABLE);
    TFT_Light_1;
}
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

/*************SPI配置函数*******************
，SCL空闲时低电平，第一个上升沿采样
模拟SPI
******************************************/

/**************************SPI模块发送函数************************************************

 *************************************************************************/
void SPI_SendByte(unsigned char byte)
{
    unsigned char counter;
    for (counter = 0; counter < 8; counter++)
    {
        TFT_SCK_0;

        if ((byte & 0x80) == 0)
        {
            TFT_SDA_0;
        }
        else
        {
            TFT_SDA_1;
        }
        byte = byte << 1;
        TFT_SCK_1;
    }
    TFT_SCK_0;
}

void TFT_SEND_CMD(unsigned char o_command)
{
    TFT_CS_0;
    TFT_DC_0;
    SPI_SendByte(o_command);
    TFT_CS_1;
    // TFT_DC_1;
}

void TFT_SEND_DATA(unsigned char o_data)
{
    TFT_CS_0;
    TFT_DC_1;
    SPI_SendByte(o_data);
    TFT_CS_1;
}

void TFT_SET_ADD(unsigned short int X, unsigned short int Y, unsigned short int X_END, unsigned short int Y_END)
{
    TFT_SEND_CMD(0x2A); // 窗口地址横向
    TFT_SEND_DATA(X >> 8);
    TFT_SEND_DATA(X); // start column
    TFT_SEND_DATA(X_END >> 8);
    TFT_SEND_DATA(X_END); // start column
    TFT_SEND_CMD(0x2B);   // 窗口地址纵向
    TFT_SEND_DATA(Y >> 8);
    TFT_SEND_DATA(Y); // start row
    TFT_SEND_DATA(Y_END >> 8);
    TFT_SEND_DATA(Y_END); // start row
    TFT_SEND_CMD(0x2C);   // 写入数据
}

void TFT_init(void) // GC9A01
{
    TFT_RST_0; // 复位
    delay_ms(100);
    TFT_RST_1;
    delay_ms(100);

    TFT_SEND_CMD(0x11);
    delay_ms(120);

    TFT_SEND_CMD(0x36);
    if (USE_HORIZONTAL == 0)
        TFT_SEND_DATA(0x00);
    else if (USE_HORIZONTAL == 1)
        TFT_SEND_DATA(0xC0);
    else if (USE_HORIZONTAL == 2)
        TFT_SEND_DATA(0x70);
    else
        TFT_SEND_DATA(0xA0);

    TFT_SEND_CMD(0x3A);
    TFT_SEND_DATA(0x05); // 16BIT

    TFT_SEND_CMD(0xB2);
    TFT_SEND_DATA(0x05);
    TFT_SEND_DATA(0x05);
    TFT_SEND_DATA(0x00);
    TFT_SEND_DATA(0x33);
    TFT_SEND_DATA(0x33);

    TFT_SEND_CMD(0xB7);
    TFT_SEND_DATA(0x35);

    TFT_SEND_CMD(0xBB);
    TFT_SEND_DATA(0x21);

    TFT_SEND_CMD(0xC0);
    TFT_SEND_DATA(0x2C);

    TFT_SEND_CMD(0xC2);
    TFT_SEND_DATA(0x01);

    TFT_SEND_CMD(0xC3);
    TFT_SEND_DATA(0x0B);

    TFT_SEND_CMD(0xC4);
    TFT_SEND_DATA(0x20);

    TFT_SEND_CMD(0xC6);
    TFT_SEND_DATA(0x0F); // 60HZ dot inversion

    TFT_SEND_CMD(0xD0);
    TFT_SEND_DATA(0xA7);
    TFT_SEND_DATA(0xA1);

    TFT_SEND_CMD(0xD0);
    TFT_SEND_DATA(0xA4);
    TFT_SEND_DATA(0xA1);

    TFT_SEND_CMD(0xD6);
    TFT_SEND_DATA(0xA1);

    TFT_SEND_CMD(0xE0);
    TFT_SEND_DATA(0xD0);
    TFT_SEND_DATA(0x04);
    TFT_SEND_DATA(0x08);
    TFT_SEND_DATA(0x0A);
    TFT_SEND_DATA(0x09);
    TFT_SEND_DATA(0x05);
    TFT_SEND_DATA(0x2D);
    TFT_SEND_DATA(0x43);
    TFT_SEND_DATA(0x49);
    TFT_SEND_DATA(0x09);
    TFT_SEND_DATA(0x16);
    TFT_SEND_DATA(0x15);
    TFT_SEND_DATA(0x26);
    TFT_SEND_DATA(0x2B);

    TFT_SEND_CMD(0xE1);
    TFT_SEND_DATA(0xD0);
    TFT_SEND_DATA(0x03);
    TFT_SEND_DATA(0x09);
    TFT_SEND_DATA(0x0A);
    TFT_SEND_DATA(0x0A);
    TFT_SEND_DATA(0x06);
    TFT_SEND_DATA(0x2E);
    TFT_SEND_DATA(0x44);
    TFT_SEND_DATA(0x40);
    TFT_SEND_DATA(0x3A);
    TFT_SEND_DATA(0x15);
    TFT_SEND_DATA(0x15);
    TFT_SEND_DATA(0x26);
    TFT_SEND_DATA(0x2A);

    TFT_SEND_CMD(0x21);

    // TFT_SEND_CMD(0x29);

    delay_ms(10);
    // TFT_SEND_CMD(0x29);
}

void TFT_clear()
{
    TFT_SET_ADD(0, 0, TFT_COLUMN_NUMBER - 1, TFT_LINE_NUMBER - 1);
    for (int i = 0; i < 128000; i++) // Line loop
    {
        TFT_SEND_DATA(0xF);
        TFT_SEND_DATA(0xF);
    }
}

void TFT_full(unsigned int color)
{
    unsigned long Line, column;
    TFT_SET_ADD(0, 0, TFT_COLUMN_NUMBER - 1, TFT_LINE_NUMBER - 1);
    for (Line = 0; Line < TFT_LINE_NUMBER; Line++) // Line loop
    {
        for (column = 0; column < TFT_COLUMN_NUMBER; column++) // column loop
        {
            TFT_SEND_DATA(color >> 8);
            TFT_SEND_DATA(color);
        }
    }
}

void TFT_Fill(u16 x, u16 y, u16 length, u16 width, unsigned int color)
{
    u16 i, j;
    TFT_SET_ADD(x, y, x + length - 1, y + width - 1);
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < width; j++)
        {
            TFT_SEND_DATA(color >> 8);
            TFT_SEND_DATA(color);
        }
    }
}

void Picture_display(u16 x, u16 y, u16 length, u16 width, const unsigned char *ptr_pic)
{
    u16 i, j;
    TFT_SET_ADD(x, y, x + length - 1, y + width - 1);
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < width; j++)
        {
            TFT_SEND_DATA(*ptr_pic++);
            TFT_SEND_DATA(*ptr_pic++);
        }
    }
}

void TFT_Page_bar()
{
    int col = TFT_COLUMN_NUMBER;
    int row = TFT_LINE_NUMBER;

    int c = 0;
    int r = 0;

    int c2 = 0;
    // int r2 = 0;

    int ex = 0;

    for (c = 0; c < col; c++)
    {
        for (r = 0; r < row; r++)
        {
            c2 = (int)(c / 30);
            // r2 = (int)(r/30);

            ex = c2 % 8;
            // ex = r2 % 8;

            if (ex == 0)
            {
                TFT_SEND_DATA(0xFF);
                TFT_SEND_DATA(0xFF);
            }
            else if (ex == 1)
            {
                TFT_SEND_DATA(0x00);
                TFT_SEND_DATA(0x00);
            }
            else if (ex == 2)
            {
                TFT_SEND_DATA(0x00);
                TFT_SEND_DATA(0x1F);
            }
            else if (ex == 3)
            {
                TFT_SEND_DATA(0xF8);
                TFT_SEND_DATA(0x1F);
            }
            else if (ex == 4)
            {
                TFT_SEND_DATA(0xFF);
                TFT_SEND_DATA(0xE0);
            }
            else if (ex == 5)
            {
                TFT_SEND_DATA(0x07);
                TFT_SEND_DATA(0xFF);
            }
            else if (ex == 6)
            {
                TFT_SEND_DATA(0xF8);
                TFT_SEND_DATA(0x00);
            }
            else if (ex == 7)
            {
                TFT_SEND_DATA(0x07);
                TFT_SEND_DATA(0xE0);
            }
            // delay_ms(10);
        }
    }
}

void TFT_Page_border()
{
    int col = TFT_COLUMN_NUMBER;
    int row = TFT_LINE_NUMBER;

    int c, r;

    TFT_SET_ADD(0, 0, TFT_COLUMN_NUMBER - 1, TFT_LINE_NUMBER - 1);

    for (r = 0; r < row; r++)
    {
        for (c = 0; c < col; c++)
        {
            if (c == 0 || r == 0 || c == col - 1 || r == row - 1)
            {
                TFT_SEND_DATA(0xFF);
                TFT_SEND_DATA(0xFF);
            }
            else
            {
                TFT_SEND_DATA(0x00);
                TFT_SEND_DATA(0x00);
            }
            // delay_ms(50);
        }
    }
}

void TFT_Page_net()
{
    int col = TFT_COLUMN_NUMBER;
    int row = TFT_LINE_NUMBER;

    int c, r;

    TFT_SET_ADD(0, 0, TFT_COLUMN_NUMBER - 1, TFT_LINE_NUMBER - 1);

    for (c = 0; c < col; c++)
    {
        for (r = 0; r < row; r++)
        {
            if (r % 2 == 0)
            {
                TFT_SEND_DATA(0xFF);
                TFT_SEND_DATA(0xFF);
            }
            else
            {
                TFT_SEND_DATA(0x00);
                TFT_SEND_DATA(0x00);
            }
        }
    }
}

void TFT_Page_cell()
{
    int col = TFT_COLUMN_NUMBER;
    int row = TFT_LINE_NUMBER;

    int c = 0;
    int r = 0;

    int c2 = 0;
    int r2 = 0;

    int c3 = 0;
    int r3 = 0;

    TFT_SET_ADD(0, 0, TFT_COLUMN_NUMBER - 1, TFT_LINE_NUMBER - 1);

    for (c = 0; c < col; c++)
    {
        for (r = 0; r < row; r++)
        {
            c2 = (int)(c / 20);
            r2 = (int)(r / 20);
            c3 = c2 % 2;
            r3 = r2 % 2;

            if (c3 == r3)
            {
                TFT_SEND_DATA(0xFF);
                TFT_SEND_DATA(0xFF);
            }
            else
            {
                TFT_SEND_DATA(0x00);
                TFT_SEND_DATA(0x00);
            }
        }
    }
}
