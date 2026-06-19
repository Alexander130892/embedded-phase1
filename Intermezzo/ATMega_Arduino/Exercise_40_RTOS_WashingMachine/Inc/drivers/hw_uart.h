/*
 * File:    hw_uart.h
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This header file declares the hardware-level UART interface
 *   functions for initializing the serial port, transmitting data
 *   (queuing, draining, and printing strings), and receiving input on
 *   an embedded system. The functions wrap low-level UART operations
 *   and return status codes indicating success or failure of serial
 *   communication operations.
 */
#ifndef HW_UART_H
#define HW_UART_H

#include <stdint.h>
#include "uart.h"

uart_return_state_t hw_uart_init(uint16_t ubrr);
uart_return_state_t hw_uart_queue(uint8_t data);
uart_return_state_t hw_uart_drain(void);    // no longer used
uart_return_state_t hw_uart_receive_string(char* str);
uart_return_state_t hw_uart_print(const char* str);
void hw_uart_print_uint32(uint32_t value);

#endif
