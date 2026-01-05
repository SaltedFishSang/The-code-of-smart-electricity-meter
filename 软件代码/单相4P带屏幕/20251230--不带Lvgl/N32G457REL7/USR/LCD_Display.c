#include "BSP.h"
/*!
    \brief     Display_page1 显示界面1
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Display_page1(void)
{
    /*顶端显示时间、4G信号*/
    TFTLCD_Fill(0, 0, TFTLCD_W, TFTLCD_H, LGRAY);
    TFTLCD_DrawLine(0, 48, TFTLCD_W, 48, RED);
    /*显示日期*/
    TFTLCD_ShowIntNum(0, 8, 2025, 4, BLACK, LGRAY, 24);
    TFTLCD_ShowString(48, 8, "/", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowIntNum(60, 8, 12, 2, BLACK, LGRAY, 24);
    TFTLCD_ShowString(84, 8, "/", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowIntNum(96, 8, 29, 2, BLACK, LGRAY, 24);
    /*显示时间*/
    TFTLCD_ShowIntNum(132, 8, 10, 2, BLACK, LGRAY, 24);
    TFTLCD_ShowString(156, 8, ":", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowIntNum(168, 8, 6, 2, BLACK, LGRAY, 24);

    /*显示是否定时*/
    TFTLCD_ShowCN(216, 12, 0, MAGENTA, LGRAY, 16, 0);
    TFTLCD_ShowCN(232, 12, 1, MAGENTA, LGRAY, 16, 0);
    TFTLCD_ShowCN(248, 12, 2, MAGENTA, LGRAY, 16, 0);
    /*显示图标*/
    TFTLCD_ShowString(290, 8, "4G", BLACK, LGRAY, 24, 0);

    /*显示电压*/
    TFTLCD_ShowCN(0, 64, 0, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowCN(32, 64, 1, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowString(64, 64, ":", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowIntNum(88, 64, 220, 4, BLACK, LGRAY, 24);
    TFTLCD_ShowString(160, 64, "V", BLACK, LGRAY, 24, 0);

    /*显示在线*/
    TFTLCD_ShowCN(224, 64, 17, GREEN, LGRAY, 32, 0);
    TFTLCD_ShowCN(256, 64, 19, GREEN, LGRAY, 32, 0);

    /*显示电流*/
    TFTLCD_ShowCN(0, 96, 0, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowCN(32, 96, 2, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowString(64, 96, ":", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowIntNum(88, 96, 80, 4, BLACK, LGRAY, 24);
    TFTLCD_ShowString(160, 96, "A", BLACK, LGRAY, 24, 0);
    /*显示功率*/
    TFTLCD_ShowCN(0, 128, 3, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowCN(32, 128, 4, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowString(64, 128, ":", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowIntNum(88, 128, 17600, 6, BLACK, LGRAY, 24);
    TFTLCD_ShowString(208, 128, "W", BLACK, LGRAY, 24, 0);
    /*显示电能*/
    TFTLCD_ShowCN(0, 160, 7, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowCN(32, 160, 8, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowString(64, 160, ":", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowFloatNum1(88, 160, (float)123456789, 10, BLACK, LGRAY, 24);
    TFTLCD_ShowString(256, 160, "KW/H", BLACK, LGRAY, 24, 0);
    /*显示输出*/
    TFTLCD_ShowCN(0, 192, 9, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowCN(32, 192, 10, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowString(64, 192, ":", BLACK, LGRAY, 24, 0);
    TFTLCD_ShowCN(88, 192, 11, BLACK, LGRAY, 32, 0);
    TFTLCD_ShowCN(120, 192, 12, BLACK, LGRAY, 32, 0);

    /*显示锁扣*/
    TFTLCD_ShowCN(224, 192, 15, RED, LGRAY, 32, 0);
    TFTLCD_ShowCN(256, 192, 16, RED, LGRAY, 32, 0);
}
