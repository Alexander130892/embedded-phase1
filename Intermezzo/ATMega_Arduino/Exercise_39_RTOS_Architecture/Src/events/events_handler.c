// event_handler.c
#include "events_handler.h"
#include "timer.h"
#include "hw_uart.h"
#include "door_fsm.h"
#include "buzzer.h"
#include "wm_fsm.h"

// Only handleEvent should call these, so static and not in .h
static void onButtonStart(Event ev);
static void onButtonCancel(Event ev);
static void onDoorOpen(Event ev);
static void onDoorClosed(Event ev);
static void onDoorUnlocked(Event ev);
static void onWaterLevelOk(Event ev);
static void onTimeout(Event ev);
static void onMotorError(Event ev);
static void onLimitOpen(Event ev);
static void onLimitClosed(Event ev);
static void onStart(Event ev);
static void onCancel(Event ev);
static void onPhaseTimeOut(Event ev);

typedef void (*EventHandler)(Event ev);

//designated initializer [] = function to coop with enum
static const EventHandler handlers[] = {
    [EV_BUTTON_START]   = onButtonStart,
    [EV_BUTTON_CANCEL]  = onButtonCancel,
    [EV_DOOR_OPEN]      = onDoorOpen,
    [EV_DOOR_UNLOCKED]  = onDoorUnlocked,
    [EV_DOOR_CLOSED]    = onDoorClosed,
    [EV_WATER_LEVEL_OK] = onWaterLevelOk,
    [EV_TIMEOUT]        = onTimeout,
    [EV_MOTOR_ERROR]    = onMotorError,
    [EV_LIMIT_OPEN]     = onLimitOpen,
    [EV_LIMIT_CLOSED]   = onLimitClosed,
    [EV_START]          = onStart,
    [EV_CANCEL]         = onCancel,
    [EV_PHASE_TIMEOUT]  = onPhaseTimeOut,
};

void handleEvent(Event ev) {
    if(ev.type != EV_NONE && ev.type < EV_COUNT && handlers[ev.type]) {
        handlers[ev.type](ev);
    }
}
static void onButtonStart(Event ev) {
    door_fsm_handle(ev);
}
static void onButtonCancel(Event ev)    {
    wm_fsm_handle(ev);
}
static void onDoorOpen(Event ev)        {
    wm_fsm_handle(ev);
}
static void onDoorClosed(Event ev)      {
    wm_fsm_handle(ev);
}
static void onDoorUnlocked(Event ev)      {
    wm_fsm_handle(ev);
}
static void onWaterLevelOk(Event ev)    {
    wm_fsm_handle(ev);
}
static void onTimeout(Event ev)         { 
    switch(ev.value) {
        case TIMEOUT_BUZZER_START:
                buzzerOn();
                sw_timerStart(&timer_buzzer_off, 500);
            break;
        case TIMEOUT_BUZZER_STOP:
            buzzerOff();
            break;
        case TIMEOUT_DOOR_AUTO_CLOSE:
            door_fsm_handle(ev);
            break;
    }
}
static void onMotorError(Event ev){
    (void)ev;
}
static void onLimitOpen(Event ev){
    door_fsm_handle(ev);
}
static void onLimitClosed(Event ev){
    door_fsm_handle(ev);
}
static void onStart(Event ev){
    wm_fsm_handle(ev);
}
static void onCancel(Event ev){
    wm_fsm_handle(ev);
}
static void onPhaseTimeOut(Event ev){
    wm_fsm_handle(ev);
}
void processEvents(void) {
    Event ev;
    while(getEvent(&ev)) {
        logEvent(ev);
        handleEvent(ev);
    }
}