/*
 * hooks.h
 *
 *  Created on: Jun 6, 2026
 *      Author: alexandervindelinckx
 */

#ifndef APP_HOOKS_H_
#define APP_HOOKS_H_

#include "rtos/kernel_objects.h"
void vApplicationTickHook(void);
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
void vApplicationIdleHook(void);
void vApplicationMallocFailedHook(void);

#endif /* APP_HOOKS_H_ */
