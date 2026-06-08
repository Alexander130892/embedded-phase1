/*
 * File:    task.h
 * Author:  Alexander130892
 * Date:    7-6-2026
 *

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