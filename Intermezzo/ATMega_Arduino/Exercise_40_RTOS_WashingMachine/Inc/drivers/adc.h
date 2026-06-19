/*
 * File:    adc.h
 * Author:  Alexander130892
 * Date:    10-6-2026
 *
 * Description:
 *   ADC driver interface for ATmega328P.
 *   Configured for 10-bit single conversion mode,
 *   AVcc reference, prescaler 128 (125kHz @ 16MHz).
 */

#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void        adc_init(void);
uint16_t    adc_read(uint8_t channel);

#endif /* ADC_H */