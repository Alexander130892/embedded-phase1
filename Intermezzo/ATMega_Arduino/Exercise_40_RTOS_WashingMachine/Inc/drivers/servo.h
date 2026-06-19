/*
 * File:    servo.h
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This header file defines the interface for servo motor control,
 *   providing functions to initialize the servo and set its position
 *   to a specified angle between 0 and 180 degrees.
 */
#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

void    servo_init(void);
void    servo_setAngle(uint8_t angle);   /* 0 - 180 degrees */

#endif /* SERVO_H */