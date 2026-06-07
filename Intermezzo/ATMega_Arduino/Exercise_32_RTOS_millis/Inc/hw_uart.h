/*
 * File:    hw_uart.h
 * Author:  Alexander130892
 * Date:    7-6-2026
 *
 * Description:
 *   This header file declares the hardware abstraction layer interface
 *   for UART communication, providing functions to initialize the UART
 *   module and handle data transmission and reception on an embedded
 *   system.
 */
#ifndef HW_UART_H
#define HW_UART_H

#include <stdint.h>
#include "uart.h"

uart_return_state_t hw_uart_init(uint16_t ubrr);
uart_return_state_t hw_uart_transmit(uint8_t data);
uart_return_state_t hw_uart_receive_string(char* str);

#endif
