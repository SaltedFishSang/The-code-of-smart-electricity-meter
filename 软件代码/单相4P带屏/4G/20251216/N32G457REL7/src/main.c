#include "bsp.h"
#include "pic.h"

int main(void)
{
  init_system();
  BspInit();
  printf("hello world\r\n");
  //TFT_SEND_CMD(0x29);
 // TFT_clear();
 // delay_ms(100);
 // TFT_full(0x001F);
		TFTLCD_Fill(0,0,TFTLCD_W,TFTLCD_H,WHITE);	
//	delay_ms(1000);	
//	TFTLCD_Fill(0,0,TFTLCD_W,TFTLCD_H,BLACK);
//	delay_ms(1000);	
//	TFTLCD_Fill(0,0,TFTLCD_W,TFTLCD_H,BLUE);		
//	delay_ms(1000);
//	TFTLCD_Fill(0,0,TFTLCD_W,TFTLCD_H,RED);		
//	delay_ms(1000);	
//	TFTLCD_Fill(0,0,TFTLCD_W,TFTLCD_H,GREEN);		
//	delay_ms(1000);	
//		  TFTLCD_ShowPicture(0,0,240,20,gImage_lxh1);delay_ms(1);
//		TFTLCD_ShowPicture(0,20,240,20,gImage_lxh2);delay_ms(1);
//		TFTLCD_ShowPicture(0,40,240,20,gImage_lxh3);delay_ms(1);
//		TFTLCD_ShowPicture(0,60,240,20,gImage_lxh4);delay_ms(1);
//		TFTLCD_ShowPicture(0,80,240,20,gImage_lxh5);delay_ms(1);
//		TFTLCD_ShowPicture(0,100,240,20,gImage_lxh6);delay_ms(1);
//		TFTLCD_ShowPicture(0,120,240,20,gImage_lxh7);delay_ms(1);
//		TFTLCD_ShowPicture(0,140,240,22,gImage_lxh8);delay_ms(1);
		TFTLCD_ShowString(0,0,"i miss you",RED,WHITE,16,0);
  while (1)
  {
    BspTask();


    //    delay_us(1000000);
    //    printf("main loop\r\n");
  }
}
