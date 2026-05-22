/*
 * File:    uart.h
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This header file declares the interface for a UART communication
 *   module, providing functions for initializing the USART2
 *   peripheral, sending individual characters or strings, and
 *   receiving strings via an interrupt-driven circular buffer. It
 *   exposes the shared ring buffer variables (`head`, `tail`, and
 *   `buffer`) used by the ISR and receive logic for asynchronous data
 *   handling.
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
