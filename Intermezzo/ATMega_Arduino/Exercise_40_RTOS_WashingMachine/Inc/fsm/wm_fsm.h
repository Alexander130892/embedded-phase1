/*
 * File:    wm_fsm.h
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This header file defines a finite state machine (FSM) for a
 *   washing machine, specifying its operational states (filling,
 *   washing, rinsing, spinning, etc.), error conditions, and the
 *   interface functions to initialize, handle events, and query the
 *   machine's state.
 */

#ifndef WM_FSM_H
#define WM_FSM_H

#include "events.h"

#define DURATION_FILLING_MS     10000u
#define DURATION_WASHING_MS     5000u
#define DURATION_DRAINING_MS    5000u
#define DURATION_RINSING_MS     5000u
#define DURATION_SPINNING_MS    5000u

typedef enum {
    WM_IDLE,
    WM_FILLING,
    WM_WASHING,
    WM_DRAINING_AFTER_WASH,
    WM_RINSING,
    WM_DRAINING_AFTER_RINSE,
    WM_SPINNING,
    WM_DONE,
    WM_ERROR
} wm_state_t;

typedef enum {
    WM_ERR_NONE,
    WM_ERR_DOOR_OPEN,
    WM_ERR_WATER_TIMEOUT,
    WM_ERR_MOTOR,
    WM_ERR_START_DOOR,
} wm_error_t;

extern wm_error_t wm_last_error;

void wm_fsm_init(void);
void wm_fsm_handle(event_t ev);
void wm_fsm_do_actions(void);
wm_state_t wm_fsm_get_state(void);
const char* wm_fsm_get_state_name(void);
const char* wm_fsm_get_error_name(void);

#endif