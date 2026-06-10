/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    10-6-2026
 *
 * Description:
 *   This file implements the main control loop for an AVR
 *   microcontroller system that initializes GPIO, UART, and timers,
 *   then continuously processes events from a queue (such as button
 *   presses) and handles timeouts for a buzzer. The system uses an
 *   event-driven architecture to manage hardware interactions and
 *   logging via serial communication.
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
#include "gpio.h"
#include "task.h"
#include "hw_uart.h"
#include "events.h"
#include "events_handler.h"

#define WAKEUP_DELAY 10000

// TEST ONLY
void taskQueueFlood(void) {
    for(uint8_t i = 0; i < 10; i++) {
        bool result = postEvent((Event){EV_BUTTON_START, 0});
        if(!result) {
            hw_uart_queue('F');   // queue full
            hw_uart_queue('U');
            hw_uart_queue('L');
            hw_uart_queue('L');
            hw_uart_queue('\r');
            hw_uart_queue('\n');
        }
    }
}


int main(void)
{
    gpio_init();
    hw_uart_init(MYUBRR);
    sei();                  // Set Enable Interrupts
    timer0_init();
    timer1_pwm_init();
    sw_timerStart(&timer_uart, 1000);
    for(volatile uint32_t d = 0; d < WAKEUP_DELAY; d++);  // settling delay
    for(;;) {
        schedulerRun();
        //taskQueueFlood(); // Test only -- 
        Event ev;
        if(getEvent(&ev)) {
            logEvent(ev);      // print event name to terminal
            handleEvent(ev);
        }
        if(sw_timerExpired(&timer_buzzer)){
            postEvent((Event){EV_TIMEOUT, TIMEOUT_BUZZER_START});
        }
        if(sw_timerExpired(&timer_buzzer_off)){
            postEvent((Event){EV_TIMEOUT, TIMEOUT_BUZZER_STOP});
        }        
    }
    return 0;   // never reached
}

