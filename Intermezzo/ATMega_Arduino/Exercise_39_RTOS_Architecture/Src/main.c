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


#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

#include "uart.h"
#include "timer.h"
#include "buzzer.h"
#include "gpio.h"
#include "task.h"
#include "hw_uart.h"
#include "events.h"
#include "events_handler.h"
#include "door_fsm.h"
#include "wm_fsm.h"

#define WAKEUP_DELAY 10000



int main(void)
{
    gpio_init();
    buzzer_init();
    hw_uart_init(MYUBRR);
    door_fsm_init();
    wm_fsm_init();
    sei();                  // Set Enable Interrupts
    timer0_init();
    sw_timerStart(&timer_LED, 500);
    for(volatile uint32_t d = 0; d < WAKEUP_DELAY; d++);  // settling delay
    for(;;) {
        schedulerRun();        // produce events from inputs/timers
        processEvents();       // consume events --> FSM transitions
        door_fsm_do_actions(); // periodic actions for current state
        wm_fsm_do_actions();
    }
    return 0;   // never reached
}


