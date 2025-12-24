#include "bsp.h"
#include "pic.h"
#include "lvgl.h"
u8 state = 0;
int main(void)
{
  init_system();
  BspInit();
  printf("hello world\r\n");
  //TFTLCD_Fill(0, 0, TFTLCD_W, TFTLCD_H, WHITE);
 // TFTLCD_ShowString(0, 0, "i miss you", RED, WHITE, 16, 0);
  lv_init();
	lv_port_disp_init();
	//lv_port_indev_init();
	//demo_create();
	lv_test_theme_1(lv_theme_night_init(210, NULL));
  while (1)
  {
    BspTask();
    RS485Task();
		lv_task_handler();
  }
}
