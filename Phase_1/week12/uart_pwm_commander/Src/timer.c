/*
 * File:    timer.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file implements timer functionality for an STM32F446RE
 *   microcontroller, providing PWM signal generation with configurable
 *   duty cycle, a millisecond counter function, and LED blinking
 *   capability through timer interrupts. The timer is configured to
 *   run at 1kHz with interrupt-driven LED blinking every 500
 *   milliseconds.
 */
#include "timer.h"
#include "led.h"
#include "stm32f446re.h"
#include <stdint.h>

uint32_t g_CCR;
uint32_t g_duty_cycle = 50;

void init_pwm(void){
	g_CCR = g_duty_cycle * (ARR + 1) / 100;
	// Init timer
		// 	Tim2 Input Clk = 16 MHz (default) --> APB1 prescaler /1, default
		// 	PSC = 159 --> 16 Mhz / 159+1 		= 100 000 Hz
		//	ARR = 99 	--> 100 000 / 99+1 		= 1000 Hz

	// DIR bit at pos 4 > 0 is counting up
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CR1_OFFSET)) 		&= ~(0x1 << 4);
	// CEN bit at pos 0 > disable while setting registers
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CR1_OFFSET)) 		&= ~(0x1 << 0);
	// TIMx_PSC
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_PSC_OFFSET)) 		= PSC;
	// TIMx_ARR
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_ARR_OFFSET)) 		= ARR;
	// Set OCM bits in TIMx_CCMR1 → PWM mode 1 (110)
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CCMR1_OFFSET))		&= ~(0x7 << 4);
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CCMR1_OFFSET))		|= (0x6 << 4);
	// Set OC1PE = 1 in TIMx_CCMR1 (preload enable)
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CCMR1_OFFSET))		|= (0x1 << 3);
	// Set TIMx_CCR1 (initial duty cycle) D = CCR1 / (ARR+1)
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CCR1_OFFSET))		= g_CCR;
	// Set CC1E = 1 in TIMx_CCER (enable channel output to pin)
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CCER_OFFSET))		|= (0x1 << 0);
	// Write UG = 1 in TIMx_EGR (force update — loads PSC/ARR into shadow registers)
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_EGR_OFFSET))		|= (0x1 << 0);
	// Set CEN = 1 in TIMx_CR1 (start counter)
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CR1_OFFSET))		|= (0x1 << 0);
}
uint32_t millis(){
	uint32_t static time=0;

	if((*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) & (0x1 << 0))){ //Check UIF (bit 0) is set (overflow)
		// Clear UIF — must happen before next wait
		*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) &= ~(0x1 << 0);
		 time++;
	}
	return time ; // only ms resolution
}
/*Timer clock after prescaler: 16 MHz / (159+1) = 100,000 Hz
One tick = 1/100,000 = 10 µs
One full CNT cycle (ARR=99): 100 ticks × 10 µs = 1 ms
So overflow_count increments every 1 ms — the unit holds*/

void init_blink(void){
	//Configure NVIC
	*((volatile uint32_t*)(NVIC_BASE_ADDR + NVIC_ISER0_OFFSET))		|= (0x1 << TIM2_POS); //NVIC_EnableIRQ(TIM2_IRQn);
	//Enable TIM2 Interruot
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_DIER_OFFSET)) 		|= (0x1 << 0); // Enable Interrupt mode
}

void TIM2_IRQHandler(void){
	if( (*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) & (0x1 << 0)) ){
		//Clear overflow bit
		*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) &= ~(0x1);
		overflow_count++;
		if(overflow_count >= 500){
			overflow_count = 0;
			led_blink();
		}
	}
}
