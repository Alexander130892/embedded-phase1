#ifndef DOOR_FSM_H
#define DOOR_FSM_H

/*
 * Note: door_fsm was originally a full state machine with OPENING/CLOSING
 * states. After refactoring, door state detection moved to taskDoor.
 * This module now only tracks OPEN/CLOSED state via door_fsm_update().
 */

#include "events.h"

typedef enum {
    DOOR_CLOSED,   
    DOOR_OPEN,     
} door_state_t;

void door_fsm_init(void);
//void door_fsm_do_actions(void);
door_state_t door_fsm_get_state(void);
void door_fsm_update(door_state_t new_state);

#endif
