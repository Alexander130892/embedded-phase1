/*
 * File:    hw_uart.h
 * Author:  Alexander130892
 * Date:    30-5-2026
 *
 * Description:
 *   This header file declares hardware-level UART initialization and
 *   transmission functions for a microcontroller, providing an
 *   interface to configure the UART baud rate and send single bytes of
 *   data.
 */

#ifndef HW_UART_H
#define HW_UART_H

#include <stdint.h>
#include "uart.h"


uart_return_state_t hw_uart_init(uint16_t ubrr);
uart_return_state_t hw_uart_transmit(uint8_t data);

#endif