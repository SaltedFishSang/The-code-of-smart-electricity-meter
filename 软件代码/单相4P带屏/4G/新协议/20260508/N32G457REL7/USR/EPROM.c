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
/*!
     \brief      EEP_Write8:指定地址写入单字节
    \param[in]  addr：存储地址
    \param[in]  data：单字节数据
    \param[out] none
    \retval     none
*/
void EEP_Write8(u16 addr, u8 data)
{
    AT24CXX_WriteOneByte(addr, data);
}
/*!
     \brief      EEP_Read8:从指定地址读出单字节数据
    \param[in]  addr：存储地址
    \param[out] u8:读出单字节数据
    \retval     none
*/
u8 EEP_Read8(u16 addr)
{
    return AT24CXX_ReadOneByte(addr);
}

/*!
     \brief      EEP_Write16:指定地址写入双字节
    \param[in]  addr：存储地址
    \param[in]  data：双字节数据
    \param[out] none
    \retval     none
*/
void EEP_Write16(u16 addr, u16 data)
{
    AT24CXX_WriteOneByte(addr, (data >> 8) & 0xFF);
    AT24CXX_WriteOneByte(addr + 1, data & 0xFF);
}
/*!
     \brief      EEP_Read16:从指定地址读出双字节数据
    \param[in]  addr：存储地址
    \param[out] u16:读出双字节数据
    \retval     none
*/
u16 EEP_Read16(u16 addr)
{
    u16 temp = 0;
    temp |= AT24CXX_ReadOneByte(addr) << 8;
    temp |= AT24CXX_ReadOneByte(addr + 1);
    return temp;
}

/*!
     \brief      EEP_Write32:指定地址写入四字节
    \param[in]  addr：存储地址
    \param[in]  data：四字节数据
    \param[out] none
    \retval     none
*/
void EEP_Write32(u16 addr, u32 data)
{
    AT24CXX_WriteOneByte(addr, (data >> 24) & 0xFF);
    AT24CXX_WriteOneByte(addr + 1, (data >> 16) & 0xFF);
    AT24CXX_WriteOneByte(addr + 2, (data >> 8) & 0xFF);
    AT24CXX_WriteOneByte(addr + 3, data & 0xFF);
}
/*!
     \brief      EEP_Read16:从指定地址读出四字节数据
    \param[in]  addr：存储地址
    \param[out] u32:读出四字节数据
    \retval     none
*/
u32 EEP_Read32(u16 addr)
{
    u32 temp = 0;
    temp |= AT24CXX_ReadOneByte(addr) << 24;
    temp |= AT24CXX_ReadOneByte(addr + 1) << 16;
    temp |= AT24CXX_ReadOneByte(addr + 2) << 8;
    temp |= AT24CXX_ReadOneByte(addr + 3);
    return temp;
}

#define SwitchOnResetValue 1   // 开关默认值
#define SwitchLockResetValue 0 // 锁扣默认值
#define OverVResetValue 270    // 过压默认值
#define UnderVResetValue 180   // 欠压默认值
#define OverCResetValue 80000  // 默认过流值 单位mA
#define OverPResetValue 17600  // 默认过载值 单位w
#define LoseCResetValue 30     // 默认漏电值  单位mA
#define OverTResetValue 80     // m默认过温值

/*!
    \brief      Eprom_write_resetData
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Eprom_write_resetData(void)
{
    EEP_Write8(SwitchOn, SwitchOnResetValue);
    EEP_Write8(SwichLock, SwitchLockResetValue);
    EEP_Write16(OverV, OverVResetValue);
    EEP_Write16(UnderV, UnderVResetValue);
    EEP_Write32(OverC, OverCResetValue);
    EEP_Write32(OverP, OverPResetValue);
    EEP_Write8(LoseC, LoseCResetValue);
    EEP_Write8(OverT, OverTResetValue);
}

/*!
    \brief      Eprom_write_Data
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Eprom_write_Data(void)
{
    EEP_Write8(SwitchOn, Switch_Status.SwitchOnvalue);
    EEP_Write8(SwichLock, Switch_Status.SwitchLockvalue);
    EEP_Write16(OverV, Switch_Status.OverVValue);
    EEP_Write16(UnderV, Switch_Status.UnderVValue);
    EEP_Write32(OverC, Switch_Status.OverCValue);
    EEP_Write32(OverP, Switch_Status.OverPValue);
    EEP_Write8(LoseC, Switch_Status.LoseCValue);
    EEP_Write8(OverT, Switch_Status.OverTValue);
}
/*!
    \brief      Eprom_read_Data
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Eprom_read_Data(void)
{
    Switch_Status.SwitchOnvalue = EEP_Read8(SwitchOn);
    Switch_Status.SwitchLockvalue = EEP_Read8(SwichLock);
    Switch_Status.OverVValue = EEP_Read16(OverV);
    Switch_Status.UnderVValue = EEP_Read16(UnderV);
    Switch_Status.OverCValue = EEP_Read32(OverC);
    Switch_Status.OverPValue = EEP_Read32(OverP);
    Switch_Status.LoseCValue = EEP_Read8(LoseC);
    Switch_Status.OverTValue = EEP_Read8(OverT);
#if EPROM_DBG
    printf("Switch_Status.SwitchOnvalue == %d\r\n", Switch_Status.SwitchOnvalue);
    printf("Switch_Status.SwitchLockvalue == %d\r\n", Switch_Status.SwitchLockvalue);
    printf("Switch_Status.OverVValue == %d\r\n", Switch_Status.OverVValue);
    printf("Switch_Status.UnderVValue == %d\r\n", Switch_Status.UnderVValue);
    printf("Switch_Status.OverCValue == %d\r\n", Switch_Status.OverCValue);
    printf("Switch_Status.OverPValue == %d\r\n", Switch_Status.OverPValue);
    printf("Switch_Status.LoseCValue == %d\r\n", Switch_Status.LoseCValue);
    printf("Switch_Status.OverTValue == %d\r\n", Switch_Status.OverTValue);
#endif
    Relay_Control(Switch_Status.SwitchOnvalue);
}
/*!
    \brief      Eprom_write_Energy ：写入累计电能
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Eprom_write_Energy(void) // 写入电能 被放大1000倍写入
{
    EEP_Write32(Energy, BL0942_EleData.energy);
}

/*!
     \brief      Eprom_read_Energy:读出累计电能
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Eprom_read_Energy(void)
{
    BL0942_EleData.energy = EEP_Read32(Energy);
}
/*!
    \brief      Eprom_write_resetEnergy ：Write Energy 0
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Eprom_write_resetEnergy(void) // 写入电能 清除累计电能
{
    EEP_Write32(Energy, 0);
    Eprom_read_Energy();
}

/*!
     \brief      Eprom_paraInit:EPROM数据初始化
    \param[in]  none
    \param[in]  none
    \param[out] none
    \retval     none
*/
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
        Eprom_write_resetEnergy();
        Eprom_read_Energy();
        Eprom_write_resetData();
        Eprom_read_Data();
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
        Eprom_read_Energy();
        Eprom_write_resetData();
        Eprom_read_Data();

        //
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
    Eprom_write_resetData();
}
