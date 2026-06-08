/*
 * File:    gpio.c
 * Author:  Alexander130892
 * Date:    8-6-2026
 *
 * Description:
 *   This file initializes GPIO pins on an AVR microcontroller,
 *   configuring LED pins (internal, red, green, blue) and a piezo
 *   buzzer pin as outputs, while setting up a push button pin as
 *   input. The commented pull-up resistor line indicates optional
 *   input configuration for debouncing or noise reduction.
 */



#include <stdint.h>
#include "gpio.h"
#include <avr/io.h>

void gpio_init(void){
    /* Configure PB5 as output */
    LED_DDR     |= (1u << LED_INTERNAL_PIN);
    LED_DDR     |= (1u << LED_R_PIN);
    LED_DDR     |= (1u << LED_G_PIN);
    LED_DDR     |= (1u << LED_B_PIN);
    LED_DDR     |= (1u << PIEZZO_PIN);
    /* Configure PB2 as input */
    PB_DDR      &= ~(1u << PB_PIN);
    //PB_PORT     |= (1 << PB_PIN); // pull up resistor
}



