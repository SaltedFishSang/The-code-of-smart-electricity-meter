#ifndef _LCD_Display_H
#define _LCD_Display_H
#include "BSP.h"
extern void Display_pageinit(void);
extern void Display_page1(void);
extern void TFTTask(void);
extern void TFTTimer(void);
extern void DisplayOutputState(u8 state);
extern void DisplayLockState(u8 state);
extern void DisplayNetState(u8 state);
extern void DisplayClockState(u8 state);
#endif
