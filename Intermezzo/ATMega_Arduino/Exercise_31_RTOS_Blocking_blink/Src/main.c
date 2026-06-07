/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    30-5-2026
 *
 * Description:
 *   This Arduino (AVR) program implements a software UART transmitter
 *   that sends three bytes (0x1, 0x2, 0x3) to an STM32 microcontroller
 *   at 9600 baud, while simultaneously receiving ASCII characters from
 *   the STM32 via software UART and echoing them to the serial monitor
 *   via hardware UART.
 */
/*
    Notes:
        
    Mapping:
        Rx   -   D6  -   PD6     
        Tx   -   D7  -   PD7
        

    Wiring:
        PD6 (UnoRx) -- PA0 (STM Tx)     -- Ch 1 (Logic Analyzer)
        PD7 (UnoTx) -- PA1 (STM Rx)     -- Ch 2 (Logic Analyzer)
        GND         --  GND             -- GND  (Logic Analyzer)
        USB -- STM  -- 5V -- 5V (Uno)

*/

/*
Exercise:
    blink LED, use blocking delay();

    Try to read Pushbotton press.
    Output to serial when button is pressed
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include "uart.h"
#include "hw_uart.h"

#define WAKEUP_DELAY 10000

#define LED_DDR   DDRB
#define LED_PORT  PORTB
#define LED_PIN   PB5 

#define PB_DDR   DDRD
#define PB_PORT  PORTD
#define PB_PIN   PD2 

int main(void)
{
    /* Configure PB5 as output */
    LED_DDR |= (1 << LED_PIN);
    /* Configure PB2 as input */
    PB_DDR &= ~(1 << PB_PIN);
    PB_PORT   |= (1 << PB_PIN);

    hw_uart_init(MYUBRR);
    sei();                  // Set Enable Interrupts

    for(volatile uint32_t d = 0; d < WAKEUP_DELAY; d++);  // settling delay
    for(;;) {
        LED_PORT |=  (1 << LED_PIN);   /* set high  LED on  */
        _delay_ms(1000);
        LED_PORT &= ~(1 << LED_PIN);   /* set low   LED off */
        _delay_ms(1000);
        if(PIND & (1u << PB_PIN)){
            hw_uart_transmit('P');
            hw_uart_transmit('B');
            hw_uart_transmit('\r');
            hw_uart_transmit('\n');
        }
    }
    return 0;   // never reached
}