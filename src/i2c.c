#include <stdint.h>
#include <stm32f10x.h>
#include "gpio.h"

typedef enum _i2c_state_t {
	START_SENT = 0,
	ADDR_SENT = 1,
	DATA_SENT = 2,
	STOP_SENT = 3
} i2c_state_t;


volatile int i2c_rdy;

static volatile uint8_t i2c_addr;
static volatile uint16_t temp;
static volatile i2c_state_t i2c_state;


void DMA1_Channel6_IRQHandler(void)
{
	if (DMA1->ISR & DMA_ISR_TCIF6) {
		DMA1->IFCR = 0x00F00000;
		DMA1_Channel6->CCR &= ~DMA_CCR6_EN;
		I2C1->CR2 |= I2C_CR2_ITEVTEN;
	}
}

void I2C1_EV_IRQHandler(void)
{	
	switch (i2c_state) {
	
	case START_SENT: 
		if (I2C1->SR1 & I2C_SR1_SB) {
			I2C1->DR = i2c_addr;
			i2c_state = ADDR_SENT;
		}
		break;
		
	case ADDR_SENT:
		if (I2C1->SR1 & I2C_SR1_ADDR) {
			I2C1->CR2 &= ~I2C_CR2_ITEVTEN;
			temp = I2C1->SR2;
			i2c_state = DATA_SENT;
		} 
		break;
		
	case DATA_SENT:
		if (I2C1->SR1 & I2C_SR1_BTF) {
			I2C1->CR1 |= I2C_CR1_STOP;
			i2c_state = STOP_SENT;
		}
		break;
		
	case STOP_SENT:
		while (I2C1->SR2 & I2C_SR2_BUSY);
		i2c_rdy = 1;
		break;
		
	default: break;
	}
}


void i2c_init(void)
{
	// RCC
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	
	// I2C
	I2C1->CR2 |= 24;
	
	I2C1->CCR |= 
		I2C_CCR_FS |
		I2C_CCR_DUTY |
		0x03;
		
	I2C1->TRISE |= 0x0B;	
	
	I2C1->CR2 |= 
		I2C_CR2_DMAEN | 
		I2C_CR2_ITEVTEN;
	
	NVIC->ISER[0] = 0x80000000;
	
	// DMA
	DMA1_Channel6->CPAR = I2C1_BASE + 0x10;
	
	DMA1->IFCR = 0x00F00000;
	
	DMA1_Channel6->CCR = 
		DMA_CCR6_TCIE |
		DMA_CCR6_MINC |
		DMA_CCR6_DIR;
	
	NVIC->ISER[0] |= 0x00010000;
	
	// GPIO
	GPIOB->CRL &= ~0x44000000;
	GPIOB->CRL |= 0xFF000000;
	GPIOB->BSRR = 
		GPIO_BSRR_BS7 | 
		GPIO_BSRR_BS6;
}

int i2c_send_data(uint8_t addr, const uint8_t *buf, int size)
{
	i2c_rdy = 0;
	i2c_addr = addr;
	i2c_state = START_SENT;
	
	DMA1_Channel6->CMAR = (uint32_t)(buf);
	DMA1_Channel6->CNDTR = size;
	DMA1_Channel6->CCR |= DMA_CCR6_EN;
	
	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR1 |= I2C_CR1_START;
	
	return 0;
}

/*
 Было бы не плохо добавить в этот модуль, функцию обработки ошибок.
 И восстановление работоспособности I2C.
 
 Т.е. если у нас произошла проблема с I2C делаем его сброс и 
 реинициализацию.
 */
