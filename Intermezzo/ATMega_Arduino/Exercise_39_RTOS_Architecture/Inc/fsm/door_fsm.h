#ifndef DOOR_FSM_H
#define DOOR_FSM_H

#include "events.h"
#include "timer.h"

typedef enum {
    DOOR_CLOSED,   // BLUE LED
    DOOR_OPENING,  // GREEN LED
    DOOR_OPEN,     // RED LED
    DOOR_CLOSING   // GREEN LED
} DoorState;

extern SoftTimer timer_autoClose;

void door_fsm_handle(Event ev);
void door_fsm_init(void);
void door_fsm_do_actions(void);
DoorState door_fsm_get_state(void);

#endif
