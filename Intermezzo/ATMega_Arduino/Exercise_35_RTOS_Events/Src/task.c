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
#include <avr/io.h>
#include "task.h"
#include "timer.h"
#include "events.h"
#include "hw_uart.h"
#include "gpio.h"
#include <stdbool.h>

static void taskLed_R(void);
static void taskLed_G(void);
static void taskLed_B(void);
static void taskButtons(void);
static void taskTick(void);
static void taskSerialInput(void);

static Task periodic_tasks[] = {
    { taskLed_R, 500, 0 },
    { taskLed_G, 750, 0 },
    { taskLed_B, 1000, 0 },
    { taskButtons, 10, 0 },
    { taskSerialInput, 10, 0 },
    { taskTick, 1000, 0 }
};
static const uint8_t taskCount = sizeof(periodic_tasks) / sizeof(periodic_tasks[0]);

void taskLed_R(void){
    LED_PORT ^=  (1 << LED_R_PIN);   /* Toggle LED */
}
void taskLed_G(void){
    LED_PORT ^=  (1 << LED_G_PIN);   /* Toggle LED */
}
void taskLed_B(void){
    LED_PORT ^=  (1 << LED_B_PIN);   /* Toggle LED */
}
void taskButtons(void){
    static bool prev_state;
    static bool initialized = false;
    if(!initialized) {
        prev_state = (PIND & (1u << PB_PIN));
        initialized = true;
    }
    bool current_state = (PIND & (1u << PB_PIN));
    if(!current_state && prev_state) {   // falling edge
        postEvent((Event){EV_BUTTON_START, 0});
    }
    prev_state = current_state;
}
void taskTick(void){
    hw_uart_queue('T');
    hw_uart_queue('i');
    hw_uart_queue('c');
    hw_uart_queue('k');
    hw_uart_queue('\r');
    hw_uart_queue('\n');
}
void taskSerialInput(void) {
    char str[MAX_LENGTH];
    if(hw_uart_receive_string(str) == STATUS_OK) {
        switch(str[0]) {
            case 's': postEvent((Event){EV_BUTTON_START, 0}); break;
            case 'c': postEvent((Event){EV_BUTTON_CANCEL, 0}); break;
            case 'd': postEvent((Event){EV_DOOR_OPEN, 0}); break;
            default: break;
        }
    }
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
