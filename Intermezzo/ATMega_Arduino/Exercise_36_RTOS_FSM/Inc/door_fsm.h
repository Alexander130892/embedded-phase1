#ifndef DOOR_FSM_H
#define DOOR_FSM_H

#include "events.h"

typedef enum {
    STATE_CLOSED,   // BLUE LED
    STATE_OPENING,  // GREEN LED
    STATE_OPEN,     // RED LED
    STATE_CLOSING   // GREEN LED
} DoorState;

void door_fsm_handle(Event ev);
void door_fsm_init(void);
DoorState door_fsm_get_state(void);

#endif
