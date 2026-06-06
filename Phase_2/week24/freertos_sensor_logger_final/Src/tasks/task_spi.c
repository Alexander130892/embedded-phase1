/*
 * task_spi.c
 *
 *  Created on: Jun 6, 2026
 *      Author: alexandervindelinckx
 */

#include <stdint.h>
#include "tasks/task_spi.h"
#include "rtos/kernel_objects.h"
#include "drivers/bmp280/bmp280.h"
#include "drivers/uart.h"
#include "app/watchdog.h"

void vSPITask(void *pvParameters){
	int32_t data;
	xTimerStart(xTimerBMP280, 	 0);
	xSemaphoreTake(xMutexUart, portMAX_DELAY);
	uart_send_string("vSPI_Task running\r\n");
	xSemaphoreGive(xMutexUart);
	for(;;){
		if(ulTaskNotifyTake(pdTRUE,portMAX_DELAY)){
			if(bmp280_spi_read_temp(&data) == STATUS_OK){ // should check range or WHOAMI
				b_timeout_bmp280 = false;
				xQueueSend( xQueue_bmp280, 	(void *) &data, 0);
			}else{
				vTaskDelay(pdMS_TO_TICKS(10)); // yield before retrying// likely wrong read
			}
		}
	}
}

void vBMP280Timer (TimerHandle_t xTimerBMP280 ){
	xTaskNotifyGive(xSPITaskHandle);
}
