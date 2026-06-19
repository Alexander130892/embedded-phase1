/*
 * File:    timer.c
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This file implements a software timer system for an AVR
 *   microcontroller using Timer0 interrupt to maintain a system tick
 *   counter and provide timer management functions for various
 *   application subsystems (LED, UART, phase, water, buzzer). The soft
 *   timers track elapsed time by comparing current ticks against a
 *   stored start time and duration.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "timer.h"

static uint32_t systemTicks = 0;

soft_timer_t timer_LED = {0, 0, false};
soft_timer_t timer_uart = {0, 0, false};
soft_timer_t timer_phase = {0, 0, false};
soft_timer_t timer_water = {0u, 0u, false};
soft_timer_t timer_buzzer = {0, 0, false};

void timer0_init(void){
	TCCR0A = 0x02; 		// CTC mode
	TCCR0B = 0x3;   	// PSC 64
	OCR0A = TIMER0_ARR;		
	TIMSK0 |= (1u << OCIE0A); // Enable interrupt
}
uint32_t getTicks(void){
	uint32_t val;
    cli();
    val = systemTicks;
    sei();
    return val;
}

ISR(TIMER0_COMPA_vect) {
    systemTicks++;
}

void sw_timerStart(soft_timer_t *timer, uint32_t duration) {
    timer->start = getTicks();
    timer->duration = duration;
    timer->active = true;
}

bool sw_timerExpired(soft_timer_t *timer) {
    if (!(timer->active)) {
        return false;
    }
    if (getTicks() - timer->start >= timer->duration) {
        timer->active = false;
        return true;
    }else {
        return false;
    }
}
void sw_timerStop(soft_timer_t *timer)
{
    timer->active = false;
}
          