#include <avr/io.h>
#include <util/delay.h>

/*
 * Bare-metal blink for ATmega328P (Arduino Uno)
 *
 * No Arduino runtime. No HAL. Just direct register access.
 *
 * LED = PB5 = Arduino pin 13 (built-in LED)
 *
 * Register map (from ATmega328P datasheet §14):
 *   DDRB  = Data Direction Register B  (1 = output, 0 = input)
 *   PORTB = Port B Data Register       (write output value)
 *   PINB  = Port B Input Pins Register (read input value)
 *
 * PB5 = bit 5 of port B
 */

#define LED_DDR   DDRB
#define LED_PORT  PORTB
#define LED_PIN   PB5          /* defined in avr/iom328p.h as (5) */

int main(void)
{
    /* Configure PB5 as output */
    LED_DDR |= (1 << LED_PIN);

    while (1) {
        LED_PORT |=  (1 << LED_PIN);   /* set high  → LED on  */
        _delay_ms(500);

        LED_PORT &= ~(1 << LED_PIN);   /* set low   → LED off */
        _delay_ms(500);
    }

    return 0;   /* never reached */
}