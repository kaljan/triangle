#include <stm32f10x.h>
#include <stdio.h>

#include "gpio.h"

#include "ssd1311.h"

extern void systick_init(void);
const char lcd_name[22] = "REX001602CYPP5N00000 \0";

extern volatile int systick_flag;

int main(void)
{
	volatile int i, j = 0;
	char str[33];
	
	GPIO_Init();
	systick_init();
	
	while (systick_flag == 0);
	systick_flag = 0;
	GPIOB->BSRR = GPIO_BSRR_BR5;
	while (systick_flag == 0);
	systick_flag = 0;
	GPIOB->BSRR = GPIO_BSRR_BS5;
	
	ssd1311_init();
	while (systick_flag == 0);
	systick_flag = 0;
	ssd1311_on();

	sprintf(str, "Hello world!    %s", lcd_name);
	ssd1311_set_string(str, 0);
	ssd1311_swap();
	
	while (1) {
	}	
}
