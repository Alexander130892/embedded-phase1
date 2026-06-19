/*
 * File:    actuators.c
 * Author:  Alexander130892
 * Date:    10-6-2026
 *
 * Description:
 *   Actuator driver for the washing machine.
 *   Motor, pump and water valve control.
 *   No process logic — pure hardware abstraction.
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