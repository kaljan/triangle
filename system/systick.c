#include <stdlib.h>
#include <stm32f10x.h>
#include "gpio.h"

volatile int systick_flag;

void SysTick_Handler(void)
{
	if (SysTick->CTRL & 0x10000) {
		systick_flag = 1;
		GPIO_Handler();
	}
}


void systick_init(void)
{
	systick_flag = 0;
	SysTick->LOAD = 249999;
	SysTick->CTRL |= 0x06;
	SysTick->CTRL |= 0x01;
}
