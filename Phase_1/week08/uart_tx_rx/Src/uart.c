/*
 * File:    uart.c
 * Author:  Alexander130892
 * Date:    22-4-2026
 *
 * Description:
 *   This file implements basic UART communication functions for an
 *   STM32F4 microcontroller, providing low-level serial transmission
 *   and reception of individual characters and strings through USART2.
 *   The implementation uses direct register manipulation to poll
 *   status flags and transfer data via the UART data register.
 */
#include "uart.h"
#include	"stm32f46re.h"

void uart_send_char(uint8_t c){
	while(!(*((volatile uint32_t *)(USART2_BASE_ADDR + USART_SR_OFFSET)) & (0x1 << 7))){}; // Wait until TXE is 1 (Tx data register empty)
	*((volatile uint32_t *)(USART2_BASE_ADDR + USART_DR_OFFSET)) = c;

	return;
}

void uart_send_string(const char* str){
	while((*str)!='\0'){
		uart_send_char(*str);
		str++;
	}
}
void uart_receive_char(uint8_t* c){
	while(!(*((volatile uint32_t *)(USART2_BASE_ADDR + USART_SR_OFFSET)) & (0x1 << 5))){}; // Wait until RXNE is 1 (Rx data is full)
	*c = *((volatile uint8_t *)(USART2_BASE_ADDR + USART_DR_OFFSET));
}
void uart_receive_string(char* str, uint8_t maxlen){
	if (maxlen == 0) return;
	uint8_t index;
	for (index = 0; index < maxlen -1 ; index++){
		uint8_t c;
		uart_receive_char(&c);
		if(c =='\n' || c =='\r'){
			break;
		}
		str[index]=c;
	}
	str[index]='\0';
}
