/*
 * File:    uart.h
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This header defines the interface for a UART2 peripheral driver,
 *   providing initialization, character/string transmission, and
 *   interrupt-driven string reception functions, along with constants
 *   for baud rate configuration and a maximum buffer length of 64
 *   bytes.
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>
#include "status.h"

#define UART_MAX_LENGTH 			64
#define UART_BAUDRATE_115200		(0x8B)
#define UART_ALT_FUNC				0x7


status_t uart_init(void);
status_t uart_gpio_init(void);
status_t uart_send_char(uint8_t c);
status_t uart_send_string(const char* str);
bool uart_receive_string(char *str, uint8_t maxlen);

void USART2_IRQHandler(void);

#endif
