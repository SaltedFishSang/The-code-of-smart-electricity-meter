#include "bsp.h"
#include "pic.h"
u8 state = 0;
int main(void)
{
  init_system();
  BspInit();
  TFTLCD_Fill(0, 0, TFTLCD_W, TFTLCD_H, WHITE);
  TFTLCD_ShowString(0, 0, "i miss you", RED, WHITE, 16, 0);
	printf("hello world\r\n");
  while (1)
  {
    BspTask();
    RS485Task();
  }
}
