/*
 * File:    led.h
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This header file defines the interface for controlling an LED,
 *   providing functions to initialize, turn on/off, blink, and query
 *   the state of an LED, along with PWM initialization for GPIO pin
 *   PA0.
 */



#ifndef LED_H
#define LED_H

#include <stdbool.h>

void led_init(void);
void led_on(void);
void led_off(void);
void led_blink(void);
bool led_get_state(void);
void init_gpio_pa0_pwm(void);

#endif
