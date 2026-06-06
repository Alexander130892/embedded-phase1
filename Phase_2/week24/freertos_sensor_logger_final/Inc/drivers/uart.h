#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>
#include "drivers/status.h"

#define UART_MAX_LENGTH 			64
#define UART_BAUDRATE_115200		(0x8B)
#define UART_ALT_FUNC				0x7


status_t uart_init(void);
status_t uart_gpio_init(void);
status_t uart_send_char(uint8_t c);
status_t uart_send_string(const char* str);
bool uart_receive_string(char *str, uint8_t maxlen);

#endif
