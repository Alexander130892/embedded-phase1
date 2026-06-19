/*
 * File:    actuators.h
 * Author:  Alexander130892
 * Date:    15-6-2026
 *
 * Description:
 *   Actuator driver interface for the washing machine.
 *   Provides control functions for motor, pump and water valve.
 *   All hardware details are encapsulated here — FSMs and tasks
 *   must not access actuator pins directly.
 *
 *   Motor  : L298N ENA on PD5 — digital on/off
 *   Pump   : relay module on PB0 — active LOW
 *   Valve  : servo on PB1 — via servo.c (Timer1 OC1A)
 */

#ifndef ACTUATORS_H
#define ACTUATORS_H

void    actuators_init(void);

void    motorStart(void);
void    motorStop(void);
void    motorSlow(void); // placeholder only - TODO - add pwm (HW constraint at the moment)
void    motorFast(void); // placeholder only - TODO - add pwm (HW constraint at the moment)


void    pumpOn(void);
void    pumpOff(void);

void    valveOpen(void);
void    valveClose(void);

#endif /* ACTUATORS_H */