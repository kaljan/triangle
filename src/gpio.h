#ifndef GPIO_H
#define GPIO_H

#include <stm32f10x.h>

#define mLD3_On()	GPIOC->BSRR = GPIO_BSRR_BR9;
#define mLD3_Off()	GPIOC->BSRR = GPIO_BSRR_BS9;
#define mLD3_Toggle() \
	if (GPIOC->IDR & GPIO_IDR_IDR9) { \
		GPIOC->BSRR = GPIO_BSRR_BR9; \
	} else { \
		GPIOC->BSRR = GPIO_BSRR_BS9; \
	}
	
#define mLD4_On()	GPIOC->BSRR = GPIO_BSRR_BR8;
#define mLD4_Off()	GPIOC->BSRR = GPIO_BSRR_BS8;
#define mLD4_Toggle() \
	if (GPIOC->IDR & GPIO_IDR_IDR8) { \
		GPIOC->BSRR = GPIO_BSRR_BR8; \
	} else { \
		GPIOC->BSRR = GPIO_BSRR_BS8; \
	}

#define mMRK1_On()	GPIOB->BSRR = GPIO_BSRR_BS14;
#define mMRK1_Off()	GPIOB->BSRR = GPIO_BSRR_BR14;
#define mMRK1_Toggle() \
	if (GPIOB->IDR & GPIO_IDR_IDR14) { \
		GPIOB->BSRR = GPIO_BSRR_BR14; \
	} else { \
		GPIOB->BSRR = GPIO_BSRR_BS14; \
	}
	
#define mMRK2_On()	GPIOB->BSRR = GPIO_BSRR_BS15;
#define mMRK2_Off()	GPIOB->BSRR = GPIO_BSRR_BR15;
#define mMRK2_Toggle() \
	if (GPIOB->IDR & GPIO_IDR_IDR15) { \
		GPIOB->BSRR = GPIO_BSRR_BR15; \
	} else { \
		GPIOB->BSRR = GPIO_BSRR_BS15; \
	}

typedef enum _t_GPIO_State {
	PRESSED = 0,
	RELEASED
} GPIO_State;

struct GPIO_List {
	int id;
	GPIO_State *event;
	GPIO_State state;
	int pin;
	GPIO_TypeDef * gpio;		
	struct GPIO_List * next;
};

extern GPIO_State Button1_Event;
extern GPIO_State Button2_Event;
extern GPIO_State Button3_Event;
extern GPIO_State Button4_Event;

void GPIO_Init(void);
void GPIO_Handler(void);

#endif // GPIO_H
