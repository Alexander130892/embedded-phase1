/*
 * File:    task_log.h
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This header file declares the `vLogTask` function, which is an
 *   RTOS task that handles logging functionality within an embedded
 *   system. It includes kernel object definitions required for
 *   real-time operating system task management.
 */
#ifndef TASKS_TASK_LOG_H_
#define TASKS_TASK_LOG_H_

#include "rtos/kernel_objects.h"

void vLogTask(void *pvParameters);

#endif /* TASKS_TASK_LOG_H_ */
