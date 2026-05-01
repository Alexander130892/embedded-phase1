/*
 * File:    timer.h
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This header file defines timer functionality for a microcontroller
 *   system, providing PWM control and millisecond timing capabilities
 *   through Timer 2 configured to run at 1kHz. It includes interrupt
 *   handling, initialization functions for PWM and blinking
 *   operations, and a millis() function for timestamp tracking using
 *   an overflow counter.
 */
#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#define PSC 159		// 	PSC = 159 --> 16 Mhz / 159+1 		= 100 000 Hz
#define ARR 99		//	ARR = 99 	--> 100 000 / 99+1 		= 1000 Hz

void TIM2_IRQHandler(void);
void init_pwm(void);
void init_blink(void);
uint32_t millis();

extern uint32_t g_duty_cycle;
extern uint32_t g_CCR;
volatile uint32_t static overflow_count=0;


#endif /* TIMER_H_ */
