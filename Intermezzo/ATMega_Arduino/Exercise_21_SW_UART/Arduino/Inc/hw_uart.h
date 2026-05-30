#ifndef HW_UART_H
#define HW_UART_H

#include <stdint.h>
#include "uart.h"


uart_return_state_t hw_uart_init(uint16_t ubrr);
uart_return_state_t hw_uart_transmit(uint8_t data);

#endif