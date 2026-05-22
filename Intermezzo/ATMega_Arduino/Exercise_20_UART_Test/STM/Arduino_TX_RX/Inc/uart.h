#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>
#include "status.h"

#define UART_MAX_LENGTH 			64
#define UART_BAUDRATE_115200		(0x8B)
#define UART_BAUDRATE_9600  		0x683   // mantissa 104 | fraction 3
#define UART2_ALT_FUNC				0x7
#define UART4_ALT_FUNC				0x8


status_t uart2_init(void);
status_t uart4_init(void);
status_t uart2_gpio_init(void);
status_t uart4_gpio_init(void);
status_t uart_send_char(uint8_t c);
status_t uart_send_string(const char* str);
status_t uart2_transmit(uint8_t c);
status_t uart4_transmit(uint8_t c);
bool uart_receive_string(char *str, uint8_t maxlen);

void USART2_IRQHandler(void);
void UART4_IRQHandler(void);

#endif
