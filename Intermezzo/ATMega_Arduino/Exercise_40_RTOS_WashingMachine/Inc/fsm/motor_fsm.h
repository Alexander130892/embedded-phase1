/*
 * File:    motor_fsm.h
 * Author:  Alexander130892
 * Date:    17-6-2026
 *
 * Description:
 *   Motor FSM interface — table-driven implementation.
 *   Controls the washing machine motor via actuators.h.
 *
 *   States:
 *       MOTOR_STOPPED   — motor off
 *       MOTOR_SLOW      — washing speed
 *       MOTOR_FAST      — spinning speed
 *       MOTOR_ERROR     — fault detected
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