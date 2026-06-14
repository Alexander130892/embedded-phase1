/*
 * File:    wm_fsm.c
 * Author:  Alexander130892
 * Date:    14-6-2026
 *
 * Description:
 *   This file implements a finite state machine (FSM) for a washing
 *   machine that manages the complete wash cycle from idle through
 *   filling, washing, rinsing, spinning, and done states, responding
 *   to events like start commands, timeouts, door openings, and
 *   cancellations. It coordinates with a door FSM to ensure the door
 *   is closed before operation and transitions between wash phases
 *   based on timers and water level events.
 */
#include "events.h"
#include "wm_fsm.h"
#include "hw_uart.h"
#include "timer.h"
#include "door_fsm.h"

static WmState wm_state = WM_IDLE;

static void errorHandle(void);
static void cancelHandle(void);

void wm_fsm_init(void){
    wm_state = WM_IDLE;
}
WmState wm_fsm_get_state(void){
    return wm_state;
}
void wm_fsm_handle(Event ev){
    switch(wm_state){
        case WM_IDLE:  
            if(ev.type == EV_START && door_fsm_get_state()==DOOR_CLOSED){
                wm_state = WM_FILLING;
                sw_timerStart(&timer_phase, DURATION_FILLING_MS);
               hw_uart_print("STATE: Filling");
               
            }else if(ev.type == EV_START && door_fsm_get_state()!=DOOR_CLOSED){
                errorHandle();
            }else if(ev.type == EV_BUTTON_CANCEL){
                cancelHandle();
            }
            break;
        case WM_FILLING:  
            if(ev.type == EV_WATER_LEVEL_OK){
                wm_state = WM_WASHING;
                sw_timerStart(&timer_phase, DURATION_WASHING_MS);
               hw_uart_print("STATE: Washing");
            } else if(ev.type == EV_DOOR_OPEN){
                errorHandle();
                hw_uart_print("ERROR: DOOR Open");
            } else if(ev.type == EV_PHASE_TIMEOUT){
                errorHandle();
            } else if(ev.type == EV_BUTTON_CANCEL){
                cancelHandle();
            }
            break;
        case WM_WASHING:   
            if(ev.type == EV_PHASE_TIMEOUT){
                wm_state = WM_DRAINING_AFTER_WASH;
                sw_timerStart(&timer_phase, DURATION_DRAINING_MS);
                hw_uart_print("STATE: Draining");
            }else if(ev.type == EV_DOOR_OPEN){
                errorHandle();
                hw_uart_print("ERROR: DOOR Open");
            }else if(ev.type == EV_BUTTON_CANCEL){
                cancelHandle();
            }
            break;
        case WM_DRAINING_AFTER_WASH:
            if(ev.type == EV_PHASE_TIMEOUT){
                wm_state = WM_RINSING;
                sw_timerStart(&timer_phase, DURATION_RINSING_MS);
               hw_uart_print("STATE: Rinsing");
            }else if(ev.type == EV_DOOR_OPEN){
                errorHandle();
            }else if(ev.type == EV_BUTTON_CANCEL){
                cancelHandle();
            }
            break;
        case WM_RINSING:
            if(ev.type == EV_PHASE_TIMEOUT){
                wm_state = WM_DRAINING_AFTER_RINSE;
                sw_timerStart(&timer_phase, DURATION_DRAINING_MS);
                hw_uart_print("STATE: Draining");
            }else if(ev.type == EV_DOOR_OPEN){
                errorHandle();
            }else if(ev.type == EV_BUTTON_CANCEL){
                cancelHandle();
            }
            break;
        case WM_DRAINING_AFTER_RINSE:
            if(ev.type == EV_PHASE_TIMEOUT){
                wm_state = WM_SPINNING;
                sw_timerStart(&timer_phase, DURATION_SPINNING_MS);
                hw_uart_print("STATE: Spinning");
            }else if(ev.type == EV_DOOR_OPEN){
                errorHandle();
            }else if(ev.type == EV_BUTTON_CANCEL){
                cancelHandle();
            }
            break;
        case WM_SPINNING:
            if(ev.type == EV_PHASE_TIMEOUT){
                wm_state = WM_DONE;
                hw_uart_print("STATE: Done");
            }else if(ev.type == EV_DOOR_OPEN){
                errorHandle();
            }else if(ev.type == EV_BUTTON_CANCEL){
                cancelHandle();
            }
            break;
        case WM_DONE:
            if(ev.type == EV_BUTTON_CANCEL || ev.type == EV_START){
                cancelHandle();   // reuse — both reset to IDLE
            }
            break;
        case WM_ERROR:
            if(ev.type == EV_BUTTON_CANCEL) {
                cancelHandle();
            }
            break;
    }
}
void wm_fsm_do_actions(void){

}

void errorHandle(void){
    timer_phase.active = false;   // stop phase timer
    wm_state = WM_ERROR;
    hw_uart_print("STATE: Error");
}

void cancelHandle(void){
    timer_phase.active = false;   // stop phase timer
    wm_state = WM_IDLE;
    hw_uart_print("STATE: IDLE");
}
