/*
 * File:    led.h
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This header file defines an interface for controlling an LED with
 *   functions to initialize, turn on/off, and check the current state
 *   of the LED. It provides a simple abstraction layer for basic LED
 *   operations in an embedded system.
 */
#ifndef LED_H
#define LED_H

#include <stdbool.h>

void led_init(void);
void led_on(void);
void led_off(void);
bool led_get_state(void);

#endif
