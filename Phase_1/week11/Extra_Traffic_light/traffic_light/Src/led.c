/*
 * led.c
 *
 *  Created on: Apr 29, 2026
 *      Author: alexandervindelinckx
 */

#include "led.h"
#include "stm32f446re.h"
#include <stdint.h>

void init_gpio_PCX(uint8_t x){
	*((volatile uint32_t *)(GPIOC_BASE_ADDR + GPIOC_MODER_OFFSET)) 	&= ~(0x3 << 2*x);
	*((volatile uint32_t *)(GPIOC_BASE_ADDR + GPIOC_MODER_OFFSET)) 	|= 	(0x1 << 2*x);
	*((volatile uint32_t *)(GPIOC_BASE_ADDR + GPIOC_ODR_OFFSET)) 	&= ~(0x1 << x);
}

void init_button_PB(uint8_t x){
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOB_MODER_OFFSET)) 	&= ~(0x3 << 2*x);
}
