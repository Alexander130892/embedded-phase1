/*
 * kernel_objects.c
 *
 *  Created on: Jun 6, 2026
 *      Author: alexandervindelinckx
 */

#include "kernel_objects.h"

QueueHandle_t xQueue_bmp280;
QueueHandle_t xQueue_mpu6050;
QueueHandle_t xQueueUARTRx;

SemaphoreHandle_t xMutexUart;

TimerHandle_t xTimerBMP280;
TimerHandle_t xTimerHeartBeat;

TaskHandle_t xSPITaskHandle;		// BMP280 @ 1 Hz (SW Timer)
TaskHandle_t xI2CTaskHandle;		// MPU6050 @ 100 Hz (HW TIM2)
TaskHandle_t xCmdTaskHandle;		// UART Command Parsing
TaskHandle_t xLogTaskHandle;		// CSV over UART + Watchdog kick (SW Timer)

TaskStatus_t pxTaskStatusArray[N_TASKS];
