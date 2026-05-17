/*
 * File:    systick.h
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This header declares the interface for a SysTick timer module
 *   configured to generate 1 ms tick interrupts on a 16 MHz system
 *   clock. It exposes functions to initialize the timer, handle the
 *   SysTick interrupt, and retrieve the current millisecond tick
 *   count.
 */
#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>
#include "status.h"

#define SYSTICK_MS_VALUE	15999		// 16 MHZ / 1000ms -1


void systick_init(void);
void SysTick_Handler(void);
uint32_t systick_get_ms(void);

#endif /* SYSTICK_H_ */
