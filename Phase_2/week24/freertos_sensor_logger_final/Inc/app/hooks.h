/*
 * File:    hooks.h
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This header file declares FreeRTOS hook functions that are called
 *   at specific points during kernel execution, such as on each tick,
 *   during stack overflow, idle time, and memory allocation failures.
 *   It provides integration points for application-specific handling
 *   of these kernel events.
 */
#ifndef APP_HOOKS_H_
#define APP_HOOKS_H_

#include "rtos/kernel_objects.h"
void vApplicationTickHook(void);
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
void vApplicationIdleHook(void);
void vApplicationMallocFailedHook(void);

#endif /* APP_HOOKS_H_ */
