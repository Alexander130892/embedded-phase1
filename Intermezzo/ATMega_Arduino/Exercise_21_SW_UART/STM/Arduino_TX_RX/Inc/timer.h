/*
 * File:    timer.h
 * Author:  Alexander130892
 * Date:    30-5-2026
 *
 * Description:
 *   This header file defines timer initialization and utility
 *   functions for an embedded system, specifically configuring Timer 2
 *   to generate a 2 Hz interrupt with 1 ms tick resolution for
 *   millisecond-level timekeeping. It provides functions to initialize
 *   the timer, handle its interrupt, and retrieve elapsed
 *   milliseconds.
 */
#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include "status.h"


// 	Tim2 Input Clk = 16 MHz (default) --> APB1 prescaler /1, default
// 	PSC = 15999 --> 16 Mhz / 15999+1 	= 1000 Hz
//	ARR = 499 	--> 1000 / 499+1 		= 2 Hz
#define PSC_VALUE	15999	//Pre-scaler 16 MHZ/ (PSC_VALUE +1) = 1ms
#define ARR_VALUE	499		//Auto-reload Register (Period = ARR+1 = 500 ms)

status_t timer2_init(void);
void TIM2_IRQHandler(void);
uint32_t millis();


#endif /* TIMER_H_ */
