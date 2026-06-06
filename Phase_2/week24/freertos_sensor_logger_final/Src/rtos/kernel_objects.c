/*
 * File:    kernel_objects.c
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This file declares and initializes FreeRTOS kernel objects
 *   (queues, semaphores, timers, and task handles) used for inter-task
 *   communication and synchronization in an embedded system that reads
 *   sensor data from a BMP280 barometer and MPU6050 accelerometer via
 *   SPI/I2C, processes UART commands, and logs CSV data.
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
