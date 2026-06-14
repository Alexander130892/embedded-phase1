/*
 * File:    door_fsm.h
 * Author:  Alexander130892
 * Date:    14-6-2026
 *
 * Description:
 *   This header file defines a finite state machine for controlling a
 *   door with four states (closed, opening, open, closing) and
 *   provides functions to initialize the FSM, handle events, execute
 *   state actions, and query the current door state.
 */
#ifndef DOOR_FSM_H
#define DOOR_FSM_H

#include "events.h"

typedef enum {
    DOOR_CLOSED,   // BLUE LED
    DOOR_OPENING,  // GREEN LED
    DOOR_OPEN,     // RED LED
    DOOR_CLOSING   // GREEN LED
} DoorState;

void door_fsm_handle(Event ev);
void door_fsm_init(void);
void door_fsm_do_actions(void);
DoorState door_fsm_get_state(void);

#endif
