/*
 * File:    timer.h
 * Author:  Alexander130892
 * Date:    25-5-2026
 *
 * Description:
 *   This header file defines the interface and configuration for Timer
 *   2 on an STM32 microcontroller, setting up a 1 Hz interrupt (1 ms
 *   tick) through prescaler and auto-reload values, with an
 *   initialization function and interrupt handler.
 */
#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include "status.h"


// 	Tim2 Input Clk = 16 MHz (default) --> APB1 prescaler /1, default
// 	PSC = 15999 --> 16 Mhz / 15999+1 	= 1000 Hz
//	ARR = 499 	--> 1000 / 999+1 		= 1 Hz
#define PSC_VALUE	15999	//Pre-scaler 16 MHZ/ (PSC_VALUE +1) = 1ms
#define ARR_VALUE	999		//Auto-reload Register (Period = ARR+1 = 1000 ms)

status_t timer2_init(void);
void TIM2_IRQHandler(void);



#endif /* TIMER_H_ */
