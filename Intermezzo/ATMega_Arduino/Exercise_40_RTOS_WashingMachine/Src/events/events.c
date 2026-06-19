/*
 * File:    events.c
 * Author:  Alexander130892
 * Date:    14-6-2026
 *
 * Description:

 */



#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "events.h"
#include "hw_uart.h"

static event_t queue[EVENT_QUEUE_SIZE];
static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;

bool postEvent(event_t event) {
    bool result;
    bool queue_overflow = false;
    cli();
    uint8_t next = (head + 1) % EVENT_QUEUE_SIZE;
    if (next == tail){
        queue_overflow = true;
        result = false; // queue full
    }
    else{
        queue[head] = event;
        head = next;
        result = true;
    }
    sei();
    if(queue_overflow){
        #ifndef NDEBUG
        hw_uart_print("QUEUE OVERFLOW");   // debug -- ! dont print when isr are disabled
        #endif
    }
    return result;
}

bool getEvent(event_t *event) {
    bool result;
    cli();
    if (head == tail){
        result =  false; // empty
    }else{
        *event = queue[tail];
        tail = (tail + 1) % EVENT_QUEUE_SIZE;
        result = true;
    }
    sei();
    return result;
}

