/*
 * File:    uart.h
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This header file declares the interface for a UART communication
 *   module, providing functions to initialize the peripheral, send
 *   individual characters or strings, and receive strings via
 *   interrupt-driven buffering. It defines a circular buffer with
 *   `head` and `tail` indices for managing incoming data through the
 *   USART2 interrupt handler.
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
