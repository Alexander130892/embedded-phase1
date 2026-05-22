/*
 * File:    timer.h
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This header defines configuration and interface for a millisecond
 *   timer using TIM2 on a microcontroller running at 16 MHz, with
 *   prescaler and auto-reload values set to generate a 1 kHz interrupt
 *   frequency. It declares the TIM2 interrupt handler and a `millis()`
 *   function that returns elapsed milliseconds by tracking timer
 *   overflow counts.
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
