/*
 * File:    events.c
 * Author:  Alexander130892
 * Date:    13-6-2026
 *
 * Description:
 *   This file implements a circular event queue for an AVR
 *   microcontroller system, providing interrupt-safe functions to post
 *   and retrieve events, along with a logging function to output event
 *   names via UART. The queue uses head/tail pointers with atomic
 *   operations (cli/sei) to safely handle concurrent access between
 *   interrupt handlers and main code.
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
        [EV_LIMIT_OPEN]     = "EV_LIMIT_OPEN",
        [EV_LIMIT_CLOSED]   = "EV_LIMIT_CLOSED",
        [EV_START]          = "EV_START",
        [EV_PHASE_TIMEOUT]  = "EV_PHASE_TIMEOUT",
        [EV_WATER_TIMEOUT]  = "EV_WATER_TIMEOUT",
        [EV_CANCEL]         = "EV_CANCEL",
    };
    const char* name = (ev.type < EV_COUNT) ? names[ev.type] : "EV_UNKNOWN";
    const char* p = name;
    hw_uart_print(p);
}
