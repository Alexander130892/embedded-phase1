/*
 * task_spi.h
 *
 *  Created on: Jun 6, 2026
 *      Author: alexandervindelinckx
 */

#ifndef TASKS_TASK_SPI_H_
#define TASKS_TASK_SPI_H_

#include "rtos/kernel_objects.h"

#define BMP280_PERIOD		1000

void vSPITask(void *pvParameters);
void vBMP280Timer (TimerHandle_t xTimerBMP280 );

#endif /* TASKS_TASK_SPI_H_ */
