/*
 * File:    timer.c
 * Author:  Alexander130892
 * Date:    8-6-2026
 *
 * Description:
 *   This file implements a timer system for an AVR microcontroller,
 *   providing a hardware timer (Timer0) that increments a system tick
 *   counter via interrupts, and software timer functions that allow
 *   multiple independent timers to track elapsed time for buzzer, LED,
 *   UART, and button debounce operations. It also configures Timer1
 *   for PWM output at 500Hz with adjustable duty cycle.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "timer.h"

static uint32_t systemTicks = 0;
SoftTimer timer_buzzer_off = {0, 0, false};
SoftTimer timer_buzzer = {0, 0, false};
SoftTimer timer_autoClose = {0, 2000, false};
SoftTimer timer_LED = {0, 0, false};
SoftTimer timer_uart = {0, 0, false};
SoftTimer timer_button_debounce = {0, 0, false};

void timer0_init(void){
	TCCR0A = 0x02; 		// CTC mode
	TCCR0B = 0x3;   	// PSC 64
	OCR0A = TIMER0_ARR;		
	TIMSK0 |= (1u << OCIE0A); // Enable interrupt
}

void timer1_pwm_init(void) {
    // OC1A (PB1) as output
    DDRB |= (1 << PB1);

    // Fast PWM mode 14 (TOP = ICR1), non-inverting on OC1A
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10); // PSC 64

    ICR1  = 499;    // TOP → 500Hz
    OCR1A = 249;    // 50% duty cycle
    TCCR1A &= ~(1 << COM1A1);   // start silent
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

void sw_timerStart(SoftTimer *timer, uint32_t duration) {
    timer->start = getTicks();
    timer->duration = duration;
    timer->active = true;
}

bool sw_timerExpired(SoftTimer *timer) {
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