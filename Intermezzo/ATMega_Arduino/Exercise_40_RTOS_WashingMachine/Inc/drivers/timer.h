/*
 * File:    timer.h
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This header file defines a software timer interface for a 16MHz
 *   microcontroller, configuring a hardware timer with prescaler and
 *   auto-reload values to provide millisecond-resolution tick
 *   counting, and declaring functions to manage multiple software
 *   timers for tracking elapsed time.
 */
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

/*
16,000,000 / 64 = 250,000 ticks/second
250,000 / 1000 = 250 ticks/ms
ARR = 249
*/

#define TIMER0_PSC      64u              //  16MHz / 64 = 250 000
#define TIMER0_ARR      249u             // 250 000 / 1000 = 250 --> 249+1

typedef struct {
    uint32_t start;
    uint32_t duration;
    bool active;
} soft_timer_t;

//extern SoftTimer timer_uart; //no longer used
extern soft_timer_t timer_phase;
extern soft_timer_t timer_water;
extern soft_timer_t timer_buzzer;

void timer0_init(void);
uint32_t getTicks(void);
void sw_timerStart(soft_timer_t *timer, uint32_t duration);
bool sw_timerExpired(soft_timer_t *timer);
void sw_timerStop(soft_timer_t *timer);


#endif