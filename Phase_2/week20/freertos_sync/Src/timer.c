/*
 * File:    timer.c
 * Author:  Alexander130892
 * Date:    24-5-2026
 *
 * Description:
 *   This file initializes and configures Timer 2 (TIM2) on an
 *   STM32F446RE microcontroller to generate periodic interrupts, and
 *   implements an interrupt handler that signals a FreeRTOS semaphore
 *   when the timer overflows. The timer is set up with specific
 *   prescaler and auto-reload values to create timed events that
 *   synchronize task execution in the FreeRTOS scheduler.
 */
#include <stdbool.h>
#include "timer.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stm32f446re.h"

extern SemaphoreHandle_t xSemaphore;


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
	*((volatile uint32_t*)(NVIC_IPR_ADDR + TIM2_POS_IPR))			&= ~(0xF << ((8*0)+4)); //28%4 = 0
	*((volatile uint32_t*)(NVIC_IPR_ADDR + TIM2_POS_IPR))			|= (0x6 << ((8*0)+4));
	return STATUS_OK;
}

void TIM2_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if( (*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) & (0x1 << 0)) ){
		xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
		//Clear overflow bit
		*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) &= ~(0x1);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

