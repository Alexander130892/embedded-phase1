/*
 * File:    uart.h
 * Author:  Alexander130892
 * Date:    10-5-2026
 *
 * Description:
 *   This header file defines the interface for UART (serial
 *   communication) functionality, including functions to send and
 *   receive characters/strings and a circular buffer implementation
 *   with head/tail pointers for buffering received data. It also
 *   declares an interrupt handler for USART2, indicating it uses
 *   interrupt-driven UART communication.
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
