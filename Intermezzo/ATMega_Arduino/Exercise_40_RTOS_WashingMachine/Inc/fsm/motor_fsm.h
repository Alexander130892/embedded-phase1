/*
 * File:    motor_fsm.h
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This header file defines a finite state machine for controlling a
 *   motor with four states (stopped, slow, fast, error) and provides
 *   functions to initialize the FSM, handle events, and query the
 *   current motor state.
 */
#ifndef MOTOR_FSM_H
#define MOTOR_FSM_H

#include "events.h"

typedef enum {
    MOTOR_STOPPED,
    MOTOR_SLOW,
    MOTOR_FAST,
    MOTOR_ERROR
} motor_state_t;

void            motor_fsm_init(void);
void            motor_fsm_handle(event_t ev);
motor_state_t   motor_fsm_get_state(void);

#endif /* MOTOR_FSM_H */