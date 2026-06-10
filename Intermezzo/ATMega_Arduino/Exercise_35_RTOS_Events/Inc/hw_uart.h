/*
 * File:    hw_uart.h
 * Author:  Alexander130892
 * Date:    10-6-2026
 *
 * Description:
 *   This header file declares the hardware UART interface functions
 *   for initializing UART communication, queueing data for
 *   transmission, draining the transmit buffer, and receiving strings
 *   from the UART device.
 */
#ifndef HW_UART_H
#define HW_UART_H

#include <stdint.h>
#include "uart.h"

uart_return_state_t hw_uart_init(uint16_t ubrr);

uart_return_state_t hw_uart_queue(uint8_t data);
uart_return_state_t hw_uart_drain(void);
uart_return_state_t hw_uart_receive_string(char* str);

#endif
