#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f1xx.h"
#include "qian_head.h"

//#include "stm32f10x.h"

void SysTick_Init(void);
//void Delay_us(__IO u32 nTime);         // 单位1us

//#define DELAY_US(x) {	detime = x;\
//											while(detime);\
//											}
//#define DELAY_MS(x) DELAY_US(1000*x)	 //单位ms


#endif /* __SYSTICK_H */
