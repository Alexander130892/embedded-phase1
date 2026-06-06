/*
 * File:    task_spi.h
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This header file defines the interface for an SPI task that
 *   manages communication with a BMP280 sensor, including a periodic
 *   timer callback that executes every 1000 milliseconds to read
 *   sensor data.
 */
#ifndef TASKS_TASK_SPI_H_
#define TASKS_TASK_SPI_H_

#include "rtos/kernel_objects.h"

#define BMP280_PERIOD		1000

void vSPITask(void *pvParameters);
void vBMP280Timer (TimerHandle_t xTimerBMP280 );

#endif /* TASKS_TASK_SPI_H_ */
