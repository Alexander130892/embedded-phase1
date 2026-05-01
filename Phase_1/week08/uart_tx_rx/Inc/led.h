/*
 * File:    led.h
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This header file defines a simple LED control interface with
 *   functions to initialize, turn on/off, and check the state of an
 *   LED. It provides a basic abstraction layer for LED hardware
 *   control in embedded systems.
 */
#ifndef LED_H
#define LED_H

#include <stdbool.h>

void led_init(void);
void led_on(void);
void led_off(void);
bool led_get_state(void);

#endif
