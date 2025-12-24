#include "bsp.h"
#include "pic.h"
RCC_ClocksType  RCC_ClocksTypeStruct = {0};
int main(void)
{
  init_system();

  BspInit();
  printf("hello world\r\n");
  TFTLCD_Fill(0, 0, TFTLCD_W, TFTLCD_H, WHITE);
  TFTLCD_ShowString(0, 0, "i miss you", RED, WHITE, 16, 0);
	 printf("hello world\r\n");
		RCC_GetClocksFreqValue(&RCC_ClocksTypeStruct);
  while (1)
  {
    BspTask();
  }
}
