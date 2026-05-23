/*
 * led.c
 *
 *  Created on: Apr 29, 2026
 *      Author: alexandervindelinckx
 */

#include "led.h"
#include "stm32f446re.h"
#include <stdint.h>
//PA5 - D13 LED
status_t LED_init(void){
	//Enable Clock
	*((volatile uint32_t *)(RCC_BASE_ADDR + RCC_AHB1ENR_OFFSET)) 	|= 	(1u << 0);			//  Bit 0 for GPIOA
	//PA5
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET)) 	&= ~(0x3 << 2*5);
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET)) 	|= 	(0x1 << 2*5);
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_ODR_OFFSET)) 	&= ~(0x1 << 5);
	return STATUS_OK;
}
status_t LED_toggle(void){
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_ODR_OFFSET)) 	^= (0x1 << 5);
	return STATUS_OK;
}
