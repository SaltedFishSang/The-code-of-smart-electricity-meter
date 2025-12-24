#include "BSP.h"
#define Flash_DEBUG 1
#define FLASH_TEST_ADDRESS 0x1020000
#define BUFFER_SIZE 47
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t m_buffer[BUFFER_SIZE];
void Flash_ReadModbusDI(void);
void Flash_WirteResetModbusDI(void);
void Flash_WirteResetData(void);
void Flash_ReadModbusRemain(void);
void Flash_paraInit(void)
{
    uint8_t table[4];
    Qflash_Init();
    Qflash_Read(FLASH_TEST_ADDRESS, table, 4);
#if Flash_DEBUG
    printf("0x%x 0x%x 0x%x 0x%x ", table[0], table[1], table[2], table[3]);
#endif
    if ((table[0] == 0xa1) && (table[1] == 0x58) && (table[2] == 0x59) && (table[3] == 0xa2))
    {
#if Flash_DEBUG
        printf("Read flash parameters\r\n");
#endif
        //Flash_WirteResetData();
        Flash_ReadModbusDI();
        Flash_ReadModbusRemain();
    }
    else
    {
        Flash_WirteResetData();
    }
}

/*!
    \brief      Flash_WirteModbusDI
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Flash_WirteModbusDI(void)
{
    Qflash_Read(FLASH_TEST_ADDRESS, m_buffer, 47);
    m_buffer[4] = Modbus_ReadDI(1);
    m_buffer[5] = Modbus_ReadDI(2);
    m_buffer[6] = Modbus_ReadDI(3);
    Qflash_Erase_Sector(FLASH_TEST_ADDRESS);
    Qflash_Write(FLASH_TEST_ADDRESS, m_buffer, 47);
}

/*!
    \brief      Flash_ReadModbusDI  从Flash中读取DI数据 存放到DI数据里面
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Flash_ReadModbusDI(void)
{
    Qflash_Read(FLASH_TEST_ADDRESS, m_buffer, 43);
    Modbus_WirteDI(1, m_buffer[4]);
    Modbus_WirteDI(2, m_buffer[5]);
    Modbus_WirteDI(3, m_buffer[6]);
#if Flash_DEBUG
    printf("分合闸开关：0x%x\r\n", Modbus_ReadDI(1));
    printf("本地使能开关：0x%x\r\n", Modbus_ReadDI(2));
    printf("定时开关：0x%x\r\n", Modbus_ReadDI(3));
#endif
}
/*!
    \brief      Flash_WirteModbusRemain
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Flash_WirteModbusRemain(void)
{
    uint8_t i = 0;
    Qflash_Read(FLASH_TEST_ADDRESS, m_buffer, 47);
    m_buffer[7] = (Modbus_Readremain(3) >> 8) & 0xff; // 过压
    m_buffer[8] = Modbus_Readremain(3) & 0xff;

    m_buffer[9] = (Modbus_Readremain(4) >> 8) & 0xff; // 欠压
    m_buffer[10] = Modbus_Readremain(4) & 0xff;

    m_buffer[11] = (Modbus_Readremain(5) >> 8) & 0xff; // 过流
    m_buffer[12] = Modbus_Readremain(5) & 0xff;

    m_buffer[13] = (Modbus_Readremain(8) >> 8) & 0xff; // 过载
    m_buffer[14] = Modbus_Readremain(8) & 0xff;

    m_buffer[15] = Modbus_Readremain(9) & 0xff; // 跳闸开关

    m_buffer[16] = Modbus_Readremain(13) & 0xff; // 定时点个数

    for (i = 0; i < 10; i++)
    {
        m_buffer[17 + i * 3] = Modbus_Readremain(14 + i * 2) & 0xff;        // 定时点1：星期/开关
        m_buffer[18 + i * 3] = (Modbus_Readremain(15 + i * 2) >> 8) & 0xff; // 定时点1：时
        m_buffer[19 + i * 3] = Modbus_Readremain(15 + i * 2) & 0xff;        // 定时点1：分
    }
    Qflash_Erase_Sector(FLASH_TEST_ADDRESS);
    Qflash_Write(FLASH_TEST_ADDRESS, m_buffer, 47);
}

/*!
    \brief      Flash_ReadModbusRemain
    \param[in]  address remain寄存器起始地址  value:写入值  num :写入个数
    \param[out] none
    \retval     none
*/
void Flash_ReadModbusRemain(void)
{
    uint8_t i = 0;
    Qflash_Read(FLASH_TEST_ADDRESS, m_buffer, 47);
    Modbus_Wrietemain(3, (m_buffer[7] << 8) + m_buffer[8]);
    Modbus_Wrietemain(4, (m_buffer[9] << 8) + m_buffer[10]);
    Modbus_Wrietemain(5, (m_buffer[11] << 8) + m_buffer[12]);
    Modbus_Wrietemain(8, (m_buffer[13] << 8) + m_buffer[14]);
    Modbus_Wrietemain(9, m_buffer[15]);
    Modbus_Wrietemain(13, m_buffer[16]);
    for (i = 0; i < 10; i++)
    {
        Modbus_Wrietemain(14 + 2 * i, m_buffer[17 + 3 * i]);
        Modbus_Wrietemain(15 + 2 * i, (m_buffer[18 + 3 * i] << 8) + m_buffer[19 + 3 * i]);
    }
#if Flash_DEBUG
    printf("过压值：%d\r\n", Modbus_Readremain(3));
    printf("欠压值：%d\r\n", Modbus_Readremain(4));
    printf("过流值：%d\r\n", Modbus_Readremain(5));
    printf("过载功率：%d\r\n", Modbus_Readremain(8));
    printf("跳闸开关：0x%x\r\n", Modbus_Readremain(9));
    if (Modbus_Readremain(9) & (0x01))
    {
        printf("过压跳闸 开\r\n");
    }
    else
    {
        printf("过压跳闸 关\r\n");
    }
    if (Modbus_Readremain(9) & (0x02))
    {
        printf("欠压跳闸 开\r\n");
    }
    else
    {
        printf("欠压跳闸 关\r\n");
    }
    if (Modbus_Readremain(9) & (0x04))
    {
        printf("过流跳闸 开\r\n");
    }
    else
    {
        printf("过流跳闸 关\r\n");
    }
    if (Modbus_Readremain(9) & (0x08))
    {
        printf("过载跳闸 开\r\n");
    }
    else
    {
        printf("过载跳闸 关\r\n");
    }
    printf("定时点数：%d\r\n", Modbus_Readremain(13));

    for (i = 0; i < 10; i++)
    {
        printf("定时点%d 时：%d  分: %d   ", i + 1, (Modbus_Readremain(15 + i * 2) >> 8) & 0xFF, Modbus_Readremain(15 + i * 2) & 0xFF);
        if (Modbus_Readremain(14 + 2 * i) & (0x80))
        {
            printf("开 ");
        }
        else
        {
            printf("关 ");
        }
        printf("0x%x\r\n", Modbus_Readremain(14 + 2 * i));
    }

    for (i = 0; i < 47; i++)
    {
        printf("0x%x ", m_buffer[i]);
    }
#endif
}
/*!
    \brief      Flash_WirteResetData 在Flash里面写入默认参数
    \param[in]  void
    \param[out] none
    \retval     none
*/
void Flash_WirteResetData(void)
{
    uint8_t flashwrite_data[23];
#if Flash_DEBUG
    printf("This is a new Flash waiting to write default parameters\r\n");
#endif
    flashwrite_data[0] = 0xa1;
    flashwrite_data[1] = 0x58;
    flashwrite_data[2] = 0x59;
    flashwrite_data[3] = 0xa2;

    flashwrite_data[4] = 0x01; // 分合闸开关 默认合闸
    flashwrite_data[5] = 0x01; // 本地使能开关 默认打开
    flashwrite_data[6] = 0x00; // 定时开关关闭

    flashwrite_data[7] = 0x01; // 过压值 270
    flashwrite_data[8] = 0x0E;

    flashwrite_data[9] = 0x00; // 欠压值 180
    flashwrite_data[10] = 0xB4;

    flashwrite_data[11] = 0x1F; // 过流值 8000
    flashwrite_data[12] = 0x40;

    flashwrite_data[13] = 0x44; // 过载有功功率
    flashwrite_data[14] = 0xC0;

    flashwrite_data[15] = 0x0F; // 跳闸开关

    flashwrite_data[16] = 0x02; // 默认两个定时点

    flashwrite_data[17] = 0xFF; // 默认一个点 周一到周日都打开
    flashwrite_data[18] = 0x0;  // 默认一个点 开始时间 0点0分
    flashwrite_data[19] = 0x0;

    flashwrite_data[20] = 0x7F; // 默认2个点 周一到周日都关闭
    flashwrite_data[21] = 23;   // 默认2个点 开始时间 23点59分
    flashwrite_data[22] = 59;
    Qflash_Erase_Sector(FLASH_TEST_ADDRESS);
    Qflash_Write(FLASH_TEST_ADDRESS, flashwrite_data, 23);
}
