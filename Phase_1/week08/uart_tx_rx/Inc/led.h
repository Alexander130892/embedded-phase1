/*
 * File:    led.h
 * Author:  Alexander130892
 * Date:    22-4-2026
 *
 * Description:
 *   This header file defines the interface for controlling an LED,
 *   providing functions to initialize the LED, turn it on/off, and
 *   check its current state. It serves as a simple LED driver API for
 *   embedded systems programming.
 */
#ifndef LED_H
#define LED_H

#include <stdbool.h>

void led_init(void);
void led_on(void);
void led_off(void);
bool led_get_state(void);

#endif
