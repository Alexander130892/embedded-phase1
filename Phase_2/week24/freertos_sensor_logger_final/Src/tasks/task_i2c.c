/*
 * File:    task_i2c.c
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This file implements an I2C task that reads sensor data from an
 *   MPU6050 accelerometer/gyroscope device and queues the data for
 *   other tasks to consume. The task waits for notifications to
 *   trigger reads, handles communication timeouts, and synchronizes
 *   UART output using a mutex.
 */
#include <stdint.h>
#include "tasks/task_i2c.h"
#include "rtos/kernel_objects.h"
#include "drivers/timer.h"
#include "drivers/mpu6050/mpu6050.h"
#include "drivers/uart.h"
#include "app/watchdog.h"

void vI2CTask(void *pvParameters){
	uint8_t dev_addr = (uint8_t)(uintptr_t)pvParameters;
	mpu6050_data_t data;
	timer2_enable();
	xSemaphoreTake(xMutexUart, portMAX_DELAY);
	uart_send_string("vI2C_Task running\r\n");
	xSemaphoreGive(xMutexUart);
	for(;;){
		if(ulTaskNotifyTake(pdTRUE,portMAX_DELAY)){
			if(mpu6050_read_data(dev_addr, &data)==STATUS_OK){
				b_timeout_mpu6050 = false;
				xQueueSend( xQueue_mpu6050, 	(void *) &data, 0);
			}else{
				vTaskDelay(pdMS_TO_TICKS(10)); // yield before retrying// likely wrong read
			}
		}
	}
}

