/*
 * File:    wm_fsm.h
 * Author:  Alexander130892
 * Date:    14-6-2026
 *
 * Description:
 *   This header file defines a finite state machine for a washing
 *   machine that manages the sequential states (filling, washing,
 *   draining, rinsing, spinning) with their respective durations,
 *   providing functions to initialize, process events, execute
 *   actions, and query the current state.
 */
#ifndef WM_FSM_H
#define WM_FSM_H

#include "events.h"

#define DURATION_FILLING_MS     10000
#define DURATION_WASHING_MS     3000
#define DURATION_DRAINING_MS    3000
#define DURATION_RINSING_MS     3000
#define DURATION_SPINNING_MS    3000

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
} WmState;

void wm_fsm_init(void);
void wm_fsm_handle(Event ev);
void wm_fsm_do_actions(void);
WmState wm_fsm_get_state(void);

#endif