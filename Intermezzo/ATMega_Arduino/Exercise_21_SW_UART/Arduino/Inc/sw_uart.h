/*
 * File:    sw_uart.h
 * Author:  Alexander130892
 * Date:    30-5-2026
 *
 * Description:
 *   This header file defines the interface for a software-based UART
 *   implementation, providing functions for initializing the UART,
 *   managing a receive buffer, and transmitting/receiving individual
 *   bytes and strings.
 */

#ifndef SW_UART_H
#define SW_UART_H

#include <stdint.h>
#include <stdbool.h>
#include "uart.h"


uart_return_state_t sw_uart_init(uint16_t ubrr); 

//Buffer Layer
bool available(void);
void clearBuffer(void);
uart_return_state_t readByte(uint8_t* data);
uart_return_state_t writeBuffer(uint8_t data);

//Transport layer
uart_return_state_t sendByte(uint8_t data);
uart_return_state_t uart_receive_string(char* str);

#endif