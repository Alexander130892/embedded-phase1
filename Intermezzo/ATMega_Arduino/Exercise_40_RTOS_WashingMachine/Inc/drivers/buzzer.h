/*
 * File:    buzzer.h
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This header file defines the interface for a buzzer peripheral,
 *   providing functions to initialize the buzzer and control its
 *   on/off states. It depends on timer and GPIO modules for its
 *   implementation.
 */

#ifndef BUZZER_H
#define BUZZER_H

#include "timer.h"
#include "gpio.h"



void buzzerOn(void);
void buzzerOff(void);
void buzzer_init(void);


#endif