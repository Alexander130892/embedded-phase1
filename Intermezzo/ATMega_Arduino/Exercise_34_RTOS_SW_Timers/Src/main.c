/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    8-6-2026
 *
 * Description:
 *   This is an Arduino Uno firmware that initializes GPIO, UART, and
 *   timers to manage LED blinking, button debouncing, and a buzzer
 *   with periodic task execution in a main loop. It communicates with
 *   an STM microcontroller via serial while monitoring a pushbutton
 *   input and controlling RGB LED outputs through PWM.
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

#define WAKEUP_DELAY 10000

int main(void)
{
    gpio_init();
    hw_uart_init(MYUBRR);
    sei();                  // Set Enable Interrupts
    timer0_init();
    timer1_pwm_init();
    sw_timerStart(&timer_LED, 500);
    sw_timerStart(&timer_uart, 1000);
    sw_timerStart(&timer_button_debounce, 200);

    for(volatile uint32_t d = 0; d < WAKEUP_DELAY; d++);  // settling delay
    for(;;) {
        if(b_button_pressed){
            sw_timerStart(&timer_buzzer, 2000);
            b_button_pressed = !b_button_pressed;
        }
        if(sw_timerExpired(&timer_buzzer)){
            taskBuzzer();
        }
        if(sw_timerExpired(&timer_buzzer_off)) {
            TCCR1A &= ~(1 << COM1A1);       // disconnect OC1A → buzzer off
        }
        if(sw_timerExpired(&timer_LED)){
            taskLed_R();
            taskLed_G();
            taskLed_B();
            sw_timerStart(&timer_LED, 500);
        }
        if(sw_timerExpired(&timer_uart)){
            taskTick();
            
            sw_timerStart(&timer_uart, 1000);
        }
        
        if(sw_timerExpired(&timer_button_debounce)){
            taskButtons();
            sw_timerStart(&timer_button_debounce, 200);
        }
    }
    return 0;   // never reached
}
