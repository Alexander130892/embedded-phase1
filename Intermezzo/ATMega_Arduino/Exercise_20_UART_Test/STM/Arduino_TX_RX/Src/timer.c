/*
 * timer.c
 *
 *  Created on: May 22, 2026
 *      Author: alexandervindelinckx
 */

#include "timer.h"
#include "uart.h"
#include "stm32f446re.h"
#include <stdbool.h>

status_t timer2_init(void){
	//Clock init timer
	*((volatile uint32_t *)(RCC_BASE_ADDR + RCC_APB1ENR_OFFSET)) 	|= 0x1;
	// TIM2_CR1
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CR1_OFFSET)) 		&= ~(0x1 << 4); // DIR bit at pos 4 > 0 is counting up
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CR1_OFFSET)) 		&= ~(0x1 << 0); // CEN bit at pos 0 > disable while setting registers
	// TIMx_PSC
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_PSC_OFFSET)) 		= PSC_VALUE; // PSC = 15999
	// TIMx_ARR
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_ARR_OFFSET)) 		= ARR_VALUE; // DIR bit at pos 4 > 0 is counting up
	// TIMx_DIER
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_DIER_OFFSET)) 		|= (0x1 << 0); // Enable Interrupt mode
	// TIMx_EGR
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_EGR_OFFSET)) 		|= (0x1 << 0); // Manually create event to update shadow registers
	// TIMx_SR
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) 		&= ~(0x1); // Bit 0 checks for overflow, clear by software, init to zero
	// Enable TIM2
	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CR1_OFFSET)) 		|= (0x1 << 0); // CEN bit at pos 0 > Enable after setting up

	//Configure NVIC
	*((volatile uint32_t*)(NVIC_BASE_ADDR + NVIC_ISER0_OFFSET))		|= (0x1 << TIM2_POS); //NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void){
	static bool toggled = false;
	if( (*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) & (0x1 << 0)) ){
		// send on/off to Arduino
		if(toggled){
			uart4_transmit(0x01);
		}
		else{
			uart4_transmit(0x00);
		}
		toggled=!toggled;
		//Clear overflow bit
		*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) &= ~(0x1);
	}
}

