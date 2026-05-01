/*
 * File:    led.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file provides low-level LED control functions for an STM32F4
 *   microcontroller, specifically managing an LED connected to GPIO
 *   pin PA10 through direct register manipulation. It offers basic
 *   operations to initialize the LED pin as an output, turn the LED
 *   on/off, and read its current state.
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
