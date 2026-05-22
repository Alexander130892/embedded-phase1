/*
 * File:    timer.h
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This header file configures Timer 2 (TIM2) on a microcontroller to
 *   generate a 1000 Hz interrupt by defining prescaler and auto-reload
 *   register values for a 16 MHz clock. It declares the TIM2 interrupt
 *   handler and a `millis()` function that tracks elapsed milliseconds
 *   using a volatile overflow counter incremented by the timer ISR.
 */
#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#define PSC 159		// 	PSC = 159 --> 16 Mhz / 159+1 		= 100 000 Hz
#define ARR 99		//	ARR = 99 	--> 100 000 / 99+1 		= 1000 Hz

void TIM2_IRQHandler(void);



uint32_t millis();
volatile uint32_t static overflow_count=0;


#endif /* TIMER_H_ */
