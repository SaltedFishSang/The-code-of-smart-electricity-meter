/**
 * @file mian.c
 * @author Sangjian
 * @version v1
 * @data 20250426
 * @copyright Copyright (c) 225, kaanxun Technologies Inc. All rights reserved.
 */ \
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include "BSP.h"

//#define LOG_INFO(x, ...) printf("%s:%s:%d:" x "", \
//                                __FILE__,         \
//                                __FUNCTION__,     \
//                                __LINE__,         \
//                                ##__VA_ARGS__)
int main(void)
{
    BSPInit();
    Flash_paraInit();
    while (1)
    {
        BSPTask();
        BL0942Task();
        LTE_process();
    }
}
