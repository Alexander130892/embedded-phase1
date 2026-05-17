/*
 * File:    systick.c
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This file configures the ARM Cortex-M SysTick timer to generate
 *   periodic 1-millisecond interrupts using the 16 MHz processor
 *   clock. It maintains a running millisecond counter via the
 *   `SysTick_Handler` ISR, which can be read externally through
 *   `systick_get_ms()`.
 */
//SYSTICK
#include "systick.h"

#define SYST_CSR    *((volatile uint32_t *)0xE000E010)  // Control & Status
#define SYST_RVR    *((volatile uint32_t *)0xE000E014)  // Reload Value
#define SYST_CVR    *((volatile uint32_t *)0xE000E018)  // Current Value

volatile uint32_t tick_ms=0;

void systick_init(void){
	SYST_RVR	=	SYSTICK_MS_VALUE;		// 16 MHZ / 1000ms -1
	SYST_CVR 	= 	0x0;		// Reset
	SYST_CSR 	|=	(0x7<<0);	//Bit 0 = 1 — enable counter Bit 1 = 1 — enable interrupt Bit 2 = 1 — use processor clock (16MHz)
}

void SysTick_Handler(void){
	tick_ms++;
}
uint32_t systick_get_ms(void){
    return tick_ms;
}
