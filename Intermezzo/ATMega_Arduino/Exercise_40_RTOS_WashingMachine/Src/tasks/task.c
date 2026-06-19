/*
 * File:    task.c
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This file implements a real-time task scheduler for a washing
 *   machine controller, managing periodic tasks like button input
 *   detection, door/water sensor monitoring, LED status updates, and
 *   serial command processing at different intervals (10–1000 ms). It
 *   posts events to state machines (door and washing machine FSMs)
 *   based on hardware inputs and commands received via UART.
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "task.h"
#include "timer.h"
#include "wm_fsm.h"
#include "door_fsm.h"
#include "adc.h"
#include "events.h"
#include "hw_uart.h"
#include "buzzer.h"
#include "gpio.h"

//only called from tasks.c, so static prototypes
static void taskButtons(void);
static void taskTick(void);
static void taskDoor(void);
static void taskWater(void);
static void taskStatusLEDs(void);
static void taskSerialInput(void);
static void taskProcessTimers(void);
static void taskHeartbeat(void);
static void taskCpuLoad(void);
static bool detectFallingEdge(bool current, bool *prev);
static task_t periodic_tasks[] = {
    { taskButtons,      10,  0 },
    { taskDoor,         20,  0 },
    { taskWater,        50,  0 },
    { taskProcessTimers, 10, 0 },
    { taskStatusLEDs,   100, 0 },
    { taskSerialInput,   50, 0 },
    { taskTick,        1000, 0 },
    { taskHeartbeat,    500, 0 },
    { taskCpuLoad,    1000, 0 }
};
static const uint8_t taskCount = sizeof(periodic_tasks) / sizeof(periodic_tasks[0]);



static bool detectFallingEdge(bool current, bool *prev) {
    bool edge = (!current && *prev);
    *prev = current;
    return edge;
}
static void taskButtons(void) {
    static bool prev_start  = true;
    static bool prev_cancel = true;
    if (detectFallingEdge(readStartButton(), &prev_start)) {
        postEvent((event_t){EV_START, 0});
    }
    if (detectFallingEdge(readCancelButton(), &prev_cancel)) {
        postEvent((event_t){EV_CANCEL, 0});
    }
}
static void taskDoor(void) {
    static bool prev = true;    // idle high = door open
    bool current = readDoor();
    if (current && !prev) {     // rising edge = door opened
        postEvent((event_t){EV_DOOR_OPEN, 0});
        door_fsm_update(DOOR_OPEN);
    }
    if (!current && prev) {     // falling edge = door closed
        postEvent((event_t){EV_DOOR_CLOSED, 0});
        door_fsm_update(DOOR_CLOSED);
    }
    prev = current;
}
static void taskWater(void) {
    static bool prev_level_ok = false;
    bool current_level_ok = (adc_read(WATER_LEVEL_CH) >= WATER_LEVEL_THRESH);

    if (current_level_ok && !prev_level_ok) {   // rising edge 
        postEvent((event_t){EV_WATER_LEVEL_OK, 0});
    }
    prev_level_ok = current_level_ok;
}
static void taskStatusLEDs(void) {
    switch (wm_fsm_get_state()) {
        case WM_IDLE:
        case WM_DONE:
            ledBlueOff();
            ledGreenOn();
            ledRedOff();
            break;
        case WM_FILLING:
        case WM_WASHING:
        case WM_RINSING:
        case WM_SPINNING:
        case WM_DRAINING_AFTER_WASH:
        case WM_DRAINING_AFTER_RINSE:
            ledBlueOn();
            ledGreenOff();
            ledRedOff();
            break;
        case WM_ERROR:
            ledBlueOff();
            ledGreenOff();
            ledRedOn();
            break;
    }
}
void taskTick(void){
    #ifndef NDEBUG
    hw_uart_print("Tick");
    //hw_uart_print_uint32(getTicks()); //print actual mseconds that have passed since start
    #endif
}
static void taskSerialInput(void) {
    char str[UART_BUFFER_SIZE];
    if (hw_uart_receive_string(str) == STATUS_OK) {
        if      (strcmp(str, "start")   == 0) { postEvent((event_t){EV_START, 0});          }
        else if (strcmp(str, "cancel")  == 0) { postEvent((event_t){EV_CANCEL, 0});         }
        else if (strcmp(str, "door")    == 0) { 
            postEvent((event_t){EV_DOOR_OPEN, 0});
            door_fsm_update(DOOR_OPEN);
        }
        else if (strcmp(str, "close")   == 0) { 
            postEvent((event_t){EV_DOOR_CLOSED, 0});
            door_fsm_update(DOOR_CLOSED);
        }
        else if (strcmp(str, "water")   == 0) { postEvent((event_t){EV_WATER_LEVEL_OK, 0}); }
        else if (strcmp(str, "fault")   == 0) { postEvent((event_t){EV_MOTOR_ERROR, 0});    }
        else if (strcmp(str, "timeout") == 0) { postEvent((event_t){EV_PHASE_TIMEOUT, 0});  }
        else if (strcmp(str, "status")  == 0) { 
            hw_uart_print(wm_fsm_get_state_name());
            hw_uart_print(wm_fsm_get_error_name());
        }
        else {
        #ifndef NDEBUG
        hw_uart_print("Unknown command");
        #endif
        }
    }
}
static void taskProcessTimers(void) {
    if(sw_timerExpired(&timer_buzzer)){
        postEvent((event_t){EV_TIMEOUT, 0});
    }
    if(sw_timerExpired(&timer_phase)){
        postEvent((event_t){EV_PHASE_TIMEOUT, 0});
    }
    if (sw_timerExpired(&timer_water)) {
        postEvent((event_t){EV_WATER_TIMEOUT, 0});
    }   
}
static void taskHeartbeat(void) {
    ledHbToggle();
}
static void taskCpuLoad(void) {
    static uint32_t prev_idle = 0u;
    uint32_t delta = idle_count - prev_idle;
    prev_idle = idle_count;
    // delta = idle iterations per second
    // higher = more idle time = lower CPU load 
    #ifndef NDEBUG
    hw_uart_print("CPU IDLE: ");
    hw_uart_print_uint32(delta);
    #endif
}
void schedulerRun(void) {
    uint32_t now = getTicks();
    for (uint8_t i = 0; i < taskCount; i++) {
        if (now - periodic_tasks[i].last_run >= periodic_tasks[i].period) {
            periodic_tasks[i].last_run = now;
            periodic_tasks[i].function();
        }
    }
}
