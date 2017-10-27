
#include <stdint.h>
#include "gpio.h"

#define DEBOUNCE_CNT_INITIAL	512
#define DEBOUNCE_CNT_MAX		1024
#define DEBOUNCE_CNT_MIN		0

GPIO_State Button1_Event = RELEASED;
GPIO_State Button2_Event = RELEASED;
GPIO_State Button3_Event = RELEASED;
GPIO_State Button4_Event = RELEASED;

struct GPIO_List GPIO_Input4 = {4, &Button4_Event, RELEASED, 0,  GPIOA, 0};
struct GPIO_List GPIO_Input3 = {3, &Button3_Event, RELEASED, 1,  GPIOA, &GPIO_Input4};
struct GPIO_List GPIO_Input2 = {2, &Button2_Event, RELEASED, 2, GPIOA, &GPIO_Input3};
struct GPIO_List GPIO_Input1 = {1, &Button1_Event, RELEASED, 3,  GPIOA, &GPIO_Input2};
struct GPIO_List * sw_list = &GPIO_Input1;


void GPIO_Init(void)
{
	RCC->APB2ENR |= 
		RCC_APB2ENR_IOPAEN |
		RCC_APB2ENR_IOPBEN |
		RCC_APB2ENR_IOPCEN |
		RCC_APB2ENR_AFIOEN;	
		
	GPIOB->CRH &= ~0x44000000;
	GPIOB->CRH |= 0x33000000;
	
	GPIOB->CRH &= ~0x00400000;
	GPIOB->CRH |= 0x00300000;
	
	GPIOC->CRH &= ~0x00000044;
	GPIOC->CRH |= 0x00000033;
}

/*!
 --------------------------------------------------------------------
 \brief
 
 --------------------------------------------------------------------
 */
GPIO_State GPIO_DebounceFilter(GPIO_TypeDef * gpio, uint16_t pin) 
{
	uint16_t mask = 0x0001 << pin;
	int dc = DEBOUNCE_CNT_INITIAL;
	GPIO_State st;
	
	do {
		if (gpio->IDR & mask) {
			dc--;
			if (dc > DEBOUNCE_CNT_MIN) 
				continue;
			st = RELEASED;
			break;
		}
		dc++;
		if (dc < DEBOUNCE_CNT_MAX)
			continue;
		st = PRESSED;
		break;
	} while (1);
	return st;
}

/*!
 --------------------------------------------------------------------
 \brief
 
 --------------------------------------------------------------------
 */
void GPIO_DebounceProcess(struct GPIO_List * sw)
{
	if (sw->state == RELEASED) {
		if (GPIO_DebounceFilter(sw->gpio, sw->pin) == PRESSED) 
			sw->state  = PRESSED;
	} else {
		if (GPIO_DebounceFilter(sw->gpio, sw->pin) == RELEASED) {
			sw->state  = RELEASED;
			*sw->event = PRESSED;
		}
	}
}

/*!
 --------------------------------------------------------------------
 \brief
 
 --------------------------------------------------------------------
 */
void GPIO_Handler(void)
{
	sw_list = &GPIO_Input1;
	while (sw_list != 0) {
		GPIO_DebounceProcess(sw_list);
		sw_list = sw_list->next;
	} 
}
