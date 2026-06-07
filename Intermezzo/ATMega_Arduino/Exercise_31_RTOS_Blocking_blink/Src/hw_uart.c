/*
 * File:    hw_uart.c
 * Author:  Alexander130892
 * Date:    7-6-2026
 *
 * Description:
 *   This file implements a hardware UART driver for an AVR
 *   microcontroller with interrupt-driven receive buffering and
 *   polling-based transmit. It provides initialization, character
 *   transmission, and string reception with circular buffer management
 *   and overflow detection.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "uart.h"
#include "hw_uart.h"

static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;
static char buffer[MAX_LENGTH];
volatile uart_return_state_t    hw_uart_return_state = STATUS_OK;

// ----- HW UART -----
uart_return_state_t hw_uart_init(uint16_t ubrr){ // HW UART
    //Set baud rate
    UBRR0H  = (uint8_t)(ubrr>>8);
    UBRR0L  = (uint8_t)ubrr;
    // Enable USART Receive Complete Interrupt
    UCSR0B  |= (1u << RXCIE0);
    // Set frame format: 8data, 1stop bit 
    UCSR0C  |= (0 << USBS0)|(3u << UCSZ00);
    // Enable transmitter 
    UCSR0B  |= (1u << TXEN0) |(1u << RXEN0);
    return STATUS_OK;
}
uart_return_state_t hw_uart_transmit(uint8_t data){
    while(!(UCSR0A & (1u << UDRE0))); //Wait until UART is not busy
    UDR0 = data;
    return STATUS_OK;
}

uart_return_state_t hw_uart_receive_string(char* str){
	static uint8_t index = 0;
	uart_return_state_t complete = BUFFER_EMPTY;
	while(head != tail)
	{
		if (buffer[tail] == '\r' || buffer[tail] == '\n')
		{
			if (index > 0)
			{
				str[index]   = '\0';
				index = 0;
				complete = STATUS_OK;
			}
			tail = (tail + 1) % MAX_LENGTH;
		}
		else
		{
			if (index < MAX_LENGTH - 1)
			{
				str[index++] = buffer[tail];
			}
			tail = (tail + 1) % MAX_LENGTH;
		}
	}
	return complete;
}

ISR(USART_RX_vect) {
    if((head + 1) % MAX_LENGTH != tail) {   // buffer not full
        buffer[head] = UDR0;
        head = (head + 1) % MAX_LENGTH;
    } else {
        (void)UDR0;   // must read even if discarding — otherwise Interrupt is not cleared
        hw_uart_return_state = BUFFER_OVERFLOW;
    }
}

