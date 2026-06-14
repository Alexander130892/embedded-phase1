/*
 * File:    gpio.c
 * Author:  Alexander130892
 * Date:    14-6-2026
 *
 * Description:
 *   This file initializes GPIO pins on an AVR microcontroller,
 *   configuring LED outputs (internal and RGB), a piezo buzzer output,
 *   and a push button input with pull-up resistor.
 */
#include <stdint.h>
#include "gpio.h"
#include <avr/io.h>
#include <stdbool.h>

void gpio_init(void){
    /* Configure PB5 as output */
    LED_DDR     |= (1u << LED_INTERNAL_PIN);
    LED_DDR     |= (1u << LED_R_PIN);
    LED_DDR     |= (1u << LED_G_PIN);
    LED_DDR     |= (1u << LED_B_PIN);
    PIEZZO_DDR     |= (1u << PIEZZO_PIN);
    /* Configure PB2 as input */
    PB_DDR      &= ~(1u << PB_PIN);
    PB_PORT     |= (1 << PB_PIN); // pull up resistor
}



