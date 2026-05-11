/*
 * File:    timer.h
 * Author:  Alexander130892
 * Date:    11-5-2026
 *
 * Description:
 *   This header file defines a millisecond timing system using Timer 2
 *   with a prescaler and auto-reload configuration to generate 1kHz
 *   interrupts. It provides a millis() function to return elapsed
 *   milliseconds since startup, tracking timer overflows through an
 *   interrupt handler.
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
