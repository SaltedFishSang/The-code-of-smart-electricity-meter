#include "bsp.h"
#include "pic.h"
#include "lvgl.h"
#include "../GUI_APP/generated/gui_guider.h"
#include "../GUI_APP/generated/events_init.h"
#include "custom.h"
lv_ui guider_ui;
u8 state = 0;
int main(void)
{
  init_system();
  BspInit();
  printf("hello world\r\n");
// TFTLCD_Fill(0, 0, TFTLCD_W, TFTLCD_H, WHITE);
// TFTLCD_ShowString(0, 0, "i miss you", RED, WHITE, 16, 0);
	lv_init();
	lv_port_disp_init();
		lv_port_indev_init();
		// 1. 获取当前活动屏幕（默认屏幕）
	lv_obj_t * scr = lv_scr_act();

	// 画个标签：居中显示
	lv_obj_t * label = lv_label_create(scr);
	lv_label_set_text(label, "HELLO STM32");
	lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0); 
	lv_obj_center(label); // 居中

	// 再画个标签：放在屏幕左上角 (50, 100)
	lv_obj_t * label1 = lv_label_create(scr);
	lv_label_set_text(label1, "HELLO LVGL");
	lv_obj_set_style_text_font(label1, &lv_font_montserrat_14, 0);
	lv_obj_set_pos(label1, 100, 100); // 绝对定位到 (50,100)
	
	// 画个方块
	lv_obj_t * test_rect = lv_obj_create(lv_scr_act());
	lv_obj_set_size(test_rect, 50, 50);
	lv_obj_set_style_bg_color(test_rect, lv_palette_main(LV_PALETTE_RED), 0);
	lv_obj_align(test_rect, LV_ALIGN_TOP_LEFT, 0, 0);
	
	// 再画一个
	lv_obj_t * test_rect1 = lv_obj_create(lv_scr_act());
	lv_obj_set_size(test_rect1, 60, 60);
	lv_obj_set_style_bg_color(test_rect1, lv_palette_main(LV_PALETTE_YELLOW), 0);
	lv_obj_align(test_rect1, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
//custom_init(&guider_ui);
//  setup_ui(&guider_ui);
//  events_init(&guider_ui);
  while (1)
  {
    BspTask();
    RS485Task();
		lv_task_handler();
  }
}
