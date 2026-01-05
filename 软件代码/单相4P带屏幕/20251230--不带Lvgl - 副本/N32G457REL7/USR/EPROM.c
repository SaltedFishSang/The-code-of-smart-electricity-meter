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

// u8 temp;
// void AT24CXX_Check(void)
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
#define Modbus_DIAddressBegin 4 // ModbuS  DI 寄存器器起始地址 长度 20 预留了20个空间来存放DI
#define Modbus_DILength 5       // 目前Modbus DI 寄存器个数
/*!
    \brief      Eprom_WirteModbusDI
    \param[in]  address DI寄存器地址  value:写入值
    \param[out] none
    \retval     none
*/
void Eprom_WirteModbusDI(u8 address, u8 value)
{
    AT24CXX_WriteOneByte(Modbus_DIAddressBegin + address, value); // EPROM起始地址4  从地址4开始 20个地址空间全用来存DI  4 -23
}

/*!
    \brief      Eprom_ReadModbusDI  从EPROM中读取DI数据 存放到DI数据里面
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Eprom_ReadModbusDI(void)
{
    u8 i = 0;
    u8 value;
    for (i = 0; i < Modbus_DILength; i++)
    {
        value = AT24CXX_ReadOneByte(Modbus_DIAddressBegin + i);
        if (value == 1)
        {
            Modbus_WirteDI(i, 1);
        }
        if (value == 0)
        {
            Modbus_WirteDI(i, 0);
        }
    }
    /*继电器上电初始化*/
    Relay_Control(Modbus_ReadDI(1));

#if _Printf_DBG
    printf("DI[%d]= %d \r\n", i, value);
    printf("锁扣开关：0x%x\r\n", Modbus_ReadDI(2));
    printf("分合闸开关：0x%x\r\n", Modbus_ReadDI(1));
    printf("定时开关：0x%x\r\n", Modbus_ReadDI(4));
#endif
}
/*!
    \brief      Eprom_WirteResetModbusDI
    \param[in]  void
    \param[out] none
    \retval     none
*/
void Eprom_WirteResetModbusDI(void)
{
    AT24CXX_WriteOneByte(Modbus_DIAddressBegin + 0, 0);
    AT24CXX_WriteOneByte(Modbus_DIAddressBegin + 1, 1); // 合闸/分闸 默认分闸
    AT24CXX_WriteOneByte(Modbus_DIAddressBegin + 2, 0); // 锁扣：本地按钮失效   默认不锁扣
    AT24CXX_WriteOneByte(Modbus_DIAddressBegin + 3, 1); // 预留
    AT24CXX_WriteOneByte(Modbus_DIAddressBegin + 4, 0); // 定时开关
}
#define Modbus_RemianAddressBegin 23 // ModbuS  remain 寄存器器起始地址 长度 51（预留）
#define Modbus_RemainLength 33       // 目前Modbus remain寄存器个数 33个
/*!
    \brief      Eprom_WirteModbusRemain
    \param[in]  address remain寄存器起始地址  value:写入值  num :写入个数
    \param[out] none
    \retval     none
*/
void Eprom_WirteModbusRemain(u8 address, u8 num)
{
    u8 i = 0;
    for (i = 0; i < num; i++)
    {
        AT24CXX_WriteOneByte(23 + address * 2 + 2 * i, (Modbus_Readremain(i + address) >> 8)); // EPROM起始地址23  从地址23开始 120个地址空间全用来存remain 23 142
        AT24CXX_WriteOneByte(23 + address * 2 + 2 * i + 1, (Modbus_Readremain(i + address) & 0xff));
    }
}

/*!
    \brief      Eprom_ReadModbusRemain
    \param[in]  address remain寄存器起始地址  value:写入值  num :写入个数
    \param[out] none
    \retval     none
*/
void Eprom_ReadModbusRemain(void)
{
    u8 i = 0;
    u16 value = 0;
    for (i = 0; i < Modbus_RemainLength; i++)
    {
        value = (AT24CXX_ReadOneByte(Modbus_RemianAddressBegin + i * 2) << 8) + AT24CXX_ReadOneByte(Modbus_RemianAddressBegin + i * 2 + 1);
        Modbus_Wrietemain(i, value);
    }
#if _Printf_DBG
    printf("过压阈值==%d\r\n", Modbus_Readremain(3));
    printf("欠压阈值==%d\r\n", Modbus_Readremain(4));
    printf("过流阈值==%dmA\r\n", Modbus_Readremain(5));
    printf("漏电流阈值==%dmA\r\n", Modbus_Readremain(6));
    u16 data = 0;
    data = Modbus_Readremain(9);
    if (data & 0x01)
    {
        printf("过压跳闸开关打开\r\n");
    }
    if (data & 0x02)
    {
        printf("欠压跳闸开关打开\r\n");
    }
    if (data & 0x04)
    {
        printf("过流跳闸开关打开\r\n");
    }
    if (data & 0x08)
    {
        printf("漏电跳闸开关打开\r\n");
    }

#endif
}
/*!
    \brief      Eprom_WirteResetModbusRemain
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Eprom_WirteResetModbusRemain(void)
{
    // Eprom_ReadModbusRemain 保持寄存器的起始地址为23
    AT24CXX_WriteOneByte(23, 0);
    AT24CXX_WriteOneByte(24, 1); // 设备地址01

    AT24CXX_WriteOneByte(25, 0);
    AT24CXX_WriteOneByte(26, 0);
    AT24CXX_WriteOneByte(27, 0x25);
    AT24CXX_WriteOneByte(28, 0x80); // 波特率 默认9600

    AT24CXX_WriteOneByte(29, 0x01);
    AT24CXX_WriteOneByte(30, 0x0E); // 过压值 270

    AT24CXX_WriteOneByte(31, 0x00);
    AT24CXX_WriteOneByte(32, 0xB4); // 欠压值 180

    AT24CXX_WriteOneByte(33, 0x1F);
    AT24CXX_WriteOneByte(34, 0x40); // 过流值8000 这个根据空开的额定电流32A 63A 80A

    AT24CXX_WriteOneByte(35, 0x00);
    AT24CXX_WriteOneByte(36, 30); // 漏电值30ma

    AT24CXX_WriteOneByte(37, 0x00);
    AT24CXX_WriteOneByte(38, 80); // 接口温度值80

    AT24CXX_WriteOneByte(39, 0x44);
    AT24CXX_WriteOneByte(40, 0xC0); // 过载有功功率

    AT24CXX_WriteOneByte(41, 0);
    AT24CXX_WriteOneByte(42, 0x3F); // 跳闸开关

    AT24CXX_WriteOneByte(49, 0);
    AT24CXX_WriteOneByte(50, 2); // 默认两个点

    AT24CXX_WriteOneByte(51, 0);
    AT24CXX_WriteOneByte(52, 0xFF); // 默认第一个点 周一到周日都打开
    AT24CXX_WriteOneByte(53, 0);
    AT24CXX_WriteOneByte(54, 0); // 默认第一个点 开始时间 0点0分

    AT24CXX_WriteOneByte(55, 0);
    AT24CXX_WriteOneByte(56, 0x7F); // 默认第2个点 周一到周日都关闭
    AT24CXX_WriteOneByte(57, 23);
    AT24CXX_WriteOneByte(58, 59); // 默认第2个点 开始时间 23点59分
}
/*********************************给设备写入累计电能 143-146********************************/
/*!
    \brief      Eprom_write_Energy ：Write Energy
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Eprom_write_Energy(void) // 写入电能 被放大1000倍写入
{
    AT24CXX_WriteOneByte(143, BL0942_EleData.energy >> 24); // 写入累计电能
    AT24CXX_WriteOneByte(144, BL0942_EleData.energy >> 16); // 写入累计电能
    AT24CXX_WriteOneByte(145, BL0942_EleData.energy >> 8);  // 写入累计电能
    AT24CXX_WriteOneByte(146, BL0942_EleData.energy);       // 写入累计电能
}
/*********************************从设备读出累计电能 90-93********************************/
/*!
     \brief      Eprom_read_Energy:read Energy
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Eprom_read_Energy(void)
{
    BL0942_EleData.energy = (AT24CXX_ReadOneByte(143) << 24) +
                            (AT24CXX_ReadOneByte(144) << 16) +
                            (AT24CXX_ReadOneByte(145) << 8) +
                            (AT24CXX_ReadOneByte(146));
    Modbus_WrieteInput(8, (BL0942_EleData.energy >> 16));
    Modbus_WrieteInput(9, (BL0942_EleData.energy & 0xffff));
}
/*!
    \brief      Eprom_write_resetEnergy ：Write Energy 0
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Eprom_write_resetEnergy(void) // 写入电能 清除累计电能
{
    AT24CXX_WriteOneByte(143, 0);
    AT24CXX_WriteOneByte(144, 0);
    AT24CXX_WriteOneByte(145, 0);
    AT24CXX_WriteOneByte(146, 0);
    Eprom_read_Energy();
}

void Eprom_paraInit(void)
{
    unsigned char table[4];
    unsigned char i = 0;
    for (i = 0; i < 4; i++)
        table[i] = AT24CXX_ReadOneByte(i);
#if _Printf_DBG
    printf("0x%x 0x%x 0x%x 0x%x ", table[0], table[1], table[2], table[3]);
#endif
    if ((table[0] == 0xa1) && (table[1] == 0x58) && (table[2] == 0x59) && (table[3] == 0xa2))
    {
#if _Printf_DBG
        printf("Read eprom parameters\r\n");
#endif
        Eprom_read_Energy();
        Eprom_ReadModbusDI();
        Eprom_ReadModbusRemain();
    }
    else
    {
#if _Printf_DBG
        printf("This is a new EPROM waiting to write default parameters\r\n");
#endif

        /*************** a1 58 59 a2**********************/
        AT24CXX_WriteOneByte(0, 0xa1);
        AT24CXX_WriteOneByte(1, 0x58);
        AT24CXX_WriteOneByte(2, 0x59);
        AT24CXX_WriteOneByte(3, 0xa2);
        Eprom_write_resetEnergy();
        Eprom_WirteResetModbusDI();
        Eprom_WirteResetModbusRemain();
        Eprom_read_Energy();
        Eprom_ReadModbusDI();
        Eprom_ReadModbusRemain();
    }
}

/*!
    \brief     sysparainit:Equipment restoration to factory settings parameters
               设备参数恢复出厂设置
    \param[in]  none
    \param[out] none
    \retval     int
*/
void sysparainit(void)
{
#if _Printf_DBG
    printf("Restore factory parameters\r\n");
#endif
    Eprom_write_resetEnergy();
    Eprom_WirteResetModbusDI();
    Eprom_WirteResetModbusRemain();
}
