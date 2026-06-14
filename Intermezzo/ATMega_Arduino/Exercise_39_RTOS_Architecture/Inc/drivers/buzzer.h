#ifndef BUZZER_H
#define BUZZER_H

#include "timer.h"
#include "gpio.h"
#include <avr/io.h>



extern SoftTimer timer_buzzer_off;
extern SoftTimer timer_buzzer;

void buzzerOn(void);
void buzzerOff(void);
void buzzer_init(void);


#endif