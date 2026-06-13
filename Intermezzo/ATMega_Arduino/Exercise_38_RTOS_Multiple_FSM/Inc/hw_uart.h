/*
 * File:    hw_uart.h
 * Author:  Alexander130892
 * Date:    13-6-2026
 *
 * Description:
 *   This header file defines the interface for hardware UART
 *   communication on an embedded system, providing functions to
 *   initialize the UART peripheral, transmit data (queuing and
 *   draining), receive strings, and print strings.
 */
#ifndef HW_UART_H
#define HW_UART_H

#include <stdint.h>
#include "uart.h"

uart_return_state_t hw_uart_init(uint16_t ubrr);

uart_return_state_t hw_uart_queue(uint8_t data);
uart_return_state_t hw_uart_drain(void);
uart_return_state_t hw_uart_receive_string(char* str);
uart_return_state_t hw_uart_print(const char* str);

#endif
