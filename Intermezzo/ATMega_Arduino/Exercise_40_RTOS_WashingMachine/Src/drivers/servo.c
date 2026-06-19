/*
 * File:    servo.c
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This file controls a servo motor on an AVR microcontroller by
 *   configuring Timer1 to generate a PWM signal on pin PB1 with a 20ms
 *   period and pulse width ranging from 0.5ms (0°) to 2.5ms (180°).
 *   The servo_setAngle() function linearly maps input angles (0–180°)
 *   to the corresponding pulse widths via output compare register
 *   updates.
 */
#include <avr/io.h>
#include "servo.h"

// Timer1 @ 16MHz / prescaler 8 = 0.5µs per tick          
#define SERVO_TOP       39999u      // 20ms period          
#define SERVO_MIN   999u    /* 0.5ms = 0°   */
#define SERVO_MAX   7999u   /* 2.5ms = 180° */

void servo_init(void){
    //OC1A (PB1) already set as output in gpio_init       
    //Fast PWM mode 14: TOP = ICR1                        
    ICR1   = SERVO_TOP;
    OCR1A  = SERVO_MIN;            //start at 0°         

    TCCR1A = (1u << COM1A1) |      // non-inverting OC1A   
             (1u << WGM11);        // WGM mode 14 bits 1:0 

    TCCR1B = (1u << WGM13)  |      //WGM mode 14 bits 3:2 
             (1u << WGM12)  |
             (1u << CS11);         // prescaler 8          
}

void servo_setAngle(uint8_t angle){
    uint8_t safe_angle = (angle > 180u) ? 180u : angle;     // clamp 
    // linear interpolation between SERVO_MIN and SERVO_MAX
    uint16_t pulse = SERVO_MIN + ((uint32_t)safe_angle * (SERVO_MAX - SERVO_MIN)) / 180u;
    OCR1A = pulse;
}