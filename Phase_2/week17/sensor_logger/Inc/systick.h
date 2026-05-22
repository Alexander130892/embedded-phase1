/*
 * File:    systick.h
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This header declares the interface for a SysTick timer module that
 *   initializes a 1ms periodic interrupt at 16MHz and provides a
 *   function to retrieve the current millisecond tick count. The
 *   `SYSTICK_MS_VALUE` macro defines the reload value (15999) used to
 *   achieve the 1ms interval.
 */
#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>
#include "status.h"

#define SYSTICK_MS_VALUE	15999		// 16 MHZ / 1000ms -1


void systick_init(void);
void SysTick_Handler(void);
uint32_t systick_get_ms(void);

#endif /* SYSTICK_H_ */
