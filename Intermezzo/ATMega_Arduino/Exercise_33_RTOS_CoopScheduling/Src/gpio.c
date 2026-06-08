

#include <stdint.h>
#include "gpio.h"
#include <avr/io.h>

void gpio_init(void){
    /* Configure PB5 as output */
    LED_DDR     |= (1 << LED_INTERNAL_PIN);
    LED_DDR     |= (1 << LED_R_PIN);
    LED_DDR     |= (1 << LED_G_PIN);
    LED_DDR     |= (1 << LED_B_PIN);
    /* Configure PB2 as input */
    PB_DDR      &= ~(1 << PB_PIN);
    //PB_PORT     |= (1 << PB_PIN); // pull up resistor
}



