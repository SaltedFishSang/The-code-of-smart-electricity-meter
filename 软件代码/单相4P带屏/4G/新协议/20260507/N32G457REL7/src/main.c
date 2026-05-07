#include "bsp.h"
int main(void)
{
  BspInit();
  Display_pageinit();
  HeartInfoBegin();
  while (1)
  {
    BspTask();
    RS485Task();
    TFTTask();
    LTE_process();
    HeartInfo();
  }
}
