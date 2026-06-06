/*
 * File:    timer.h
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This header file defines the interface and configuration for
 *   Timer2 on an embedded microcontroller, setting up a 100 Hz timer
 *   with a 10 ms period through specific prescaler and auto-reload
 *   values. It provides functions to initialize, enable, and disable
 *   the timer.
 */
#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include "drivers/status.h"


// 	Tim2 Input Clk = 16 MHz (default) --> APB1 prescaler /1, default



// 	PSC = 1599 --> 16 Mhz / 1599+1 	= 10000 Hz
//	ARR = 99 	--> 10000 / 99+1 		= 100 Hz
#define PSC_VALUE	1599	//Pre-scaler 16 MHZ/ (PSC_VALUE +1) = 0.1ms
#define ARR_VALUE	99		//Auto-reload Register (Period = ARR+1 = 10 ms)
status_t timer2_init(void);
status_t timer2_enable(void);
status_t timer2_disable(void);

#endif /* TIMER_H_ */
