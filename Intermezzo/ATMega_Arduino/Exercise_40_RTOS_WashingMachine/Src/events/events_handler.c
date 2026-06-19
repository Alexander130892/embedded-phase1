// event_handler.c
#include "events_handler.h"
#include "timer.h"
#include "hw_uart.h"
#include "door_fsm.h"
#include "buzzer.h"
#include "wm_fsm.h"

// Only handleEvent should call these, so static and not in .h
static void onDoorOpen(event_t ev);
static void onDoorClosed(event_t ev);
static void onWaterLevelOk(event_t ev);
static void onTimeout(event_t ev);
static void onWaterTimeout(event_t ev);
static void onMotorError(event_t ev);
static void onStart(event_t ev);
static void onCancel(event_t ev);
static void onPhaseTimeOut(event_t ev);

typedef void (*event_handler_fn_t)(event_t ev);

//designated initializer [] = function to coop with enum
static const event_handler_fn_t handlers[] = {  
    [EV_DOOR_OPEN]      = onDoorOpen,       // door fully open    
    [EV_DOOR_CLOSED]    = onDoorClosed,     // door closed        
    [EV_WATER_LEVEL_OK] = onWaterLevelOk,
    [EV_WATER_TIMEOUT]  = onWaterTimeout,
    [EV_TIMEOUT]        = onTimeout,
    [EV_MOTOR_ERROR]    = onMotorError,
    [EV_START]          = onStart,
    [EV_CANCEL]         = onCancel,
    [EV_PHASE_TIMEOUT]  = onPhaseTimeOut,
};

void handleEvent(event_t ev) {
    if(ev.type != EV_NONE && ev.type < EV_COUNT && handlers[ev.type]) {
        handlers[ev.type](ev);
    }
}
static void onDoorOpen(event_t ev){
    wm_fsm_handle(ev);
}
static void onDoorClosed(event_t ev){
    wm_fsm_handle(ev);
}
static void onWaterLevelOk(event_t ev){
    wm_fsm_handle(ev);
}
static void onTimeout(event_t ev){ 
    (void)ev;
    buzzerOff();
}
static void onWaterTimeout(event_t ev){
    wm_fsm_handle(ev);
}
static void onPhaseTimeOut(event_t ev){
    wm_fsm_handle(ev);
}
static void onMotorError(event_t ev){
     wm_fsm_handle(ev);
}
static void onStart(event_t ev){
    wm_fsm_handle(ev);
}
static void onCancel(event_t ev){
    wm_fsm_handle(ev);
}

void processEvents(void) {
    event_t ev;
    while(getEvent(&ev)) {
        logEvent(ev);
        handleEvent(ev);
    }
}

static const char* const names[] = {
    [EV_NONE]           = "EV_NONE",
    [EV_DOOR_OPEN]      = "EV_DOOR_OPEN",
    [EV_DOOR_CLOSED]    = "EV_DOOR_CLOSED",
    [EV_WATER_LEVEL_OK] = "EV_WATER_LEVEL_OK",
    [EV_TIMEOUT]        = "EV_TIMEOUT",
    [EV_MOTOR_ERROR]    = "EV_MOTOR_ERROR",
    [EV_START]          = "EV_START",
    [EV_PHASE_TIMEOUT]  = "EV_PHASE_TIMEOUT",
    [EV_WATER_TIMEOUT]  = "EV_WATER_TIMEOUT",
    [EV_CANCEL]         = "EV_CANCEL",
};

void logEvent(event_t ev) {
     #ifndef NDEBUG
    const char* name = (ev.type < EV_COUNT) ? names[ev.type] : "EV_UNKNOWN";
    const char* p = name;
    hw_uart_print(p);
    #endif
}
