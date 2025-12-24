#include "bsp.h"
/**
 * @brief  Inserts a delay time.
 * @param count specifies the delay time length.
 */

int main(void)
{
  init_system();
  BspInit();
  printf("hello world\r\n");
  //TFT_SEND_CMD(0x29);
 // TFT_clear();
 // delay_ms(100);
 // TFT_full(0x001F);
  while (1)
  {
    BspTask();
    //    delay_us(1000000);
    //    printf("main loop\r\n");
  }
}
