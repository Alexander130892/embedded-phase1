/*
 * File:    kernel_objects.h
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This header file declares global FreeRTOS kernel objects (queues,
 *   semaphores, timers, and tasks) used for managing inter-task
 *   communication in an embedded system that interfaces with BMP280
 *   and MPU6050 sensors via SPI/I2C protocols and UART communication.
 */
#ifndef RTOS_KERNEL_OBJECTS_H_
#define RTOS_KERNEL_OBJECTS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#define N_TASKS 			10	//Oversized

extern QueueHandle_t xQueue_bmp280;
extern QueueHandle_t xQueue_mpu6050;
extern QueueHandle_t xQueueUARTRx;

extern SemaphoreHandle_t xMutexUart;

extern TimerHandle_t xTimerBMP280;
extern TimerHandle_t xTimerHeartBeat;

extern TaskHandle_t xSPITaskHandle;		// BMP280 @ 1 Hz (SW Timer)
extern TaskHandle_t xI2CTaskHandle;		// MPU6050 @ 100 Hz (HW TIM2)
extern TaskHandle_t xCmdTaskHandle;		// UART Command Parsing
extern TaskHandle_t xLogTaskHandle;		// CSV over UART + Watchdog kick (SW Timer)

extern TaskStatus_t pxTaskStatusArray[N_TASKS];

#endif /* RTOS_KERNEL_OBJECTS_H_ */
