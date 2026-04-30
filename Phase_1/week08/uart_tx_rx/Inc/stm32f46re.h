/*
 * File:    stm32f46re.h
 * Author:  Alexander130892
 * Date:    22-4-2026
 *
 * Description:
 *   This header file defines base memory addresses for key peripherals
 *   (GPIO Port A, USART2, and RCC) on an STM32F46RE microcontroller,
 *   providing the foundation for low-level hardware register access.
 */
#ifndef STM32F46RE_H_
#define STM32F46RE_H_
// AHB1
#define GPIOA_BASE_ADDR    0x40020000

// APB1
#define USART2_BASE_ADDR   0x40004400

// AHB1
#define RCC_BASE_ADDR      0x40023800


#endif /* STM32F46RE_H_ */
