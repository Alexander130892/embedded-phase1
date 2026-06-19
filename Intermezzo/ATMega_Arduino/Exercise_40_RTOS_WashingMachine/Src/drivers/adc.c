/*
 * File:    adc.c
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This file provides ADC (Analog-to-Digital Converter)
 *   initialization and reading functions for an AVR microcontroller,
 *   configuring the ADC with a 125kHz clock and allowing
 *   single-channel conversion reads. The `adc_read()` function selects
 *   a channel, performs a blocking conversion, and returns the 10-bit
 *   result.
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