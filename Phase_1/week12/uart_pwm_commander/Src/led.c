/*
 * File:    led.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file implements LED control functions for an STM32F446RE
 *   microcontroller, configuring GPIO pin PA0 for PWM output using
 *   Timer 2 and providing functions to turn the LED on/off, check its
 *   state, and make it blink by manipulating the PWM duty cycle and
 *   timer settings.
 */
#include "led.h"
#include "timer.h"
#include "stm32f446re.h"
#include <stdint.h>

void init_gpio_pa0_pwm(void){
	init_pwm();
	//Configure PA5: MODER = 10 (AF mode), AFRL = 0001 (AF1 → TIM2_CH1)
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOA_MODER_OFFSET)) 	&= ~(0x3 << 2*0);
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOA_MODER_OFFSET)) 	|= 	(0x2 << 2*0);
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOA_AFRL_OFFSET)) 	&= ~(0xF << 4*0);
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOA_AFRL_OFFSET)) 	|= 	(0x1 << 4*0);
}

void led_init(void){
	//Init LED on pin PA0
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOA_MODER_OFFSET))	|= (0x1 << 2*0);
}
void led_on(void){
	g_duty_cycle = (uint32_t) 100;
	g_CCR = g_duty_cycle * (ARR + 1) / 100;
	// Set TIMx_CCR1 (initial duty cycle) D = CCR1 / (ARR+1)
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CCR1_OFFSET))		= g_CCR;
}
void led_off(void){
	g_duty_cycle = (uint32_t) 0;
	g_CCR = g_duty_cycle * (ARR + 1) / 100;
	// Set TIMx_CCR1 (initial duty cycle) D = CCR1 / (ARR+1)
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CCR1_OFFSET))		= g_CCR;
}
bool led_get_state(void){
	bool val= (bool)(((*((volatile uint32_t *)(TIM2_BASE_ADDR + TIMx_CCR1_OFFSET)))));
	return val;
}

void led_blink(void){ // Toggle bit that connects timer to GPIO
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CCER_OFFSET))		^= (0x1 << 0);
}
