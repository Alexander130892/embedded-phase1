/*
 * File:    adc.h
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This header file declares the interface for an ADC
 *   (Analog-to-Digital Converter) module, providing functions to
 *   initialize the ADC and read analog values from a specified
 *   channel.
 */
#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void        adc_init(void);
uint16_t    adc_read(uint8_t channel);

#endif /* ADC_H */