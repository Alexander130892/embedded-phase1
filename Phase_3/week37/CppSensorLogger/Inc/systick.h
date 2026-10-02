/*
 * File:    systick.h
 * Author:  Alexander130892
 * Date:    2-10-2026
 *
 * Description:
 *   This header file defines a SysTick timer interface for a 16 MHz
 *   system, providing initialization, interrupt handling, and
 *   millisecond counter functionality. The SysTick is configured to
 *   generate an interrupt every 1 millisecond to maintain a running
 *   millisecond timestamp.
 */
#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>

#define SYSTICK_MS_VALUE	15999		// 16 MHZ / 1000ms -1


void systick_init(void);
void SysTick_Handler(void);
uint32_t systick_get_ms(void);

#endif /* SYSTICK_H_ */
