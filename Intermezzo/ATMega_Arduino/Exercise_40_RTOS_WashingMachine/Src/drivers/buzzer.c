/*
 * File:    buzzer.c
 * Author:  Alexander130892
 * Date:    15-6-2026
 *
 * Description:
 *   Buzzer driver using Timer2 CTC mode with toggle on OC2A (D11/PB3).
 *   Prescaler 64, OCR2A = 249 → 500Hz tone.
 *   f = 16MHz / (2 * 64 * (249+1)) = 500Hz
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