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
#include <avr/io.h>
#include <avr/interrupt.h>
#include "events.h"
#include "hw_uart.h"

Event queue[EVENT_QUEUE_SIZE];
static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;

bool postEvent(Event event) {
    cli();
    uint8_t next = (head + 1) % EVENT_QUEUE_SIZE;
    if (next == tail){
        return false; // queue full
    }
    else{
        queue[head] = event;
        head = next;
        return true;
    }
    sei();
}

bool getEvent(Event *event) {
    cli();
    if (head == tail){
        return false; // empty
    }else{
        *event = queue[tail];
        tail = (tail + 1) % EVENT_QUEUE_SIZE;
        return true;
    }
    sei();
}

void logEvent(Event ev) {
    const char* names[] = {
        [EV_NONE]           = "EV_NONE",
        [EV_BUTTON_START]   = "EV_BUTTON_START",
        [EV_BUTTON_CANCEL]  = "EV_BUTTON_CANCEL",
        [EV_DOOR_OPEN]      = "EV_DOOR_OPEN",
        [EV_DOOR_CLOSED]    = "EV_DOOR_CLOSED",
        [EV_WATER_LEVEL_OK] = "EV_WATER_LEVEL_OK",
        [EV_TIMEOUT]        = "EV_TIMEOUT",
        [EV_MOTOR_ERROR]    = "EV_MOTOR_ERROR",
    };
    const char* name = (ev.type < EV_COUNT) ? names[ev.type] : "EV_UNKNOWN";
    const char* p = name;
    while(*p) hw_uart_queue(*p++);
    hw_uart_queue('\r');
    hw_uart_queue('\n');
}