#include "BSP.h"
u16 m_DiState = 0;
u16 m_remain_table[51] = {0, 9600}; // 保存寄存器
u16 m_input_table[39];              // 输入寄存器
u8 g_modbus_addr = 0x01;            // 设备地址默认 0x01
unsigned char CRCL;
unsigned char CRCH;
/* Table Of CRC Values for high-order byte */
static unsigned char auchCRCHi[] =
    {
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40};
/* Table of CRC values for low-order byte */
static unsigned char auchCRCLo[] =
    {
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
        0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
        0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
        0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
        0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
        0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
        0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
        0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
        0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
        0x40};
void crc16(unsigned char *puchMsg, UINT16 usDataLen) // puchMsg message to calculate CRC upon
{
    // usDataLen quantity of bytes in message
    unsigned char uchCRCHi = 0xFF; // high byte of CRC initialized
    unsigned char uchCRCLo = 0xFF; // low byte of CRC initialized
    unsigned int uIndex;           // will index into CRC lookup table
    while (usDataLen--)            // Pass through message buffer
    {
        uIndex = uchCRCHi ^ *puchMsg++; // calculate the CRC
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex];
    }
    CRCL = uchCRCLo;
    CRCH = uchCRCHi;
}

/*!
    \brief      Modbus_WirteDI  //写DI寄存器 线圈寄存器
    \param[in]  num  寄存器地址 bit0-15
    \param[out] value 值是 0 还是 1
    \retval     int
*/
void Modbus_WirteDI(u8 num, u8 value)
{
    if (num <= 15)
    {
        if (value == 1)
        {
            m_DiState |= (1 << num);
        }
        if (value == 0)
        {
            m_DiState &= ~(1 << num);
        }
    }
    /*这里每次更新将数据写入存储*/
}

/*!
    \brief      ReadDI  //读DI寄存器 线圈寄存器
    \param[in]  num  寄存器地址 bit0-15
    \param[out] value 值是 0 还是 1
    \retval     int
*/
u8 Modbus_ReadDI(u8 num)
{
    if (num <= 15)
    {
        if ((m_DiState & (1 << num)) == 0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

/*!
    \brief      Modbus_Readremain  //读保持寄存器
    \param[in]  num  寄存器地址 0-50
    \param[out] value u16
    \retval     int
*/
u16 Modbus_Readremain(u8 address)
{
    return m_remain_table[address];
}

/*!
    \brief      Modbus_Wrietemain  //读保持寄存器
    \param[in]  address  起始地址寄存器地址 0-50  value 写入的值
    \param[out] value u16
    \retval     int
*/
void Modbus_Wrietemain(u8 address, u16 value)
{
    m_remain_table[address] = value;
}

/*!
    \brief      Modbus_Wrieteinput  //更新输入寄存器的值
    \param[in]  address  起始地址寄存器地址 0-39  value 写入的值
    \param[out] value u16
    \retval     int
*/
void Modbus_WrieteInput(u8 address, u16 value)
{
    m_input_table[address] = value;
}

/*!
    \brief      Modbus_Readinput  //读取输入寄存器的值
    \param[in]  address  起始地址寄存器地址 0-39
    \param[out] value u16
    \retval     int
*/
u16 Modbus_Readinput(u8 address)
{
    return m_input_table[address];
}

/*!
    \brief      Modbus Send All
    \param[in]  none
    \param[out] none
    \retval     int
*/
void ModbusSendAll(void)
{
    u8 modbusdata[500];
    u8 modbusdatalen = 0;
    (void)modbusdatalen;
    u8 i;
    /*先拼DI*/
    modbusdata[0] = m_remain_table[0]; // 设备地址
    modbusdata[1] = 0x01;              // 读DI数据命令
    modbusdata[2] = 0x01;              // DI数据长度
    modbusdata[3] = m_DiState & 0xff;  // DI数据
    crc16(modbusdata, 4);
    modbusdata[4] = CRCH;
    modbusdata[5] = CRCL;

    /*拼接保持寄存器*/
    modbusdata[6] = m_remain_table[0]; // 设备地址
    modbusdata[7] = 0x03;              // 读保持寄存器长度
    modbusdata[8] = 68;                // 数据长度
    for (i = 0; i <= 34; i++)
    {
        modbusdata[9 + i * 2] = m_remain_table[i] >> 8;
        modbusdata[10 + i * 2] = m_remain_table[i];
    }
    crc16(&modbusdata[6], 71);
    modbusdata[77] = CRCH;
    modbusdata[78] = CRCL;

    /*拼接输入寄存器*/
    modbusdata[79] = m_remain_table[0]; // 设备地址
    modbusdata[80] = 0x04;              // 读输入寄存器长度
    modbusdata[81] = 28;                // 数据长度
    for (i = 0; i <= 14; i++)
    {
        modbusdata[82 + i * 2] = m_input_table[i] >> 8;
        modbusdata[83 + i * 2] = m_input_table[i];
    }
    crc16(&modbusdata[81], 31);
    modbusdata[110] = CRCH;
    modbusdata[111] = CRCL;
    modbusdatalen = 112;
}
// #define UsestandMobus 0
// /*!
//     \brief      Modbus 数据处理
//     \param[in]  none
//     \param[out] none
//     \retval     int
// */
// void ModbusDataHandle(void)
// {
//     u8 modbusaddr = 0;
//     u8 modbusfun = 0;
//     u16 modbusregadd = 0;
//     u16 modbusregdata = 0;
//     u8 i = 0;
//     u8 modbusdata[100];
//     u8 modbusdatalen = 0;
//     u8 flag = 0;
//     if (Uart1_UartStaus())
//     {
//         if (Uart1.RxLen >= 2)
//         {
//             crc16(Uart1.RxBuf, Uart1.RxLen - 2); // modbus包CRC校验
// #if _Printf_DBG
//             printf("CRC High：0x%02x\r\n", CRCH);
//             printf("CRC Low：0x%02x\r\n", CRCL);
// #endif
//             /*先CRC校验*/
//             if ((Uart1.RxBuf[Uart1.RxLen - 1] == CRCL) && (Uart1.RxBuf[Uart1.RxLen - 2] == CRCH))
//             {
//                 modbusaddr = Uart1.RxBuf[0];                           // 设备地址
//                 modbusfun = Uart1.RxBuf[1];                            // 功能码
//                 modbusregadd = (Uart1.RxBuf[2] << 8) + Uart1.RxBuf[3]; // 寄存器起始地址

// #if _Printf_DBG
//                 printf("MODBUS:\r\n");
//                 printf("设备地址：0x%02X:\r\n", modbusaddr);
//                 printf("功能码：0x%02X:\r\n", modbusfun);
//                 printf("寄存器起始地址：0x%04X:\r\n", modbusregadd);
// #endif
//                 if (modbusaddr == m_remain_table[0]) // 比对设备地址
//                 {
//                     switch (modbusfun)
//                     {
//                     case 0x01:
//                         if (modbusregadd <= 4) // 查询起始地址0-4
//                         {
//                             modbusregdata = (Uart1.RxBuf[4] << 8) + Uart1.RxBuf[5]; // 寄存器值 读线圈操作
// #if _Printf_DBG
//                             printf("读取线圈数量：0x%04X:\r\n", modbusregdata);
// #endif
//                             if ((modbusregdata + modbusregadd) <= 5) // 查询长度不超过5个
//                             {
//                                 modbusdata[0] = modbusaddr;
//                                 modbusdata[1] = modbusfun;
//                                 if ((modbusregdata / 8) == 0)
//                                 {
//                                     modbusdata[2] = 1;
//                                 }
//                                 else
//                                 {
//                                     if ((modbusregdata % 8) == 0)
//                                     {
//                                         modbusdata[2] = (modbusregdata / 8);
//                                     }
//                                     else
//                                     {
//                                         modbusdata[2] = (modbusregdata / 8) + 1;
//                                     }
//                                 }
//                                 for (i = 0; i < modbusdata[2]; i++)
//                                 {
//                                     modbusdata[3 + i] = ((m_DiState >> modbusregadd) >> (8 * i));
//                                 }
//                                 modbusdatalen = 3 + modbusdata[2] + 2;
//                                 crc16(modbusdata, modbusdatalen - 2);
//                                 modbusdata[modbusdatalen - 2] = CRCH;
//                                 modbusdata[modbusdatalen - 1] = CRCL;
// #if UsestandMobus
//                                 RS458Send(modbusdata, modbusdatalen);
// #else
//                                 ModbusSendAll();
// #endif
//                             }
//                         }
//                         break;
//                     case 0x05:
//                         modbusregdata = (Uart1.RxBuf[4] << 8) + Uart1.RxBuf[5]; // 输出线圈操作
// #if _Printf_DBG
//                         printf("控制DO：0x%04X:\r\n", modbusregdata);
// #endif
//                         if (modbusregadd <= 7)
//                         {
//                             modbusdata[0] = modbusaddr;
//                             modbusdata[1] = modbusfun;
//                             modbusdata[2] = modbusregadd >> 8;
//                             modbusdata[3] = modbusregadd;
//                             modbusdata[4] = modbusregdata >> 8;
//                             modbusdata[5] = modbusregdata;
//                             modbusdatalen = 8;
//                             crc16(modbusdata, modbusdatalen - 2);
//                             modbusdata[modbusdatalen - 2] = CRCH;
//                             modbusdata[modbusdatalen - 1] = CRCL;
//                             switch (modbusregadd)
//                             {
//                             case 0x0000: // 配置复位
//                                 if (modbusregdata == 0xFF00)
//                                 {
//                                     //  sysparainit();
//                                     // NVIC_SystemReset();
//                                 }
//                                 break;
//                             case 0x0001:                   // 远程合闸 分闸
//                                 if (Modbus_ReadDI(2) == 0) // 远程没有锁住  本地的按钮按下也可以操作这个按钮
//                                 {
//                                     // 关闭定时
//                                     if (Modbus_ReadDI(3) == 1) // 自动模式下 才可以远程控制
//                                     {
//                                         Modbus_WirteDI(4, 0);
//                                         Eprom_WirteModbusDI(4, 0); // 关闭定时

//                                         if (modbusregdata == 0xFF00)
//                                         {
//                                             if (Modbus_ReadDI(0) == 0) // 有故障必须消了才能合闸
//                                             {
//                                                 Modbus_WirteDI(1, 1); // 合闸
//                                                 Eprom_WirteModbusDI(1, 1);
//                                                 flag = 1;
// #if UsestandMobus
//                                                 RS458Send(modbusdata, modbusdatalen);
// #else
//                                                 ModbusSendAll();
// #endif
//                                                 //    Switch_Close();
//                                             }
//                                         }
//                                         if (modbusregdata == 0x0000)
//                                         {
//                                             Modbus_WirteDI(1, 0); // 分闸
//                                             Eprom_WirteModbusDI(1, 0);
//                                             Modbus_WrieteInput(10, 0x0A); // 更新最后一次分闸原因 远程
//                                             flag = 1;
// #if UsestandMobus
//                                             RS458Send(modbusdata, modbusdatalen);
// #else
//                                             ModbusSendAll();
// #endif
//                                             //  Switch_Open();
//                                         }
//                                     }
//                                 }
//                                 break;
//                             case 0x0002: // 远程锁扣/解锁
//                                 if (modbusregdata == 0xFF00)
//                                 {
//                                     Modbus_WirteDI(2, 1); // 锁扣
//                                     Eprom_WirteModbusDI(2, 1);
//                                     Switch_OpenLock();    // 锁闸
//                                     Modbus_WirteDI(1, 0); // 分闸
//                                     Eprom_WirteModbusDI(1, 0);
//                                     Modbus_WrieteInput(10, 0x0A); // 更新最后一次分闸原因 远程
//                                 }
//                                 if (modbusregdata == 0x0000)
//                                 {
//                                     Modbus_WirteDI(2, 0); // 脱扣
//                                     Eprom_WirteModbusDI(2, 0);
//                                     Switch_UnLock(); // 解锁
//                                 }
//                                 break;
//                             case 0x0004: // 电量清零
//                                 if (modbusregdata == 0xFF00)
//                                 {
//                                     /*放入电量清0及接口函数*/
//                                     // 变量也要清0
//                                     Eprom_write_resetEnergy();
//                                 }
//                                 break;
//                             case 0x0005: // 漏电测试按钮
//                             {
//                             }
//                             break;
//                             case 0x0006: // 故障消除
//                             {
//                                 if (modbusregdata == 0xFF00)
//                                 {
//                                     Modbus_WirteDI(0, 0);      // 清除故障
//                                     Modbus_WrieteInput(11, 0); // 清除故障原因
//                                 }
//                             }
//                             break;
//                             case 0x0007: // 定时开关
//                             {
//                                 if (modbusregdata == 0xFF00)
//                                 {
//                                     Modbus_WirteDI(4, 1);
//                                     Eprom_WirteModbusDI(4, 1);
//                                 }
//                                 if (modbusregdata == 0x0000)
//                                 {
//                                     Modbus_WirteDI(4, 0);
//                                     Eprom_WirteModbusDI(4, 0);
//                                 }
//                             }
//                             break;
//                             default:
//                                 break;
//                             }
// #if UsestandMobus
//                             if (flag == 0)
//                             {
//                                 RS458Send(modbusdata, modbusdatalen);
//                             }
// #else
//                             if (flag == 0)
//                             {
//                                 ModbusSendAll();
//                             }
// #endif
//                             if (flag == 1)
//                             {
//                                 flag = 0;
//                             }
//                         }
//                         break;
//                     case 0x06: // 单写保持寄存器
//                         modbusregdata = (Uart1.RxBuf[4] << 8) + Uart1.RxBuf[5];
// #if _Printf_DBG
//                         printf("单写保持寄存器 ：0x%04X:\r\n", modbusregdata);
// #endif
//                         if (modbusregadd <= 33)
//                         {
//                             modbusdata[0] = modbusaddr;
//                             modbusdata[1] = modbusfun;
//                             modbusdata[2] = modbusregadd >> 8;
//                             modbusdata[3] = modbusregadd;
//                             modbusdata[4] = modbusregdata >> 8;
//                             modbusdata[5] = modbusregdata;
//                             modbusdatalen = 8;
//                             crc16(modbusdata, modbusdatalen - 2);
//                             modbusdata[modbusdatalen - 2] = CRCH;
//                             modbusdata[modbusdatalen - 1] = CRCL;
// #if UsestandMobus
//                             RS458Send(modbusdata, modbusdatalen);
// #else
//                             ModbusSendAll();
// #endif
//                             m_remain_table[modbusregadd] = modbusregdata;
//                             Eprom_WirteModbusRemain(modbusregadd & 0xff, 1);
//                         }
//                         break;
//                     case 0x10: // 多写保持寄存器
//                         modbusregdata = (Uart1.RxBuf[4] << 8) + Uart1.RxBuf[5];
// #if _Printf_DBG
//                         printf("多写保持寄存器个数 ：0x%04X:\r\n", modbusregdata);
// #endif
//                         if (modbusregadd <= 33)
//                         {
//                             if ((modbusregadd + modbusregdata) <= 34)
//                             {
//                                 for (i = 0; i < modbusregdata; i++)
//                                 {
//                                     m_remain_table[i + modbusregadd] = (Uart1.RxBuf[7 + i * 2] << 8) + Uart1.RxBuf[8 + i * 2];
//                                 }
//                                 modbusdata[0] = modbusaddr;
//                                 modbusdata[1] = modbusfun;
//                                 modbusdata[2] = modbusregadd >> 8;
//                                 modbusdata[3] = modbusregadd;
//                                 modbusdata[4] = modbusregdata >> 8;
//                                 modbusdata[5] = modbusregdata;
//                                 modbusdata[6] = Uart2.RxBuf[6];
//                                 modbusdatalen = 9;
//                                 crc16(modbusdata, modbusdatalen - 2);
//                                 modbusdata[modbusdatalen - 2] = CRCH;
//                                 modbusdata[modbusdatalen - 1] = CRCL;
// #if UsestandMobus
//                                 RS458Send(modbusdata, modbusdatalen);
// #else
//                                 ModbusSendAll();
// #endif
//                                 Eprom_WirteModbusRemain(modbusregadd, modbusregdata);

//                                 if (modbusregadd == 0x000a) // 设置时间
//                                 {
//                                     RTC_Set(Modbus_Readremain(10), (Modbus_Readremain(11) >> 8), Modbus_Readremain(11) & 0xff, (Modbus_Readremain(12) >> 8), Modbus_Readremain(12) & 0xff, 0);
//                                 }
//                             }
//                         }
//                         break;
//                     case 0x03: // 多读多个保持寄存器
//                         modbusregdata = (Uart1.RxBuf[4] << 8) + Uart1.RxBuf[5];
// #if _Printf_DBG
//                         printf("多读保持寄存器个数 ：0x%04X:\r\n", modbusregdata);
// #endif
//                         if (modbusregadd <= 33)
//                         {
//                             if ((modbusregdata + modbusregadd) <= 34)
//                             {
//                                 modbusdata[0] = modbusaddr;
//                                 modbusdata[1] = modbusfun;
//                                 modbusdata[2] = (modbusregdata * 2);
//                                 for (i = 0; i <= modbusregdata; i++)
//                                 {
//                                     modbusdata[3 + i * 2] = m_remain_table[modbusregadd + i] >> 8;
//                                     modbusdata[4 + i * 2] = m_remain_table[modbusregadd + i];
//                                 }
//                                 modbusdatalen = (5 + (modbusregdata * 2));
//                                 crc16(modbusdata, (modbusdatalen - 2));
//                                 modbusdata[modbusdatalen - 2] = CRCH;
//                                 modbusdata[modbusdatalen - 1] = CRCL;
// #if UsestandMobus
//                                 RS458Send(modbusdata, modbusdatalen);
// #else
//                                 ModbusSendAll();
// #endif
//                             }
//                         }
//                         break;
//                     case 0x04: // 读输入寄存器的值
//                         modbusregdata = (Uart1.RxBuf[4] << 8) + Uart1.RxBuf[5];
// #if _Printf_DBG
//                         printf("读输入寄存器个数 ：0x%04X:\r\n", modbusregdata);
// #endif
//                         if (modbusregadd <= 13)
//                         {
//                             if ((modbusregadd + modbusregdata) <= 14)
//                             {
//                                 modbusdata[0] = modbusaddr;
//                                 modbusdata[1] = modbusfun;
//                                 modbusdata[2] = (modbusregdata * 2);
//                                 for (i = 0; i <= modbusregdata; i++)
//                                 {
//                                     modbusdata[3 + i * 2] = m_input_table[modbusregadd + i] >> 8;
//                                     modbusdata[4 + i * 2] = m_input_table[modbusregadd + i];
//                                 }
//                                 modbusdatalen = (5 + (modbusregdata * 2));
//                                 crc16(modbusdata, (modbusdatalen - 2));
//                                 modbusdata[modbusdatalen - 2] = CRCH;
//                                 modbusdata[modbusdatalen - 1] = CRCL;
// #if UsestandMobus
//                                 RS458Send(modbusdata, modbusdatalen);
// #else
//                                 ModbusSendAll();
// #endif
//                             }
//                         }
//                         break;
//                     default:
//                         break;
//                     }
//                 }

//                 if (modbusaddr == 0x00) // 广播地址
//                 {
//                     modbusregdata = (Uart1.RxBuf[4] << 8) + Uart1.RxBuf[5];
//                     if (modbusfun == 0x05)
//                     {
//                         if (modbusregadd == 0x0001)
//                         {
//                             if (Modbus_ReadDI(2) == 0) // 远程没有锁住  本地的按钮按下也可以操作这个按钮
//                             {
//                                 if (Modbus_ReadDI(3) == 1) // 自动模式下 才可以远程控制
//                                 {
//                                     if (modbusregdata == 0xFF00)
//                                     {
//                                         Modbus_WirteDI(1, 1); // 合闸
//                                         Eprom_WirteModbusDI(1, 1);
//                                         Switch_Close();
//                                     }
//                                     if (modbusregdata == 0x0000)
//                                     {
//                                         Modbus_WirteDI(1, 0); // 分闸
//                                         Eprom_WirteModbusDI(1, 0);
//                                         Switch_Open();
//                                         Modbus_WrieteInput(10, 0x0A); // 更新最后一次分闸原因 远程
//                                     }
//                                 }
//                             }
//                         }
//                     }

//                     if (modbusfun == 0x10)
//                     {
//                         modbusregdata = (Uart1.RxBuf[4] << 8) + Uart1.RxBuf[5];
// #if _Printf_DBG
//                         printf("多写保持寄存器个数 ：0x%04X:\r\n", modbusregdata);
// #endif
//                         if (modbusregadd <= 33)
//                         {
//                             if ((modbusregadd + modbusregdata) <= 34)
//                             {
//                                 for (i = 0; i < modbusregdata; i++)
//                                 {
//                                     m_remain_table[i + modbusregadd] = (Uart1.RxBuf[7 + i * 2] << 8) + Uart1.RxBuf[8 + i * 2];
//                                 }
//                                 modbusdata[0] = modbusaddr;
//                                 modbusdata[1] = modbusfun;
//                                 modbusdata[2] = modbusregadd >> 8;
//                                 modbusdata[3] = modbusregadd;
//                                 modbusdata[4] = modbusregdata >> 8;
//                                 modbusdata[5] = modbusregdata;
//                                 modbusdata[6] = Uart1.RxBuf[6];
//                                 modbusdatalen = 9;
//                                 crc16(modbusdata, modbusdatalen - 2);
//                                 modbusdata[modbusdatalen - 2] = CRCH;
//                                 modbusdata[modbusdatalen - 1] = CRCL;
//                                 // RS458Send(modbusdata, modbusdatalen);
//                                 Eprom_WirteModbusRemain(modbusregadd, modbusregdata);
//                                 if (modbusregadd == 0x000a) // 设置时间
//                                 {
//                                     RTC_Set(Modbus_Readremain(10), (Modbus_Readremain(11) >> 8), Modbus_Readremain(11) & 0xff, (Modbus_Readremain(12) >> 8), Modbus_Readremain(12) & 0xff, 0);
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//         Uart1ClearRxBuf();
//     }
// }
