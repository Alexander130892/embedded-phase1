/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    8-6-2026
 *
 * Description:
 *   
 */
/*
    Notes:
        
    Mapping:
        
    Wiring:
        

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
#include "door_fsm.h"

#define WAKEUP_DELAY 10000

int main(void)
{
    gpio_init();
    door_fsm_init();
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
        if(sw_timerExpired(&timer_autoClose)){
            postEvent((Event){EV_TIMEOUT, TIMEOUT_DOOR_AUTO_CLOSE});
        }        
    }
    return 0;   // never reached
}

