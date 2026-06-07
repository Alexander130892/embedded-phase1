/*
 * File:    timer.h
 * Author:  Alexander130892
 * Date:    7-6-2026
 *

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