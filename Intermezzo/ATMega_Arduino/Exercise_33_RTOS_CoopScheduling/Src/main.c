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
#include "gpio.h"
#include "task.h"
#include "hw_uart.h"

#define WAKEUP_DELAY 10000

void schedulerRun(void);

Task tasks[] = {
    { taskLed_R, 500, 0 },
    { taskLed_G, 750, 0 },
    { taskLed_B, 1000, 0 },
    { taskButtons, 10, 0 },
    { taskTick, 1000, 0 },
    { taskSerial, 1, 0 }
};
const uint8_t taskCount = sizeof(tasks) / sizeof(tasks[0]);

int main(void)
{
    gpio_init();
    hw_uart_init(MYUBRR);
    sei();                  // Set Enable Interrupts
    timer0_init();
    for(volatile uint32_t d = 0; d < WAKEUP_DELAY; d++);  // settling delay
    for(;;) {
        schedulerRun();
    }
    return 0;   // never reached
}

void schedulerRun(void) {
    unsigned long now = millis();
    for (uint8_t i = 0; i < taskCount; i++) {
        if (now - tasks[i].lastRun >= tasks[i].period) {
        tasks[i].lastRun = now;
        tasks[i].function();
        }
    }
}