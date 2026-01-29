#include "bsp.h"
u32 m_flashdata[50];
#define FLASH_PAGE_SIZE ((uint16_t)0x800)
#define flash_write_addr ((uint32_t)0x08030000) // 存放出来mac地址和设备ID之外的网络参数
#define flash_write_macidaddr ((uint32_t)0x08038000)
void DataFlashErase(void)
{
	/* Unlocks the FLASH Program Erase Controller */
	FLASH_Unlock();
	/* Erase */
	if (FLASH_COMPL != FLASH_EraseOnePage(flash_write_addr))
	{
		while (1)
		{
			printf("Flash EraseOnePage Error. Please Deal With This Error Promptly\r\n");
		}
	}
	/* Locks the FLASH Program Erase Controller */
	FLASH_Lock();
}

void FlashWriteData(void)
{
	u16 i = 0;
	u32 addr;
	DataFlashErase();
	for (i = 0; i < 4; i++)
		m_flashdata[0 + i] = EtnernetPara.IPAddr[i];

	for (i = 0; i < 4; i++)
		m_flashdata[4 + i] = EtnernetPara.GWIPAddr[i];

	for (i = 0; i < 4; i++)
		m_flashdata[8 + i] = EtnernetPara.IPMask[i];

	m_flashdata[12] = EtnernetPara.PORT >> 8;
	m_flashdata[13] = EtnernetPara.PORT & 0xff;

	for (i = 0; i < 4; i++)
		m_flashdata[14 + i] = EtnernetPara.DESIP[i];

	m_flashdata[18] = EtnernetPara.DESPORT >> 8;
	m_flashdata[19] = EtnernetPara.DESPORT & 0xff;
	
  m_flashdata[20] = 0xa1; 
	m_flashdata[21] = 0x58;
	m_flashdata[22] = 0xa2; 
	m_flashdata[23] = 0x59; 
	/* Unlocks the FLASH Program Erase Controller */
	FLASH_Unlock();

	/* Erase */
	if (FLASH_COMPL != FLASH_EraseOnePage(flash_write_addr))
	{
		while (1)
		{
			printf("Flash EraseOnePage Error. Please Deal With This Error Promptly\r\n");
		}
	}
	addr = flash_write_addr;
	/* Program */
	for (i = 0; i < 24; i++)
	{
		// printf("m_flashdata[%d] == %d", i, m_flashdata[i]);
		if (FLASH_COMPL != FLASH_ProgramWord(addr + i * 4, m_flashdata[i]))
		{
			while (1)
			{
				printf("Flash ProgramWord Error. Please Deal With This Error Promptly\r\n");
			}
		}
	}

	// /* Locks the FLASH Program Erase Controller */
	FLASH_Lock();
}

void FlasgWriteResetData(void)
{
	DataFlashErase();
	u32 addr = 0;
	uint16_t i = 0;
	m_flashdata[0] = 192;
	m_flashdata[1] = 168;
	m_flashdata[2] = 3;
	m_flashdata[3] = 156; // 默认IP地址

	m_flashdata[4] = 192;
	m_flashdata[5] = 168;
	m_flashdata[6] = 3;
	m_flashdata[7] = 1; // 默认网关

	m_flashdata[8] = 255;
	m_flashdata[9] = 255;
	m_flashdata[10] = 255;
	m_flashdata[11] = 0; // 默认掩码

	m_flashdata[12] = 0x1F;
	m_flashdata[13] = 0x40; // 默认本地端口

	m_flashdata[14] = 192;
	m_flashdata[15] = 168;
	m_flashdata[16] = 8;
	m_flashdata[17] = 155; // 默认服务器IP地址

	m_flashdata[18] = 0x2B;
	m_flashdata[19] = 0x48; // 默认服务器端口
	
	m_flashdata[20] = 0xa1; 
	m_flashdata[21] = 0x58;
	m_flashdata[22] = 0xa2; 
	m_flashdata[23] = 0x59; 
	/* Unlocks the FLASH Program Erase Controller */
	FLASH_Unlock();
	addr = flash_write_addr;
	/* Program */
	for (i = 0; i < 24; i++)
	{
		if (FLASH_COMPL != FLASH_ProgramWord(addr + i * 4, m_flashdata[i]))
		{
			while (1)
			{
				printf("Flash ProgramWord Error. Please Deal With This Error Promptly\r\n");
			}
		}
	}

	// /* Locks the FLASH Program Erase Controller */
	FLASH_Lock();
	memset(m_flashdata, 0, 24);
}

void FlashInit(void)
{
		uint8_t i;
	FLASH_Unlock();
	for (i = 0; i < 24; i++)
	{
		(uint32_t) m_flashdata[i] = (*(__IO uint32_t *)(flash_write_addr + i * 4));
		// printf("m_flashdata[%d] == %d\r\n", i, m_flashdata[i]);
	}
	FLASH_Lock();
	
	if( (m_flashdata[20] == 0xa1 ) && (m_flashdata[21] == 0x58 ) && (m_flashdata[22] == 0xa2 ) && (m_flashdata[23] == 0x59 ))
	{
		 FlashReadData();
     Flash_Read_mac_id();
	}else
	{
		 FlasgWriteResetData();
   //  FlashWritereset_mac_id();
	   FlashReadData();
     Flash_Read_mac_id();
	}
}
void FlashReadData(void)
{
	uint8_t i;
	FLASH_Unlock();
	for (i = 0; i < 24; i++)
	{
		(uint32_t) m_flashdata[i] = (*(__IO uint32_t *)(flash_write_addr + i * 4));
		// printf("m_flashdata[%d] == %d\r\n", i, m_flashdata[i]);
	}
	for (i = 0; i < 4; i++)
		EtnernetPara.IPAddr[i] = m_flashdata[0 + i];
	for (i = 0; i < 4; i++)
		EtnernetPara.GWIPAddr[i] = m_flashdata[4 + i];
	for (i = 0; i < 4; i++)
		EtnernetPara.IPMask[i] = m_flashdata[8 + i];

	EtnernetPara.PORT = (m_flashdata[12] << 8) + m_flashdata[13];
	for (i = 0; i < 4; i++)
		EtnernetPara.DESIP[i] = m_flashdata[14 + i];

	EtnernetPara.DESPORT = (m_flashdata[18] << 8) + m_flashdata[19];
	// printf("IP == %d.%d.%d.%d\r\nMask == %d.%d.%d.%d\r\nGwip == %d.%d.%d.%d\r\nDesIp == %d.%d.%d.%d\r\nDesport == %d\r\nPort == %d\r\n",
	// 	   EtnernetPara.IPAddr[0], EtnernetPara.IPAddr[1], EtnernetPara.IPAddr[2], EtnernetPara.IPAddr[3],
	// 	   EtnernetPara.IPMask[0], EtnernetPara.IPMask[1], EtnernetPara.IPMask[2], EtnernetPara.IPMask[3],
	// 	   EtnernetPara.GWIPAddr[0], EtnernetPara.GWIPAddr[1], EtnernetPara.GWIPAddr[2], EtnernetPara.GWIPAddr[3],
	// 	   EtnernetPara.DESIP[0], EtnernetPara.DESIP[1], EtnernetPara.DESIP[2], EtnernetPara.DESIP[3],
	// 	   EtnernetPara.DESPORT, EtnernetPara.PORT);
	FLASH_Lock();
}
//
void FlashWrite_mac_id(void)
{
	u32 addr;
	u16 i = 0;
	/* Unlocks the FLASH Program Erase Controller */
	FLASH_Unlock();
	/* Erase */
	if (FLASH_COMPL != FLASH_EraseOnePage(flash_write_macidaddr))
	{
		while (1)
		{
			printf("Flash EraseOnePage Error. Please Deal With This Error Promptly\r\n");
		}
	}
	/*把数据写入扇区7开始的区域*/
	m_flashdata[20] = EtnernetPara.MACAddr[0];
	m_flashdata[21] = EtnernetPara.MACAddr[1];
	m_flashdata[22] = EtnernetPara.MACAddr[2];
	m_flashdata[23] = EtnernetPara.MACAddr[3];
	m_flashdata[24] = EtnernetPara.MACAddr[4];
	m_flashdata[25] = EtnernetPara.MACAddr[5];

	for (i = 0; i < 16; i++)
		m_flashdata[26 + i] = (u32)g_Equipment_number[i];
	FLASH_Unlock();
	addr = flash_write_macidaddr;
	/* Program */
	for (i = 0; i < 23; i++)
	{
		// printf("m_flashdata[%d] == %d", i, m_flashdata[i]);
		if (FLASH_COMPL != FLASH_ProgramWord(addr + i * 4, m_flashdata[20 + i]))
		{
			while (1)
			{
				printf("Flash ProgramWord Error. Please Deal With This Error Promptly\r\n");
			}
		}
	}
	// /* Locks the FLASH Program Erase Controller */
	FLASH_Lock();
}
void FlashWritereset_mac_id(void)
{
	u32 addr;
	u16 i = 0;
	/* Unlocks the FLASH Program Erase Controller */
	FLASH_Unlock();
	/* Erase */
	if (FLASH_COMPL != FLASH_EraseOnePage(flash_write_macidaddr))
	{
		while (1)
		{
			printf("Flash EraseOnePage Error. Please Deal With This Error Promptly\r\n");
		}
	}
	/*把数据写入扇区7开始的区域*/
	m_flashdata[20] = 0x44;
	m_flashdata[21] = 0xA6;
	m_flashdata[22] = 0x42;
	m_flashdata[23] = 0x1E;
	m_flashdata[24] = 0xC8;
	m_flashdata[25] = 0x18;

	for (i = 0; i < 16; i++)
		m_flashdata[26 + i] = (u32)g_Equipment_number[i];
	FLASH_Unlock();
	addr = flash_write_macidaddr;
	/* Program */
	for (i = 0; i < 23; i++)
	{
		// printf("m_flashdata[%d] == %d", i, m_flashdata[i]);
		if (FLASH_COMPL != FLASH_ProgramWord(addr + i * 4, m_flashdata[20 + i]))
		{
			while (1)
			{
				printf("Flash ProgramWord Error. Please Deal With This Error Promptly\r\n");
			}
		}
	}
	// /* Locks the FLASH Program Erase Controller */
	FLASH_Lock();
}
void Flash_Read_mac_id(void)
{

	uint8_t i;
	FLASH_Unlock();
	for (i = 0; i < 22; i++)
	{
		(uint32_t) m_flashdata[20 + i] = (*(__IO uint32_t *)(flash_write_macidaddr + i * 4));
	}
	// for (i = 0; i < 17; i++)
	// 	printf(" %c", m_flashdata[26 + i]);
	for (i = 0; i < 6; i++)
		EtnernetPara.MACAddr[i] = m_flashdata[20 + i];
	for (i = 0; i < 16; i++)
		g_Equipment_number[i] = m_flashdata[26 + i];
	g_Equipment_number[16] = '\0';
	// printf("Mac_addr==0x%02x.0x%02x.0x%02x.0x%02x.0x%02x.0x%02x\r\n", EtnernetPara.MACAddr[0], EtnernetPara.MACAddr[1], EtnernetPara.MACAddr[2], EtnernetPara.MACAddr[3], EtnernetPara.MACAddr[4], EtnernetPara.MACAddr[5]);
	// printf("ID==%s\r\n", g_Equipment_number);
	FLASH_Lock();
}
