
#include "events.h"
#include "buzzer.h"
#include "actuators.h"
#include "wm_fsm.h"
#include "hw_uart.h"
#include "timer.h"
#include "door_fsm.h"

static wm_state_t wm_state = WM_IDLE;
wm_error_t wm_last_error = WM_ERR_NONE;

static void errorHandle(void);
static void cancelHandle(void);

void wm_fsm_init(void){
    wm_state = WM_IDLE;
}
wm_state_t wm_fsm_get_state(void){
    return wm_state;
}
void wm_fsm_handle(event_t ev){
    // Global transitions — checked before state-specific logic 
    // not applied in DONE or ERROR */
    if (wm_state != WM_DONE && wm_state != WM_ERROR) {
        if (ev.type == EV_MOTOR_ERROR) {
            wm_last_error = WM_ERR_MOTOR;
            errorHandle();
            return;
        }
        if (ev.type == EV_CANCEL) {
            cancelHandle();
            return;
        }
    }
    switch(wm_state){
        case WM_IDLE:  
            if(ev.type == EV_START && door_fsm_get_state()==DOOR_CLOSED){
                // exit state
                // (no exit actions)
                
                // State change
                wm_state = WM_FILLING;
                // Entry
                sw_timerStart(&timer_water, DURATION_FILLING_MS);
                valveOpen();
                #ifndef NDEBUG
                hw_uart_print("STATE: Filling");
                #endif
            }else if(ev.type == EV_START && door_fsm_get_state()!=DOOR_CLOSED){
                wm_last_error = WM_ERR_START_DOOR;
                errorHandle();
            }
            break;
        case WM_FILLING:  
            if(ev.type == EV_WATER_LEVEL_OK){
                // exit state
                // (no exit actions)
                
                // State change
                wm_state = WM_WASHING;
                // Entry
                valveClose(); 
                motorSlow();
                sw_timerStart(&timer_phase, DURATION_WASHING_MS);
                #ifndef NDEBUG
                hw_uart_print("STATE: Washing");
                #endif
            } else if(ev.type == EV_DOOR_OPEN ){
                wm_last_error = WM_ERR_DOOR_OPEN;
                errorHandle();
                #ifndef NDEBUG
                hw_uart_print("ERROR: DOOR Open");
                #endif
            } else if(ev.type == EV_WATER_TIMEOUT){
                wm_last_error = WM_ERR_WATER_TIMEOUT;
                errorHandle();
            }
            break;
        case WM_WASHING:   
            if(ev.type == EV_PHASE_TIMEOUT){
                // exit state
                // (no exit actions)

                // State change
                wm_state = WM_DRAINING_AFTER_WASH;
                // Entry
                motorStop(); 
                pumpOn();
                sw_timerStop(&timer_water);
                sw_timerStart(&timer_phase, DURATION_DRAINING_MS);
                #ifndef NDEBUG
                hw_uart_print("STATE: Draining");
                #endif
            }else if(ev.type == EV_DOOR_OPEN){
                wm_last_error = WM_ERR_DOOR_OPEN;
                errorHandle();
                #ifndef NDEBUG
                hw_uart_print("ERROR: DOOR Open");
                #endif
            }
            break;
        case WM_DRAINING_AFTER_WASH:
            if(ev.type == EV_PHASE_TIMEOUT){
                // exit state
                // (no exit actions)

                // State change
                wm_state = WM_RINSING;
                // Entry
                pumpOff();
                valveOpen();
                motorSlow();
                sw_timerStart(&timer_phase, DURATION_RINSING_MS);
                #ifndef NDEBUG
                hw_uart_print("STATE: Rinsing");
                #endif
            }else if(ev.type == EV_DOOR_OPEN){
                wm_last_error = WM_ERR_DOOR_OPEN;
                errorHandle();
            }
            break;
        case WM_RINSING:
            if(ev.type == EV_PHASE_TIMEOUT){
                // exit state
                // (no exit actions)

                // State change
                wm_state = WM_DRAINING_AFTER_RINSE;
                // Entry
                motorStop(); 
                valveClose();   
                pumpOn();
                sw_timerStart(&timer_phase, DURATION_DRAINING_MS);
                #ifndef NDEBUG
                hw_uart_print("STATE: Draining");
                #endif
            }else if(ev.type == EV_DOOR_OPEN){
                wm_last_error = WM_ERR_DOOR_OPEN;
                errorHandle();
            }
            break;
        case WM_DRAINING_AFTER_RINSE:
            if(ev.type == EV_PHASE_TIMEOUT){
                // exit state
                // (no exit actions)

                // State change
                wm_state = WM_SPINNING;
                // Entry
                pumpOff(); 
                motorFast();
                sw_timerStart(&timer_phase, DURATION_SPINNING_MS);
                #ifndef NDEBUG
                hw_uart_print("STATE: Spinning");
                #endif
            }else if(ev.type == EV_DOOR_OPEN ){
                wm_last_error = WM_ERR_DOOR_OPEN;
                errorHandle();
            }
            break;
        case WM_SPINNING:
            if(ev.type == EV_PHASE_TIMEOUT){
                // exit state
                // (no exit actions)

                // State change
                wm_state = WM_DONE;
                // Entry
                motorStop(); 
                buzzerOn(); 
                sw_timerStart(&timer_buzzer, 500);
                #ifndef NDEBUG
                hw_uart_print("STATE: Done");
                #endif
            }else if(ev.type == EV_DOOR_OPEN){
                wm_last_error = WM_ERR_DOOR_OPEN;
                errorHandle();
            }
            break;
        case WM_DONE:
            if(ev.type == EV_CANCEL || ev.type == EV_START){
                cancelHandle();   // reuse — both reset to IDLE
            }
            break;
        case WM_ERROR:
            if (ev.type == EV_CANCEL) {
                cancelHandle();
            }
            break;
        }
}
const char* wm_fsm_get_state_name(void) {
    static const char* const names[] = {
        [WM_IDLE]                = "IDLE",
        [WM_FILLING]             = "FILLING",
        [WM_WASHING]             = "WASHING",
        [WM_DRAINING_AFTER_WASH] = "DRAINING_AFTER_WASH",
        [WM_RINSING]             = "RINSING",
        [WM_DRAINING_AFTER_RINSE]= "DRAINING_AFTER_RINSE",
        [WM_SPINNING]            = "SPINNING",
        [WM_DONE]                = "DONE",
        [WM_ERROR]               = "ERROR",
    };
    return names[wm_state];
}
const char* wm_fsm_get_error_name(void) {
    static const char* const names[] = {
        [WM_ERR_NONE]          = "NO_ERROR",
        [WM_ERR_DOOR_OPEN]     = "DOOR_OPEN",
        [WM_ERR_WATER_TIMEOUT] = "WATER_TIMEOUT",
        [WM_ERR_MOTOR]         = "MOTOR_ERROR",
        [WM_ERR_START_DOOR]    = "START_WITH_DOOR_OPEN",
    };
    return names[wm_last_error];
}

void wm_fsm_do_actions(void){
    // No continuous do-actions required.
    // Motor, pump and valve are set on state entry and maintained by hardware until next transition.
}

void errorHandle(void){
    // exit state
    sw_timerStop(&timer_phase);   // stop phase timer
    sw_timerStop(&timer_water);
    // State change
    wm_state = WM_ERROR;
    // Entry
    motorStop(); 
    pumpOff(); 
    valveClose(); 
    buzzerOn();
    sw_timerStart(&timer_buzzer, 1500u);
    #ifndef NDEBUG
    hw_uart_print("STATE: Error");
    #endif
}

void cancelHandle(void){
    // exit state
    wm_last_error = WM_ERR_NONE;
    sw_timerStop(&timer_phase);  // stop phase timer
    sw_timerStop(&timer_water);
    // State change
    wm_state = WM_IDLE;
    // Entry
    motorStop(); 
    pumpOff(); 
    valveClose();
    #ifndef NDEBUG
    hw_uart_print("STATE: IDLE");
    #endif
}
