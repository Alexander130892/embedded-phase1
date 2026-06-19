/*
 * File:    actuators.c
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This file implements control functions for physical actuators
 *   (motor, pump, and servo-controlled valve) on an AVR
 *   microcontroller, providing initialization and on/off control for
 *   each device with direct GPIO manipulation and servo positioning.
 */
#include <avr/io.h>
#include "actuators.h"
#include "gpio.h"
#include "servo.h"

void actuators_init(void){
    motorStop();
    pumpOff();
    valveClose();
}
void motorStart(void){
    MOTOR_PORT |= (1u << MOTOR_PIN);
}
void motorStop(void){
    MOTOR_PORT &= ~(1u << MOTOR_PIN);
}
void motorSlow(void) {
    motorStart();   // TODO: PWM speed control if timer available
}
void motorFast(void) {
    motorStart();   // TODO: PWM speed control if timer available
}
void pumpOn(void){
    PUMP_PORT &= ~(1u << PUMP_PIN);     // active LOW 
}
void pumpOff(void){
    PUMP_PORT |= (1u << PUMP_PIN);      // active LOW
}
void valveOpen(void){
    servo_setAngle(90u);
}
void valveClose(void){
    servo_setAngle(0u);
}