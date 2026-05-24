/*
 * File:    timer.h
 * Author:  Alexander130892
 * Date:    24-5-2026
 *
 * Description:
 *   This header file declares functions to initialize a 16 MHz Timer 2
 *   peripheral configured to generate 1 kHz interrupts (1 ms period)
 *   on an STM32 microcontroller. It defines prescaler and auto-reload
 *   register values needed to achieve the desired timing frequency
 *   from the default APB1 clock.
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
