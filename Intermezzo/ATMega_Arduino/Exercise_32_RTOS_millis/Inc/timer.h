/*
 * File:    timer.h
 * Author:  Alexander130892
 * Date:    7-6-2026
 *
 * Description:
 *   This header file defines constants and function declarations for a
 *   millisecond timer implementation on a microcontroller with a 16
 *   MHz clock, configured to increment a counter every millisecond
 *   using a prescaler of 64 and an auto-reload value of 249.
 */
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/*
16,000,000 / 64 = 250,000 ticks/second
250,000 / 1000 = 250 ticks/ms
ARR = 249
*/
#define FOSC    16000000        // Clock Speed
#define PSC     64              //  16MHz / 64 = 250 000
#define TIMER_ARR     249             // 250 000 / 1000 = 250 --> 249+1

void timer0_init(void);
uint32_t millis(void);

#endif