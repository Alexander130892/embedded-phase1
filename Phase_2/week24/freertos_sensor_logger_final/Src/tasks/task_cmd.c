/*
 * task_cmd.c
 *
 *  Created on: Jun 6, 2026
 *      Author: alexandervindelinckx
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tasks/task_cmd.h"
#include "rtos/kernel_objects.h"
#include "drivers/uart.h"
#include "drivers/timer.h"
#include "drivers/bmp280/bmp280.h"
#include "drivers/mpu6050/mpu6050.h"
#include "app/watchdog.h"

void vCmdTask(void *pvParameters ){
	char item;
	static char command[UART_MAX_LENGTH];
	char* arg;
	char* cmd;
	uint8_t index = 0;
	xSemaphoreTake(xMutexUart, portMAX_DELAY);
	uart_send_string("vCmd_Task running\r\n");
	xSemaphoreGive(xMutexUart);
	for(;;){
		if(xQueueReceive(	xQueueUARTRx, 	(void *) &item, 100) == pdTRUE){
			if(item == '\r'){
				// command finished
				command[index]='\0';
				xSemaphoreTake(xMutexUart, portMAX_DELAY);
				uart_send_string(command);
				uart_send_string("\r\n");
				xSemaphoreGive(xMutexUart);

				cmd = strtok(command, " ");
				if(strcmp(cmd, "log") == 0){
					arg = strtok(NULL, " "); // argument
					if(arg==NULL){
						;// do nothing
					}else if(strcmp(arg, "on") == 0){
						// Clear both Queues
						xQueueReset(xQueue_bmp280);
						xQueueReset(xQueue_mpu6050);
						//Start logging (both sensors)
						xTimerStart(xTimerBMP280, 0);
						xTimerStart(xTimerHeartBeat, 0);
						b_timeout_bmp280 = false;
						b_timeout_mpu6050 = false;
						timer2_enable();
					}else if(strcmp(arg, "off") == 0){
						//Stop logging (both sensors)
						xTimerStop(xTimerBMP280, 0);
						xTimerStop(xTimerHeartBeat, 0);
						timer2_disable();
					}else{
						xSemaphoreTake(xMutexUart, portMAX_DELAY);
						uart_send_string("Invalid argument!\r\n");
						xSemaphoreGive(xMutexUart);
					}
				}else if (strcmp(cmd, "hwm")==0){
					UBaseType_t uxCount = uxTaskGetNumberOfTasks();
					uxTaskGetSystemState(pxTaskStatusArray, uxCount, NULL);
					char hwm_str[UART_MAX_LENGTH];
					xSemaphoreTake(xMutexUart, portMAX_DELAY);
					for(uint8_t task_i=0; task_i < uxCount; task_i++){
						uart_send_string("Task: ");
						uart_send_string(pxTaskStatusArray[task_i].pcTaskName);
						uart_send_string(" - HWM Value: ");
						snprintf(hwm_str, sizeof(hwm_str), "%lu\r\n", pxTaskStatusArray[task_i].usStackHighWaterMark);
						uart_send_string(hwm_str);
					}
					xSemaphoreGive(xMutexUart);
				}else if (strcmp(cmd, "mem")==0){
					static char heap_str[UART_MAX_LENGTH];
					xSemaphoreTake(xMutexUart, portMAX_DELAY);
					snprintf(heap_str, sizeof(heap_str), "Current free Heap available: %u\r\n", xPortGetFreeHeapSize());
					uart_send_string(heap_str);
					snprintf(heap_str, sizeof(heap_str), "Minimal free Heap Ever available: %u\r\n", xPortGetMinimumEverFreeHeapSize());
					uart_send_string(heap_str);
					xSemaphoreGive(xMutexUart);
				}
				//	reset for next command
				index = 0;
			}else
			{
				command[index] = item;
				index++;
				index = index % UART_MAX_LENGTH; // protect against overflow
			}
		}
	}
}
