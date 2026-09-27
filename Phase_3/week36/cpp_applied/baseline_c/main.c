/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    27-9-2026
 *
 * Description:
 *   This file configures and controls GPIO pin PA5 on an STM32F446RE
 *   microcontroller, providing functions to initialize it as a digital
 *   output and toggle it high or low.
 */

#include <stdint.h>
#include "../common/stm32f446re.h"

#define RCC_AHB1ENR  (*(volatile uint32_t*)(RCC_BASE_ADDR  + RCC_AHB1ENR_OFFSET))
#define GPIOA_MODER  (*(volatile uint32_t*)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET))
#define GPIOA_ODR    (*(volatile uint32_t*)(GPIOA_BASE_ADDR + GPIOx_ODR_OFFSET))

void pa5_init_output(void) {
    RCC_AHB1ENR |= (1u << 0);
    GPIOA_MODER &= ~(0b11u << 10);
    GPIOA_MODER |=  (0b01u << 10);
}

void pa5_high(void) {
    GPIOA_ODR |= (1u << 5);
}

void pa5_low(void) {
    GPIOA_ODR &= ~(1u << 5);
}