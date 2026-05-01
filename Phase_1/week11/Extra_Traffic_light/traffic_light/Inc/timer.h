/*
 * timer.h
 *
 *  Created on: Apr 29, 2026
 *      Author: alexandervindelinckx
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#define PSC 159		// 	PSC = 159 --> 16 Mhz / 159+1 		= 100 000 Hz
#define ARR 99		//	ARR = 99 	--> 100 000 / 99+1 		= 1000 Hz

void TIM2_IRQHandler(void);
void init_timer(void);
void init_blink(void);
uint32_t millis();

extern volatile uint32_t overflow_count;

#endif /* TIMER_H_ */
