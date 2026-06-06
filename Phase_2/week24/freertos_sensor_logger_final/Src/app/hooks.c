/*
 * File:    hooks.c
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This file implements FreeRTOS application hooks, with a stack
 *   overflow handler that logs the offending task name via UART and
 *   halts execution for debugger inspection, while other hooks are
 *   left empty.
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
