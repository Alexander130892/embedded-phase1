

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "timer.h"

static uint32_t millis_count = 0;

void timer0_init(void){
	TCCR0A = 0x02; 		// CTC mode
	TCCR0B = 0x3;   	// PSC 64
	OCR0A = TIMER_ARR;		
	TIMSK0 |= (1u << OCIE0A); // Enable interrupt
}

uint32_t millis(void){
	uint32_t val;
    cli();
    val = millis_count;
    sei();
    return val;
}

ISR(TIMER0_COMPA_vect) {
    millis_count++;
}

