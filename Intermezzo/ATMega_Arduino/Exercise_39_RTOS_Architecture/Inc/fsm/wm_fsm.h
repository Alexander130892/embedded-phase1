#ifndef WM_FSM_H
#define WM_FSM_H

#include "events.h"
#include "timer.h"

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

extern SoftTimer timer_phase;

void wm_fsm_init(void);
void wm_fsm_handle(Event ev);
void wm_fsm_do_actions(void);
WmState wm_fsm_get_state(void);

#endif