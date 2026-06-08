

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



