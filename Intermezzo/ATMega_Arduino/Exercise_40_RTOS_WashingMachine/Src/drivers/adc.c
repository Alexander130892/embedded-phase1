/*
 * File:    adc.c
 * Author:  Alexander130892
 * Date:    10-6-2026
 *
 * Description:
 *   ADC driver for ATmega328P. Single conversion mode,
 *   AVcc reference, prescaler 128 (125kHz @ 16MHz).
 */

#include <avr/io.h>
#include "adc.h"

void adc_init(void){
    ADMUX  =  (1u << REFS0);               // AVcc reference            
    ADCSRA =  (1u << ADEN)  |              // enable ADC                
              (1u << ADPS2) |              // prescaler 128:           
              (1u << ADPS1) |              // 16MHz / 128 = 125kHz     
              (1u << ADPS0);
    DIDR0  |= (1u << ADC0D);              // disable digital on PC0   
}

uint16_t adc_read(uint8_t channel){
    ADMUX = (ADMUX & 0xF0u) | (channel & 0x0Fu);    // select channel   
    ADCSRA |= (1u << ADSC);                         // start conversion 
    while (ADCSRA & (1u << ADSC));                  // wait             
    return ADC;                                     // 10-bit result    
}