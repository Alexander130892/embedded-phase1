/*
 * File:    servo.h
 * Author:  Alexander130892
 * Date:    15-6-2026
 *
 * Description:
 *   Servo driver interface for ATmega328P.
 *   Uses Timer1 OC1A (D9/PB1) in Fast PWM mode 14.
 *   Generates 50Hz signal with 1-2ms pulse width.
 *
 *   Angle mapping:
 *       0°   → 1ms pulse  (valve closed)
 *       90°  → 1.5ms pulse (half open)
 *       180° → 2ms pulse  (valve open)
 */

#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

void    servo_init(void);
void    servo_setAngle(uint8_t angle);   /* 0 - 180 degrees */

#endif /* SERVO_H */