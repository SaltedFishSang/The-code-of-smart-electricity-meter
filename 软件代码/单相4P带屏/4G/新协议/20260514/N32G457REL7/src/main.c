#include "bsp.h"
int main(void)
{
  BspInit();
  Display_pageinit();
  HeartInfoBegin();
  printf("hello world\r\n");
  while (1)
  {
    BspTask();
    RS485Task();
    TFTTask();
    LTE_process();
    HeartInfo();
  }
}
