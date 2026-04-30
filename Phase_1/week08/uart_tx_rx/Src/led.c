/*
 * File:    led.c
 * Author:  Alexander130892
 * Date:    22-4-2026
 *
 * Description:
 *   This file provides low-level GPIO control functions for an LED
 *   connected to pin PA10 on an STM32F4 microcontroller, allowing
 *   initialization, on/off control, and state reading through direct
 *   register manipulation.
 */
#include 	"led.h"
#include	"stm32f46re.h"

#include <stdint.h>

#define	GPIOA_MODER_OFFSET		0x00 // 00 input, 01 GPIO, bit 20-21 for PA10
#define	GPIOA_ODR_OFFSET		0x14 // bit 10 for PA10


void led_init(void){
	//Init LED on pin PA10
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOA_MODER_OFFSET))	|= (0x1 << 2*10);

}
void led_on(void){
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOA_ODR_OFFSET))	|= (0x1 << 10); // set bit 10
}
void led_off(void){
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOA_ODR_OFFSET))	&= ~(0x1 << 10); // clear bit 10
}
bool led_get_state(void){
	bool val= (bool)(((*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOA_ODR_OFFSET)))& (0x1 << 10)) >> 10);
	return val;
}
