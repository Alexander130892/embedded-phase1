/*
 * File:    uart.h
 * Author:  Alexander130892
 * Date:    22-4-2026
 *
 * Description:
 *   This header file defines register offsets and function prototypes
 *   for UART/USART2 communication, providing an interface for sending
 *   and receiving individual characters and strings over serial
 *   communication.
 */
#ifndef UART_H
#define UART_H

#include <stdint.h>

//USART2 Address Space
#define USART_SR_OFFSET			0x00 // 8bit for data transfer
#define USART_DR_OFFSET			0x04 // 8bit for data transfer
#define	USART_BRR_OFFSET		0x08 // Baudrate --> 0x8B for desired BR [15:4]_Mantissa - [3:0] Fraction
#define USART_CR1_OFFSET		0x0C // Control register - bit15 Over8 / bit 12 wordlength M / bit 3 TE
#define USART_CR2_OFFSET		0x10 // Control register - bit15 Over8 / bit 12 wordlength M / bit 3 TE
#define USART_CR3_OFFSET		0x14 // Control register - bit15 Over8 / bit 12 wordlength M / bit 3 TE

void uart_send_char(uint8_t c);
void uart_send_string(const char* str);
void uart_receive_char(uint8_t *c);
void uart_receive_string(char *str, uint8_t maxlen);

#endif
