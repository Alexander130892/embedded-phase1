/*
 * File:    uart.h
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This header file defines the interface for UART (serial
 *   communication) functionality, providing functions to send and
 *   receive characters/strings along with a circular buffer
 *   implementation using head and tail pointers for interrupt-driven
 *   data handling. It includes an interrupt handler for USART2 and
 *   maintains a global buffer with a maximum length of 64 characters
 *   for buffered communication.
 */
#ifndef UART_H
#define UART_H

#include <stdint.h>

#define MAX_LENGTH 64
extern char buffer[MAX_LENGTH];
extern volatile uint8_t head;
extern volatile uint8_t tail;

void uart_send_char(uint8_t c);
void uart_send_string(const char* str);
uint8_t uart_receive_string(char *str, uint8_t maxlen);
void init_uart(void);
void USART2_IRQHandler(void);

#endif
