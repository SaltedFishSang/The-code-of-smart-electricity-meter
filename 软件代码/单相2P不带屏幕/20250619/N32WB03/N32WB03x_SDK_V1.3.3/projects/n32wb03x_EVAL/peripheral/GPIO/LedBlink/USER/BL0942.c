#include "BSP.h"
struct _BL0942_EleData_Reg BL0942_EleData_Reg;
struct _BL0942_EleData BL0942_EleData;

#define BL0942_PWR_ON GPIO_ResetBits(GPIOB, GPIO_PIN_3)
#define BL0942_PWR_OFF GPIO_SetBits(GPIOB, GPIO_PIN_3)
#define BL0942_DEBUG 0
u8 bl0942_flag = 0;
void ReadBL0942EleFlag(void)
{
    bl0942_flag = 1;
    Vtimer_SetTimer(VTIM3, 1000, ReadBL0942EleFlag);
}
void BL0942_HardReset(void);
void BL0942ReadData(u8 add);
/*!
    \brief      BL0942_RegInit(void) BL0942寄存器初始化 配置默认参数
    \param[in]  None
    \param[out] none
    \retval     none
*/
void BL0942_RegInit(void)
{
#if BL0942_DEBUG
    printf("BL0942 Reg Init\r\n");
#endif
    BL0942WriteData(Addr_WRPROT, 0x55); // 取消写保护
    Delay_MS(100);
    BL0942WriteData(Addr_MODE, 0xC7);
    Delay_MS(100);
    BL0942WriteData(Addr_WRPROT, 0xAA); // 写保护
    Delay_MS(100);
    BL0942ReadData(Addr_MODE);
}
/*!
    \brief      BL0942_PWRGpioInit(void) BL0942电源控制GPIO口初始化
    \param[in]  None
    \param[out] none
    \retval     none
*/
void BL0942_PWRGpioInit(void)
{
    /*
     *PB3
     */
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
    // BL0942_PWR_ON;
    BL0942_HardReset();
    Delay_MS(1000);
    BL0942_RegInit();
    Vtimer_SetTimer(VTIM3, 1000, ReadBL0942EleFlag);
}
/*!
    \brief      BL0942_HardReset(void) BL0942硬件复位  通过切断BL0942电源再上电
    \param[in]  None
    \param[out] none
    \retval     none
*/
void BL0942_HardReset(void)
{
    BL0942_PWR_OFF;
    Delay_MS(500);
    BL0942_PWR_ON;
}
/*!
    \brief      void BL0942WriteData(u8 add,u32 data) BL0942往寄存器写数据
    \param[in]  u8 add:寄存器地址 u32 data:写入寄存器的数据
    \param[out] none
    \retval     none
*/
void BL0942WriteData(u8 add, u32 dat)
{
    u8 i = 0;
    u8 data[6];
    u8 checksum = 0;
    data[0] = 0xA8;
    memcpy(&data[1], &add, 1);
    memcpy(&data[2], (void *)&dat, 3);
    for (i = 0; i < 5; i++)
        checksum += data[i];
    data[5] = ~(checksum & 0xFF);
	#if BL0942_DEBUG
    printf("checksum == 0x%x\r\n", data[5]);
	#endif
    Uart2_SendData(data, 6);
}
/*!
    \brief      void BL0942ReadData(u8 add)
    \param[in]  u8 add:寄存器地址
    \param[out] u32 寄存器返回值
    \retval     none
*/
void BL0942ReadData(u8 add)
{
    u8 data[2];
    data[0] = 0x58;
    memcpy(&data[1], &add, 1);
    Uart2_SendData(data, 2);
}
/*!
    \brief      BL0942ReadEleData  读取BL0942全电参数
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BL0942ReadEleData(void)
{
    u8 data[2] = {0x58, 0xAA};
    if (bl0942_flag == 1)
    {
        bl0942_flag = 0;
        Uart2_SendData(data, 2);
    }
}
/*!
    \brief      BL0942GetEleData  从串口缓存中获取电参数数据
    \param[in]  u8 *
    \param[out] none
    \retval     none
*/
void BL0942GetEleData(u8 *data)
{
    static u32 energytemp = 0;
    u32 energy = 0;
    u32 data1 = 0;
    memcpy((void *)&BL0942_EleData_Reg.V_RMS, (void *)&data[4], 3);
    memcpy((void *)&BL0942_EleData_Reg.I_RMS, (void *)&data[1], 3);
    memcpy((void *)&BL0942_EleData_Reg.I_FAST_RMS, (void *)&data[7], 3);
    memcpy((void *)&BL0942_EleData_Reg.WATT, (void *)&data[10], 3);
    memcpy((void *)&BL0942_EleData_Reg.CF_CNT, (void *)&data[13], 3);
    memcpy((void *)&BL0942_EleData_Reg.FREQ, (void *)&data[16], 3);
    memcpy((void *)&BL0942_EleData_Reg.STATUS, (void *)&data[19], 3);
#if BL0942_DEBUG
    printf("BL0942_EleData_Reg.I_RMS == 0x%x\r\n", BL0942_EleData_Reg.I_RMS);
    printf("BL0942_EleData_Reg.V_RMS == 0x%x\r\n", BL0942_EleData_Reg.V_RMS);
    printf("BL0942_EleData_Reg.I_FAST_RMS== 0x%x\r\n", BL0942_EleData_Reg.I_FAST_RMS);
    printf("BL0942_EleData_Reg.WATT== 0x%x\r\n", BL0942_EleData_Reg.WATT);
    printf("BL0942_EleData_Reg.CF_CNT== 0x%x\r\n", BL0942_EleData_Reg.CF_CNT);
    printf("BL0942_EleData_Reg.FREQ== 0x%x\r\n", BL0942_EleData_Reg.FREQ);
    printf("BL0942_EleData_Reg.STATUS== 0x%x\r\n", BL0942_EleData_Reg.STATUS);
#endif

    BL0942_EleData.vol = 0.000062959 * BL0942_EleData_Reg.V_RMS;
    BL0942_EleData.current = 0.000062959 * BL0942_EleData_Reg.I_RMS;
    BL0942_EleData.power = 0.001604129 * BL0942_EleData_Reg.WATT;
    BL0942_EleData.freq = 1 / (float)BL0942_EleData_Reg.FREQ * 1000000;
    energytemp += BL0942_EleData_Reg.CF_CNT;
    if (energytemp >= 54) // 累计到5400是1度电 那么累计到54就是0.01度电
    {
        energy = 0.000186667 * (float)energytemp * 1000; // 放大1000倍
        data1 = BL0942_EleData.energy;
        data1 += energy;
        BL0942_EleData.energy = data1;
        energytemp = 0;
    }

#if BL0942_DEBUG
    printf("BL0942_EleData.vol == %d V \r\n", BL0942_EleData.vol);
    printf("BL0942_EleData.current == %d mA \r\n", BL0942_EleData.current);
    printf("BL0942_EleData.power == %d W \r\n", BL0942_EleData.power);
    printf("BL0942_EleData.freq == %d  \r\n", BL0942_EleData.freq);
#endif
}
/*!
    \brief      BL0942Task  BL0942任务
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BL0942Task(void)
{
    BL0942ReadEleData();
    Uart2Task();
}
