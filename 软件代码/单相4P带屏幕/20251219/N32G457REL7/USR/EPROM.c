#include "BSP.h"

#define EPROM_DBG 0

/*******IIC初始化函数 GPIO口初始化********/
void IIC_Init(void)
{
    /*SCL-->PB13  SDA-->PB12*/
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    SCL_HIGH;
    SDA_HIGH;
}

void SDA_OUT(void)
{
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
}

void SDA_IN(void)
{
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
}

// IIC 起始
void IIC_Start(void)
{
    SDA_OUT();
    SDA_HIGH;
    SCL_HIGH;
    delay_us(2);
    SDA_LOW; // START:when CLK is high,DATA change form high to low
    delay_us(2);
    SCL_LOW;
}

// 产生IIC停止信号
void IIC_Stop(void)
{
    SDA_OUT(); //
    SDA_LOW;   // STOP:when CLK is high DATA change form low to high
    SCL_HIGH;
    delay_us(2);
    SDA_HIGH;
}

// 等待应答信号到来
// 返回值： 1，接收应答失败
//          0,接收应答成功
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    SDA_IN(); // SDA设置为输入
    SDA_HIGH;
    delay_us(1);
    SCL_HIGH;
    delay_us(1);
    while (READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    SCL_LOW;
    return 0;
}

void IIC_Ack(void)
{
    SCL_LOW;
    SDA_OUT();
    SDA_LOW;
    delay_us(2);
    SCL_HIGH;
    delay_us(2);
    SCL_LOW;
}

void IIC_NAck(void)
{
    SCL_LOW;
    SDA_OUT();
    SDA_HIGH;
    delay_us(2);
    SCL_HIGH;
    delay_us(2);
    SCL_LOW;
}

void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    SCL_LOW;
    for (t = 0; t < 8; t++)
    {
        // IIC_SDA=(txd&0x80)>>7;
        if (txd & 0x80)
        {
            SDA_HIGH;
        }
        else
        {
            SDA_LOW;
        }
        txd <<= 1;
        delay_us(2);
        SCL_HIGH;
        delay_us(2);
        SCL_LOW;
        delay_us(2);
    }
}

u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN();
    for (i = 0; i < 8; i++)
    {
        SCL_LOW;
        delay_us(2);
        SCL_HIGH;
        receive <<= 1;
        if (READ_SDA)
            receive++;
        delay_us(1);
    }
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return receive;
}

// 初始化IIC接口
void AT24CXX_Init(void)
{
    IIC_Init();
}

#if EE_TYPE <= AT24C08 // 24C01/02/08
// 在AT24CXX指定地址读出一个数据
// ReadAddr:开始读数的地址
// 返回值:读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
    u8 temp = 0;
    IIC_Start();
    if (EE_TYPE > AT24C16)
    {
        IIC_Send_Byte(0XA0); // 发送写命令
        IIC_Wait_Ack();
        IIC_Send_Byte(ReadAddr >> 8); // 发送高地址
        IIC_Wait_Ack();
    }
    else
        IIC_Send_Byte(0XA0 + ((ReadAddr / 256) << 1)); // 发送器件地址0xA0 写数据

    IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr % 256); // 发送低地址
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(0XA1); // 进入接收模式
    IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);
    IIC_Stop(); // 产生一个停止条件
    return temp;
}

// 在AT24CXX 指定地址写入一个数据
//  WriteAddr 写入数据的目的地址
// DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
    IIC_Start();
    if (EE_TYPE > AT24C16)
    {
        IIC_Send_Byte(0XA0); // 发送设备地址
        IIC_Wait_Ack();
        IIC_Send_Byte(WriteAddr >> 8); // 发送字节高地址
    }
    else
    {
        IIC_Send_Byte(0XA0 + ((WriteAddr / 256) << 1)); // 发送器件地址0xA0 写数据
    }
    IIC_Wait_Ack();
    IIC_Send_Byte(WriteAddr % 256); // 发送字节低地址
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite); // 发送要写入的数据
    IIC_Wait_Ack();
    IIC_Stop();      // 产生一个停止条件
    delay_us(10000); // 10ms
}
#else // 24C16

/*******************************************************
 *函数名 AT24CXX_ReadOneByte(u16 ReadAddr)
 *功能：AT24CXX 读指定地址的一个字节 AT24C16使用
 *调用:底层I2C 读写函数
 ********************************************************/

u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
    unsigned char Page = 0, WordAddress = 0, DeviceAddress = 0xA0;
    u8 temp = 0;
    Page = ReadAddr / AT24CXX_Page_Size;
    WordAddress = (ReadAddr % AT24CXX_Page_Size) & 0x0F;
    DeviceAddress |= (((Page << 1) & 0xE0) >> 4); // High 3 bits
    WordAddress |= (Page & 0x0F) << 4;            // Low 4 bits
    IIC_Start();
    IIC_Send_Byte(DeviceAddress & 0xFE); // 发送设备地址+写方向
    IIC_Wait_Ack();
    IIC_Send_Byte(WordAddress); // 发送字节地址
    IIC_Wait_Ack();
    IIC_Start();                         // 起始信号
    IIC_Send_Byte(DeviceAddress | 0x01); // 发送设备地址+读方向
    IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);
    IIC_Stop();      // 产生一个停止条件
    delay_us(20000); // 10ms
    return temp;
}

/*****************************************************
 *函数名：AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
 * AT24C16用
 *
 ******************************************************/
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
    unsigned char Page = 0, WordAddress = 0, DeviceAddress = 0xA0;
    Page = WriteAddr / AT24CXX_Page_Size;
    WordAddress = (WriteAddr % AT24CXX_Page_Size) & 0x0F;
    DeviceAddress |= (((Page << 1) & 0xE0) >> 4); // High 3 bits
    WordAddress |= (Page & 0x0F) << 4;            // Low 4 bits
#if DEBUG > 0
    printf("Page:%x\r\n", Page);
    printf("WordAddress:%x\r\n", WordAddress);
    printf("DeviveAddress:%x\r\n", DeviceAddress);
#endif
    IIC_Start();
    IIC_Send_Byte(DeviceAddress); // 发送设备地址
    IIC_Wait_Ack();
    IIC_Send_Byte(WordAddress); // 发送字节地址
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite); // 发送要写入的数据
    IIC_Wait_Ack();
    IIC_Stop(); // 产生一个停止条件
    delay_us(20000);
}

#endif

#if QuickWR == 0

void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
    while (NumToRead)
    {
        *pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}

void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
    while (NumToWrite--)
    {
        AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}

#else

void AT24CXX_Write_Bytes(u8 *pBuffer, u16 WriteAddress, u8 Len)
{
    unsigned char Page = 0, WordAddress = 0, DeviceAddress = 0xA0;
    u8 i = 0;
    Page = WriteAddress / AT24CXX_Page_Size;
    WordAddress = (WriteAddress % AT24CXX_Page_Size) & 0x0F;
    DeviceAddress |= (((Page << 1) & 0xE0) >> 4); // High 3 bits
    WordAddress |= (Page & 0x0F) << 4;            // Low 4 bits
    IIC_Start();
    IIC_Send_Byte(DeviceAddress);
    IIC_Wait_Ack();
    IIC_Send_Byte(WordAddress);
    IIC_Wait_Ack();
    for (i = 0; i < Len; i++)
    {
        IIC_Send_Byte(*pBuffer++);
        IIC_Wait_Ack();
    }
    IIC_Stop();
    delay_ms(10);
}

void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
    unsigned char NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
    Addr = WriteAddr % AT24CXX_Page_Size;
    count = AT24CXX_Page_Size - Addr;
    NumOfPage = NumToWrite / AT24CXX_Page_Size;
    NumOfSingle = NumToWrite % AT24CXX_Page_Size;
    if (0 == Addr)
    {
        if (NumToWrite <= AT24CXX_Page_Size)
        {
            AT24CXX_Write_Bytes(pBuffer, WriteAddr, NumToWrite);
        }
        else
        {
            while (NumOfPage--)
            {
                AT24CXX_Write_Bytes(pBuffer, WriteAddr, AT24CXX_Page_Size);
                pBuffer += AT24CXX_Page_Size;
                WriteAddr += AT24CXX_Page_Size;
            }
            if (NumOfSingle != 0)
            {
                AT24CXX_Write_Bytes(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
    else
    {
        if (NumToWrite <= count)
        {
            AT24CXX_Write_Bytes(pBuffer, WriteAddr, NumToWrite);
        }
        else
        {
            AT24CXX_Write_Bytes(pBuffer, WriteAddr, count);
            NumToWrite -= count;
            pBuffer += count;
            WriteAddr += count;

            NumOfPage = NumToWrite / AT24CXX_Page_Size;
            NumOfSingle = NumToWrite % AT24CXX_Page_Size;

            while (NumOfPage--)
            {
                AT24CXX_Write_Bytes(pBuffer, WriteAddr, AT24CXX_Page_Size);
                pBuffer += AT24CXX_Page_Size;
                WriteAddr += AT24CXX_Page_Size;
            }
            if (NumOfSingle != 0)
            {
                AT24CXX_Write_Bytes(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
    unsigned char Page = 0, WordAddress = 0, DeviceAddress = 0x50;
    Page = ReadAddr / AT24CXX_Page_Size;
    WordAddress = (ReadAddr % AT24CXX_Page_Size) & 0x0F;
    DeviceAddress |= (((Page << 1) & 0xE0) >> 4); // High 3 bits
    WordAddress |= (Page & 0x0F) << 4;            // Low 4 bits
    while (NumToRead)
    {
        *pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}

#endif

//u8 temp;
//void AT24CXX_Check(void)
//{

//    AT24CXX_WriteOneByte(1, 0X55);
//    temp = AT24CXX_ReadOneByte(1);
//    if (temp == 0X55)
//    {
//        printf("EPROM OK \r\n");
//    }
//    else
//    {
//        printf("EPROM ERR\r\n");
//    }
//}
