/*
 * hooks.c
 *
 *  Created on: Jun 6, 2026
 *      Author: alexandervindelinckx
 */

#include "app/hooks.h"
#include "drivers/uart.h"

void vApplicationTickHook(void){}
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName){
	uart_send_string("Stack Overflow detected! Task: ");
	uart_send_string(pcTaskName);
	uart_send_string("\r\n");

	taskDISABLE_INTERRUPTS();
	//Spin loop - infinite loop to be caught by debugger
	for(;;);
}
void vApplicationIdleHook(void){}
void vApplicationMallocFailedHook(void){}
