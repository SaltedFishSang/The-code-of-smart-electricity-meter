#include "BSP.h"
struct _BL0942_EleData_Reg BL0942_EleData_Reg;
struct _BL0942_EleData BL0942_EleData;

#define BL0942_PWR_ON GPIO_ResetBits(GPIOA, GPIO_PIN_1)
#define BL0942_PWR_OFF GPIO_SetBits(GPIOA, GPIO_PIN_1)
#define BL0942_DEBUG 0
u8 bl0942_flag = 0;
void ReadBL0942EleFlag(void)
{
    bl0942_flag = 1;
    Vtimer_SetTimer(VTIM4, 1000, ReadBL0942EleFlag);
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
}
/*!
    \brief      BL0942_PWRGpioInit(void) BL0942电源控制GPIO口初始化
    \param[in]  None
    \param[out] none
    \retval     none
*/
void BL0942_PWRGpioInit(void)
{
    /* PA1*/
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_PIN_1);
    // BL0942_PWR_ON;
    BL0942_HardReset();
    delay_ms(3000);
    BL0942_RegInit();
    Vtimer_SetTimer(VTIM4, 1000, ReadBL0942EleFlag);
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
    delay_ms(500);
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
    Uart2Send(data, 6);
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
    Uart2Send(data, 2);
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
        //			  #if BL0942_DEBUG
        //    printf("Get BL0942 Data\r\n");
        // #endif
        Uart2Send(data, 2);
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
    static u32 fir_energry = 0; // 上一次的电能脉冲计数
    static u32 mid_energy = 0;  // 电能累计中间变量
    u32 energy = 0;
    u32 data1 = 0;
    int power = 0;
    memcpy((void *)&BL0942_EleData_Reg.V_RMS, (void *)&data[4], 3);
    memcpy((void *)&BL0942_EleData_Reg.I_RMS, (void *)&data[1], 3);
    memcpy((void *)&BL0942_EleData_Reg.I_FAST_RMS, (void *)&data[7], 3);
    memcpy((void *)&BL0942_EleData_Reg.WATT, (void *)&data[10], 3);
    memcpy((void *)&BL0942_EleData_Reg.CF_CNT, (void *)&data[13], 3);
    memcpy((void *)&BL0942_EleData_Reg.FREQ, (void *)&data[16], 3);
    memcpy((void *)&BL0942_EleData_Reg.STATUS, (void *)&data[19], 3);
#if 0
    printf("BL0942_EleData_Reg.I_RMS == 0x%x\r\n", BL0942_EleData_Reg.I_RMS);
    printf("BL0942_EleData_Reg.V_RMS == 0x%x\r\n", BL0942_EleData_Reg.V_RMS);
    printf("BL0942_EleData_Reg.I_FAST_RMS== 0x%x\r\n", BL0942_EleData_Reg.I_FAST_RMS);
    printf("BL0942_EleData_Reg.WATT== 0x%x\r\n", BL0942_EleData_Reg.WATT);
    printf("BL0942_EleData_Reg.CF_CNT== 0x%x\r\n", BL0942_EleData_Reg.CF_CNT);
    printf("BL0942_EleData_Reg.FREQ== 0x%x\r\n", BL0942_EleData_Reg.FREQ);
    printf("BL0942_EleData_Reg.STATUS== 0x%x\r\n", BL0942_EleData_Reg.STATUS);
#endif

    BL0942_EleData.vol = 0.000062959 * BL0942_EleData_Reg.V_RMS;
    BL0942_EleData.current = 0.0159227134 * BL0942_EleData_Reg.I_RMS;
    power = BL0942_EleData_Reg.WATT;
    if (power > 8388607)
    {
        power &= 0x7FFFFF;
        power = ~power;
        power += 1;
        power &= 0x7FFFFF;
    }
    BL0942_EleData.power = 0.00641652 * power;
    BL0942_EleData.freq = ((float)BL0942_EleData.power / BL0942_EleData.vol / BL0942_EleData.current) * 100 * 1000;
    energytemp += BL0942_EleData_Reg.CF_CNT;

    if (energytemp >= fir_energry)
    {
        mid_energy += (energytemp - fir_energry);
    }
    else // 电能脉冲计数累积满出现溢出情况
    {
        mid_energy += (energytemp + (0xFFFFFF - fir_energry));
    }
    fir_energry = energytemp;
    energytemp = 0;

    if (mid_energy >= 13) // 累计到1300是1度电 那么累计到13就是0.01度电
    {
        energy = 0.000747575254f * (float)mid_energy * 1000; // 放大1000倍
        data1 = BL0942_EleData.energy;
        data1 += energy;
        BL0942_EleData.energy = data1;
        mid_energy = 0;
        Modbus_WrieteInput(8, (BL0942_EleData.energy >> 16));
        Modbus_WrieteInput(9, (BL0942_EleData.energy & 0xffff));
    }
#if BL0942_DEBUG
    printf("BL0942_EleData.vol == %d V \r\n", BL0942_EleData.vol);
    printf("BL0942_EleData.current == %d mA \r\n", BL0942_EleData.current);
    printf("BL0942_EleData.power == %d W \r\n", BL0942_EleData.power);
    printf("BL0942_EleData.freq == %d  \r\n", BL0942_EleData.freq);
#endif
    Modbus_WrieteInput(4, BL0942_EleData.vol);          // 写入电压有效值
    Modbus_WrieteInput(5, BL0942_EleData.current / 10); // 写入第一路电流
    Modbus_WrieteInput(7, BL0942_EleData.power);        // 写入第1路功率
    Modbus_WrieteInput(6, BL0942_EleData.freq);         // 写入第1路功率因素
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
