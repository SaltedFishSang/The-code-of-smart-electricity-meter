#include "BSP.h"

RTC_Calendar RtcTime,RtcTime1;

void Rtc_Init(void){ 
	RTC_InitType RTC_InitStructure; 
	RTC_TimeType RTC_Time; 
	RTC_DateType RTC_Date; 
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE); //打开电源接口时钟
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_BKP, ENABLE); 
	
	PWR_BackupAccessEnable(ENABLE); //写访问使能 
	if (BKP_ReadBkpData(BKP_DAT1) != 0x0505){ //是否第一次配置？写过备份值就不执行RTC初始化 不掉电时间不变 
		RCC_ConfigRtcClk(RCC_RTCCLK_SRC_LSE); 
	  RCC_ConfigLse(RCC_LSE_ENABLE); //设置RTC时钟(RTCCLK)，选择低速外部时钟LSE作为RTC时钟//LSE 开启 
	  while (RCC_GetFlagStatus(RCC_FLAG_LSERD) == RESET);//等待LSE晶振就绪 
		RCC_EnableRtcClk (ENABLE); //使能RTC时钟 
		//RTC_WriteProtectionCmd(DISABLE); //解锁所有RTC寄存器 

		//标准库设置时间 
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F; //RTC异步分频系数(1-0X7F)
		RTC_InitStructure.RTC_SynchPrediv = 0xFF; //RTC同步分频系数(0-7FFF)
		RTC_InitStructure.RTC_HourFormat = RTC_24HOUR_FORMAT;//RTC设置为，24小时格式 
		RTC_StructInit(&RTC_InitStructure); 
		
		printf("RTC初始化%d\r\n",RTC_Init(&RTC_InitStructure)); //值为1，初始化成功 
		
		RTC_Date.Year = 25; //设置年
		RTC_Date.Month= 1;  //月
		RTC_Date.Date = 1;  //日
		RTC_Date.WeekDay = 3;  //星期
		RTC_Time.Minutes= 0;  //时
		RTC_Time.Seconds = 0;  //分
		RTC_Time.Hours = 0;   //秒
		RTC_SetDate(RTC_FORMAT_BIN,&RTC_Date);//写入日期 
		RTC_ConfigTime(RTC_FORMAT_BIN,&RTC_Time); //写入时间
		
		BKP_WriteBkpData(BKP_DAT1,0x0505); //标记已经初始化过了
	}
}

void Get_RTC_Time(void){ 
	RTC_TimeType RTC_Time; 
	RTC_DateType RTC_Date; 
	
	RTC_GetTime(RTC_FORMAT_BIN, &RTC_Time); 
	RTC_GetDate(RTC_FORMAT_BIN, &RTC_Date); 
	
	RtcTime.hours = RTC_Time.Hours;//小时
	RtcTime.minutes = RTC_Time.Minutes; //分钟
	RtcTime.seconds = RTC_Time.Seconds;//秒 
	
	RtcTime.months = RTC_Date.Month; 
	RtcTime.days = RTC_Date.Date; 
	RtcTime.years = RTC_Date.Year+2000 ; 
	RtcTime.weekday = RTC_Date.WeekDay;
	

  //printf("Time: %0.2d-%0.2d-%0.2d,%0.2d:%0.2d:%0.2d  %d\r\n",RtcTime.years, RtcTime.months, RtcTime.days, RtcTime.hours, RtcTime.minutes, RtcTime.seconds,RtcTime.weekday);
}

// BCD 转十进制工具函数
uint8_t bcd_to_dec(uint8_t bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

uint8_t dec_to_bcd(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

//W = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400 + 1) % 7
u8 calculate_weekday(uint8_t y1, uint8_t m, uint8_t d) {
    u8 weekday;
	  u16 y = 0;
	
	  y = bcd_to_dec(y1) + 2000;
		m = bcd_to_dec(m);
		d = bcd_to_dec(d);
	
	  if(m > 12 || m < 1 || d < 1 || d > 31){
        return 0;
    }
    if(m == 1 || m == 2){
        m = m + 12;
			  y--;
		}
		
		weekday = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400 + 1) % 7;
		
		if(weekday == 0){
			weekday = 7;
		}
			
    return weekday; 
		// 调整结果，使1=周一，...6=周六,7=周日
}

void Set_RTC_Time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second){ 
	  RTC_TimeType RTC_Time; 
	  RTC_DateType RTC_Date; 

    RTC_Date.Year = year - 2000;       //年
		RTC_Date.Month= month;      //月
		RTC_Date.Date = day;        //日
  	RTC_Date.WeekDay = calculate_weekday(RTC_Date.Year,RTC_Date.Month,RTC_Date.Date);  //星期
	
		RTC_Time.Hours= hour;     //时
		RTC_Time.Minutes = minute;  //分
		RTC_Time.Seconds = second;    //秒
	
		RTC_SetDate(RTC_FORMAT_BIN,&RTC_Date);    //写入日期 
		RTC_ConfigTime(RTC_FORMAT_BIN,&RTC_Time); //写入时间
}
