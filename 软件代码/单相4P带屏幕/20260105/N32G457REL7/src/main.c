#include "bsp.h"
int main(void)
{
  BspInit();
  Display_pageinit();
  HeartInfoBegin();
  HeartStausBegin();
  while (1)
  {
    BspTask();
    RS485Task();
    TFTTask();
    LTE_process();
    HeartInfo();
    HeartStatus();
  }
}
