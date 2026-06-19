/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This file implements the main control loop for an AVR-based
 *   washing machine controller using an event-driven architecture with
 *   two finite state machines (door and wash cycle FSM). It
 *   initializes hardware peripherals, runs a continuous scheduler that
 *   processes events and triggers FSM state transitions, while
 *   monitoring CPU idle time to validate system efficiency.
 */
/*
    for wiring/mapping -- see gpio.h
    
    Notes:
        * Door_fsm no longer a real FSM
        * Motor_fsm, tried it but no real value since it directly maps to wm_fsm --> deleted
        * CPU LOAD (in debug mode)
        Idle:    32737
        Filling: 32715  > -22 iterations (~0.07% load)
        Washing: 32704  > -33 iterations (~0.10% load)
        Negligible CPU load even during active wash cycle — confirms the event-driven architecture is highly efficient.
        CPU-task disabled after test
        script
        * Jitter : variation in actual task time vs scheduled time
            Sources of jitter:
                Scheduler granularity — If a task takes 500µs, the next task's check is delayed by 500µs
                ISR preemption — USART_UDRE_vect and TIMER0_COMPA_vect can interrupt mid-loop, adding variable delay
                Event processing — processEvents drains the full queue before returning, variable duration depending on queue depth
    
            taskWater is the worst offender — 104µs ADC read blocks everything. Worst case jitter for any task scheduled around taskWater is 104µs.
            CPU idle counter confirms low load — ~35000 iterations/second baseline means the main loop runs in ~28µs per iteration on average.
            Conclusion: maximum jitter ~104µs from ADC read. 
            All tasks meet their periods comfortably given the slowest task period is 10ms 
            104µs is 1% of that budget.
        * Tested in simulation (terminal commands) and real hardware
        * Test-scenarios passed:
            [x] 1 Full wash cycle
            [x] 2 Cancel mid-cycle
            [x] 3 Door open during cycle
            [x] 4 water time-out during filling
            [x] 5 (Don't) start with door open
            [x] 6 Motor error (simulate with 'fault')
            [x] 7 Event queue overflow (temporarily queue size of 1)
            [x] 8 compile and test in NDEBUG mode
            [x] 9 final CPU load test
*/


#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include "adc.h"
#include "actuators.h"
#include "servo.h"
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

#define WAKEUP_DELAY 10000u

volatile uint32_t idle_count = 0u;

int main(void)
{
    gpio_init();    
    buzzer_init();
    adc_init();
    servo_init();
    actuators_init();
    hw_uart_init(MYUBRR);
    door_fsm_init();
    wm_fsm_init();
    timer0_init();
    sei();                  // Set Enable Interrupts
    for(volatile uint32_t d = 0; d < WAKEUP_DELAY; d++);  // settling delay
    for(;;) {
        schedulerRun();        // produce events from inputs/timers
        processEvents();       // consume events --> FSM transitions
        // door_fsm_do_actions();   // empty
        // wm_fsm_do_actions();     // empty
        idle_count++;           // increments when nothing else to do 
    }
    return 0;   // never reached
}


