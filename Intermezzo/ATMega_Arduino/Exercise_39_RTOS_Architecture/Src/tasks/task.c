/*
 * File:    task.c
 * Author:  Alexander130892
 * Date:    8-6-2026
 *
 * Description:
 *   This file implements task functions for an AVR microcontroller
 *   system that manage hardware peripherals including three LEDs (red,
 *   green, blue), a buzzer, buttons, and UART communication. Each task
 *   function handles a specific peripheral operation—toggling LEDs,
 *   activating the buzzer, detecting button presses, and sending debug
 *   tick messages over UART.
 */

#include <stdint.h>
#include <stdbool.h>
#include "task.h"
#include "timer.h"
#include "buzzer.h"
#include "door_fsm.h"
#include "events.h"
#include "hw_uart.h"
#include "gpio.h"

static void taskButtons(void);
static void taskTick(void);
static void taskSerialInput(void);
static void taskProcessTimers(void);
static void taskHeartbeat(void);

static Task periodic_tasks[] = {
    { taskButtons, 10, 0 },
    { taskSerialInput, 10, 0 },
    { taskProcessTimers, 10, 0 },
    { taskTick, 1000, 0 },
    { taskHeartbeat, 500, 0 }    
};
static const uint8_t taskCount = sizeof(periodic_tasks) / sizeof(periodic_tasks[0]);

void taskButtons(void){
    static bool prev_state;
    static bool initialized = false;
    if(!initialized) {
        prev_state = readButton(PB_PIN);;
        initialized = true;
    }
    bool current_state = readButton(PB_PIN);
    if(!current_state && prev_state) {   // falling edge
        postEvent((Event){EV_BUTTON_START, 0});
    }
    prev_state = current_state;
}
void taskTick(void){
    hw_uart_print("Tick");
}
void taskSerialInput(void) {
    char str[MAX_LENGTH];
    if(hw_uart_receive_string(str) == STATUS_OK) {
        switch(str[0]) {
            case 's': postEvent((Event){EV_START, 0}); break;
            case 'c': postEvent((Event){EV_BUTTON_CANCEL, 0}); break;
            case 'd': postEvent((Event){EV_DOOR_OPEN, 0}); break;
            case 'o': postEvent((Event){EV_LIMIT_OPEN, 0}); break;
            case 'x': postEvent((Event){EV_LIMIT_CLOSED, 0}); break;
            case 'w': postEvent((Event){EV_WATER_LEVEL_OK, 0}); break;
            case 'C': postEvent((Event){EV_CANCEL, 0}); break;
            case 'b': postEvent((Event){EV_BUTTON_START, 0}); break;    // door button
            case 'e': postEvent((Event){EV_MOTOR_ERROR, 0}); break;     // motor error
            case 'u': postEvent((Event){EV_DOOR_UNLOCKED, 0}); break;   // door unlocked
            case 't': postEvent((Event){EV_PHASE_TIMEOUT, 0}); break;   // phase timeout
            case 'W': postEvent((Event){EV_WATER_TIMEOUT, 0}); break;   // water timeout
            default: break;
        }
    }
}

static void taskProcessTimers(void) {
    if(sw_timerExpired(&timer_buzzer)){
        postEvent((Event){EV_TIMEOUT, TIMEOUT_BUZZER_START});
    }
    if(sw_timerExpired(&timer_buzzer_off)){
        postEvent((Event){EV_TIMEOUT, TIMEOUT_BUZZER_STOP});
    }
    if(sw_timerExpired(&timer_autoClose)){
        postEvent((Event){EV_TIMEOUT, TIMEOUT_DOOR_AUTO_CLOSE});
    }
    if(sw_timerExpired(&timer_phase)){
        postEvent((Event){EV_PHASE_TIMEOUT, 0});
    }
}

static void taskHeartbeat(void) {
    ledToggle(LED_INTERNAL_PIN);
}

void schedulerRun(void) {
    uint32_t now = getTicks();
    for (uint8_t i = 0; i < taskCount; i++) {
        if (now - periodic_tasks[i].lastRun >= periodic_tasks[i].period) {
            periodic_tasks[i].lastRun = now;
            periodic_tasks[i].function();
        }
    }
}
