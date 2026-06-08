/*
 * File:    timer.h
 * Author:  Alexander130892
 * Date:    8-6-2026
 *
 * Description:
 *   This header file defines software timers and hardware timer
 *   initialization for a 16 MHz microcontroller, providing functions
 *   to manage millisecond-precision timing for peripherals like
 *   buzzers, LEDs, UART, and button debouncing. It configures Timer0
 *   and Timer1 with a prescaler of 64 to achieve 1 millisecond
 *   resolution and exposes a software timer API with start and
 *   expiration check functions.
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
#define FOSC    16000000        // Clock Speed
#define TIMER0_PSC     64              //  16MHz / 64 = 250 000
#define TIMER0_ARR     249             // 250 000 / 1000 = 250 --> 249+1

#define TIMER1_PSC     64              //  16MHz / 64 = 250 000
#define TIMER1_ARR     249             // 250 000 / 1000 = 250 --> 249+1

typedef struct {
    uint32_t start;
    uint32_t duration;
    bool active;
} SoftTimer;

extern SoftTimer timer_buzzer_off;
extern SoftTimer timer_buzzer;
extern SoftTimer timer_LED;
extern SoftTimer timer_uart;
extern SoftTimer timer_button_debounce;

void timer0_init(void);
void timer1_pwm_init(void);
uint32_t getTicks(void);
void sw_timerStart(SoftTimer *timer, uint32_t duration);
bool sw_timerExpired(SoftTimer *timer);

#endif