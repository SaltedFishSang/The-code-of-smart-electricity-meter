#include "bsp.h"
#include "pic.h"
u8 state = 0;
int main(void)
{
  init_system();
  BspInit();
  printf("hello world\r\n");
  TFTLCD_Fill(0, 0, TFTLCD_W, TFTLCD_H, WHITE);
  TFTLCD_ShowString(0, 0, "i miss you", RED, WHITE, 16, 0);
  printf("hello world\r\n");
  while (1)
  {
    BspTask();
    RS485Task();
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_2) == RESET)
    {
			delay_ms(10 0);
			    if (GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_2) == RESET)
					{
      state++;
      if (state == 1)
      {
        Relay_Control(0);
      }
      if (state == 2)
      {
        state = 0;
        Relay_Control(1);
      }
		}
    }
  }
}
