
#include <avr/io.h>
#include "buzzer.h"
// buzzer.c
void buzzer_init(void) {
    // Timer1 PWM setup for buzzer
    DDRB |= (1 << PB1);
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
    ICR1  = 499;
    OCR1A = 249;
    TCCR1A &= ~(1 << COM1A1);   // start silent
}

void buzzerOn(void){
    TCCR1A |= (1 << COM1A1);        // connect OC1A --> buzzer on
}
void buzzerOff(void){
    TCCR1A &= ~(1 << COM1A1);        // connect OC1A --> buzzer on
}
      