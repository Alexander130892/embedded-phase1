/*
 * File:    actuators.h
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This header file declares the interface for controlling physical
 *   actuators including a motor, pump, and valve, providing
 *   initialization and basic on/off control functions for each device.
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