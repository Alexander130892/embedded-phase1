/*
 * timer.c
 *
 *  Created on: Apr 29, 2026
 *      Author: alexandervindelinckx
 */


#include "timer.h"
#include "led.h"
#include "stm32f446re.h"
#include <stdint.h>

volatile uint32_t overflow_count = 0;

void init_timer(void){
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
	// TIMx_DIER
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_DIER_OFFSET)) 		|= (0x1 << 0); // Enable Interrupt mode
	//Configure NVIC
	*((volatile uint32_t*)(NVIC_BASE_ADDR + NVIC_ISER0_OFFSET))		|= (0x1 << TIM2_POS); //NVIC_EnableIRQ(TIM2_IRQn);
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
	return time; // only ms resolution
}
/*Timer clock after prescaler: 16 MHz / (159+1) = 100,000 Hz
One tick = 1/100,000 = 10 µs
One full CNT cycle (ARR=99): 100 ticks × 10 µs = 1 ms
So overflow_count increments every 1 ms — the unit holds*/

void TIM2_IRQHandler(void){
	if( (*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) & (0x1 << 0)) ){
		//Clear overflow bit
		*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) &= ~(0x1);
		overflow_count++;
	}
}

