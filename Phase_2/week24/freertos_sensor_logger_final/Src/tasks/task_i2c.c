/*
 * task_i2c.c
 *
 *  Created on: Jun 6, 2026
 *      Author: alexandervindelinckx
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

