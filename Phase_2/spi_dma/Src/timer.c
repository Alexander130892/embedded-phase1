/*
 * File:    timer.c
 * Author:  Alexander130892
 * Date:    10-5-2026
 *
 * Description:
 *   This file implements a millisecond timer using STM32F446RE's TIM2
 *   hardware timer, providing a `millis()` function that returns
 *   elapsed milliseconds by polling the timer's overflow flag. It also
 *   includes an interrupt handler that maintains an overflow counter
 *   that resets every 500 milliseconds.
 */
#include "timer.h"
#include "stm32f446re.h"
#include <stdint.h>


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


void TIM2_IRQHandler(void){
	if( (*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) & (0x1 << 0)) ){
		//Clear overflow bit
		*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) &= ~(0x1);
		overflow_count++;
		if(overflow_count >= 500){
			overflow_count = 0;
		}
	}
}
