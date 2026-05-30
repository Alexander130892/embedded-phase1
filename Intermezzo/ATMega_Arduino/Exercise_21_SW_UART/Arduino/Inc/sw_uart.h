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