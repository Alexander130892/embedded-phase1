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
#include "timer.h"
#include "hw_uart.h"

#define WAKEUP_DELAY 10000

#define LED_DDR   DDRB
#define LED_PORT  PORTB
#define LED_INTERNAL_PIN   PB5
#define LED_R_PIN   PB2
#define LED_G_PIN   PB3
#define LED_B_PIN   PB4 

#define PB_DDR   DDRD
#define PB_PORT  PORTD
#define PB_PIN   PD2 

int main(void)
{
    /* Configure PB5 as output */
    LED_DDR     |= (1 << LED_INTERNAL_PIN);
    LED_DDR     |= (1 << LED_R_PIN);
    LED_DDR     |= (1 << LED_G_PIN);
    LED_DDR     |= (1 << LED_B_PIN);
    /* Configure PB2 as input */
    PB_DDR      &= ~(1 << PB_PIN);
    //PB_PORT     |= (1 << PB_PIN); // pull up resistor

    hw_uart_init(MYUBRR);
    sei();                  // Set Enable Interrupts

    uint32_t prev_time_r = 0;
    uint32_t prev_time_g = 0;
    uint32_t prev_time_b = 0;
    uint32_t prev_time_uart = 0;
    uint32_t prev_time_pb = 0;
    const uint32_t period_r = 500;
    const uint32_t period_g = 750;
    const uint32_t period_b = 1000;
    const uint32_t period_uart = 1000;
    const uint32_t period_pb = 200;
    timer0_init();
    for(volatile uint32_t d = 0; d < WAKEUP_DELAY; d++);  // settling delay
    for(;;) {
        uint32_t now_time = millis();
        if((now_time-prev_time_r) > period_r){
            LED_PORT ^=  (1 << LED_R_PIN);   /* Toggle LED */
            prev_time_r = now_time;
        }
        if((now_time-prev_time_g) > period_g){
            LED_PORT ^=  (1 << LED_G_PIN);   /* Toggle LED */
            prev_time_g = now_time;
        }
        if((now_time-prev_time_b) > period_b){
            LED_PORT ^=  (1 << LED_B_PIN);   /* Toggle LED */
            prev_time_b = now_time;
        }
        if((now_time-prev_time_uart) > period_uart){
            hw_uart_queue('S');
            hw_uart_queue('\r');
            hw_uart_queue('\n');
            prev_time_uart = now_time;
        }
        if((now_time-prev_time_pb) > period_pb){
            if((PIND & (1u << PB_PIN))){
                hw_uart_queue('P');
                hw_uart_queue('B');
                hw_uart_queue('\r');
                hw_uart_queue('\n');
                
            }
            prev_time_pb = now_time;
        }
        hw_uart_drain();
    }
    return 0;   // never reached
}