/*
 * File:    task_i2c.h
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This header file declares the I2C task function for an RTOS-based
 *   embedded system, providing the entry point for a task that handles
 *   I2C communication operations.
 */
#ifndef TASKS_TASK_I2C_H_
#define TASKS_TASK_I2C_H_

#include "rtos/kernel_objects.h"

void vI2CTask(void *pvParameters);


#endif /* TASKS_TASK_I2C_H_ */
