#include "bsp.h"
int main(void)
{
  init_system();
  BspInit();
  Display_page1();
  while (1)
  {

    BspTask();
    RS485Task();
  }
}
