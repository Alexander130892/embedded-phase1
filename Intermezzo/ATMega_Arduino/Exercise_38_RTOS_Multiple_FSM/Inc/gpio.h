/*
 * File:    gpio.h
 * Author:  Alexander130892
 * Date:    14-6-2026
 *
 * Description:
 *   This header file defines GPIO pin mappings and macros for
 *   controlling an LED (with internal and RGB components), a piezo
 *   buzzer, and a push button on an AVR microcontroller, along with an
 *   initialization function declaration.
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

#define PIEZZO_DDR   DDRD
#define PIEZZO_PORT  PORTD 
#define PIEZZO_PIN  PD5 

#define PB_DDR   DDRD
#define PB_PORT  PORTD
#define PB_PIN   PD2 

void gpio_init(void);

#endif