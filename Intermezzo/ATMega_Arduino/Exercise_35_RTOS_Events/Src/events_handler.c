// event_handler.c
#include "events_handler.h"
#include "timer.h"
#include "hw_uart.h"
#include <avr/io.h>

// Only handleEvent should call these, so static and not in .h
static void onButtonStart(Event ev);
static void onButtonCancel(Event ev);
static void onDoorOpen(Event ev);
static void onDoorClosed(Event ev);
static void onWaterLevelOk(Event ev);
static void onTimeout(Event ev);
static void onMotorError(Event ev);

typedef void (*EventHandler)(Event ev);

//designated initializer [] = function to coop with enum
static const EventHandler handlers[] = {
    [EV_BUTTON_START]   = onButtonStart,
    [EV_BUTTON_CANCEL]  = onButtonCancel,
    [EV_DOOR_OPEN]      = onDoorOpen,
    [EV_DOOR_CLOSED]    = onDoorClosed,
    [EV_WATER_LEVEL_OK] = onWaterLevelOk,
    [EV_TIMEOUT]        = onTimeout,
    [EV_MOTOR_ERROR]    = onMotorError,
};

void handleEvent(Event ev) {
    if(ev.type != EV_NONE && ev.type < EV_COUNT && handlers[ev.type]) {
        handlers[ev.type](ev);
    }
}

static void onButtonStart(Event ev) {
    (void)ev;
    sw_timerStart(&timer_buzzer, 2000);
    hw_uart_queue('B');
    hw_uart_queue('T');
    hw_uart_queue('N');
    hw_uart_queue('\r');
    hw_uart_queue('\n');
}

static void onButtonCancel(Event ev)    {
    (void)ev;
    hw_uart_queue('C');
    hw_uart_queue('a');
    hw_uart_queue('n');
    hw_uart_queue('c');
    hw_uart_queue('e');
    hw_uart_queue('l');
    hw_uart_queue('\r');
    hw_uart_queue('\n');
}
static void onDoorOpen(Event ev)        {
    (void)ev;
    hw_uart_queue('D');
    hw_uart_queue('_');
    hw_uart_queue('O');
    hw_uart_queue('p');
    hw_uart_queue('e');
    hw_uart_queue('n');
    hw_uart_queue('\r');
    hw_uart_queue('\n');
}
static void onDoorClosed(Event ev)      {(void)ev;}
static void onWaterLevelOk(Event ev)    {(void)ev;}
static void onTimeout(Event ev)         { 
    switch(ev.value) {
        case TIMEOUT_BUZZER_START:
                TCCR1A |= (1 << COM1A1);        // connect OC1A → buzzer on
                sw_timerStart(&timer_buzzer_off, 500);
            break;
        case TIMEOUT_BUZZER_STOP:
            TCCR1A &= ~(1 << COM1A1);
            break;
    }
}
static void onMotorError(Event ev)      {(void)ev;}