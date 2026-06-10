/*
 * File:    gpio.h
 * Author:  Alexander130892
 * Date:    8-6-2026
 *
 * Description:
 *   This header file defines GPIO pin mappings for an AVR
 *   microcontroller, configuring pins for an RGB LED, a piezo buzzer
 *   on port B, and a pushbutton on port D, along with a function to
 *   initialize these GPIO pins.
 */
#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define LED_DDR   DDRB
#define LED_PORT  PORTB
#define LED_INTERNAL_PIN   PB5
#define LED_R_PIN   PB2
#define LED_G_PIN   PB3
#define LED_B_PIN   PB4
#define PIEZZO_PIN  PB1 

#define PB_DDR   DDRD
#define PB_PORT  PORTD
#define PB_PIN   PD2 

void gpio_init(void);

#endif