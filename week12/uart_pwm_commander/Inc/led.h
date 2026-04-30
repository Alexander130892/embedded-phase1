

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
