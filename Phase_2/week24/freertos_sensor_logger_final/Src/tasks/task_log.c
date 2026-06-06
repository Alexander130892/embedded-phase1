/*
 * task_log.c
 *
 *  Created on: Jun 6, 2026
 *      Author: alexandervindelinckx
 */

#include <stdint.h>
#include <stdio.h>
#include "tasks/task_log.h"
#include "rtos/kernel_objects.h"
#include "drivers/uart.h"
#include "drivers/bmp280/bmp280.h"
#include "drivers/mpu6050/mpu6050.h"
#include "app/watchdog.h"


void vLogTask(void *pvParameters){
	mpu6050_data_t mpu6050_read;
	int32_t	bmp280_read;
	char output_str[UART_MAX_LENGTH];
	xTimerStart(xTimerHeartBeat, 0);
	xSemaphoreTake(xMutexUart, portMAX_DELAY);
	uart_send_string("vLog_Task running\r\n");
	xSemaphoreGive(xMutexUart);
	for(;;){
		if(ulTaskNotifyTake(pdTRUE,0)){
			if(b_timeout_mpu6050){
				xSemaphoreTake(xMutexUart, portMAX_DELAY);
				uart_send_string("ERROR: MPU6050 Not responding!\r\n");
				xSemaphoreGive(xMutexUart);
			}else if (b_timeout_bmp280){
				xSemaphoreTake(xMutexUart, portMAX_DELAY);
				uart_send_string("ERROR: BMP280 Not responding!\r\n");
				xSemaphoreGive(xMutexUart);
			}
		}
		if(xQueueReceive(	xQueue_mpu6050, 	(void *) &mpu6050_read, pdMS_TO_TICKS(50)) == pdTRUE){
			snprintf(output_str, sizeof(output_str),
							"%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",
							(float)mpu6050_read.accel_x/16384,	(float)mpu6050_read.accel_y/16384,	(float)mpu6050_read.accel_z/16384, 		//2^15 / 2g (sensitivity)
							(float)mpu6050_read.gyro_x/131,		(float)mpu6050_read.gyro_y/131,		(float)mpu6050_read.gyro_z/131			//2^15 / 250g (sensitivity)
							);
			xSemaphoreTake(xMutexUart, portMAX_DELAY);
			uart_send_string(output_str);
			xSemaphoreGive(xMutexUart);
		}
		if(xQueueReceive(	xQueue_bmp280, 	(void *) &bmp280_read, 0) == pdTRUE){
			snprintf(output_str, sizeof(output_str), "%ld.%02ld\r\n",bmp280_read/100, bmp280_read%100);
			xSemaphoreTake(xMutexUart, portMAX_DELAY);
			uart_send_string(output_str);
			xSemaphoreGive(xMutexUart);
		}
	}
}
