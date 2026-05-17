/*
 * systick.h
 *
 *  Created on: May 17, 2026
 *      Author: alexandervindelinckx
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
