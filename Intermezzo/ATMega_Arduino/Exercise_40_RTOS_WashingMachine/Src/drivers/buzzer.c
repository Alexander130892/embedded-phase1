/*
 * File:    buzzer.c
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This file implements a buzzer driver for an AVR microcontroller
 *   that generates a 500Hz square wave tone on pin PB3 using Timer 2
 *   in CTC mode, with functions to initialize the timer, enable, and
 *   disable the buzzer output.
 */
#include <avr/io.h>
#include "buzzer.h"


void buzzer_init(void){
    // OC2A (PB3) already set as output in gpio_init      
    OCR2A  = 249u;                      // 500Hz            
    TCCR2A = (1u << WGM21);            // CTC mode         
    TCCR2B = (1u << CS22);             // prescaler 64     
    // OC2A disconnected until buzzerOn() called            
}

void buzzerOn(void){
    TCCR2A |= (1u << COM2A0);          // toggle OC2A      
}

void buzzerOff(void){
    TCCR2A &= ~(1u << COM2A0);         // disconnect OC2A 
}