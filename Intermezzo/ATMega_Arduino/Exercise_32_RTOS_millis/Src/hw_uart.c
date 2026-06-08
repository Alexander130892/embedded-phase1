/*
 * File:    hw_uart.c
 * Author:  Alexander130892
 * Date:    7-6-2026
 *
 * Description:
 *   This file implements a hardware UART driver for an AVR
 *   microcontroller with interrupt-driven reception and a circular
 *   buffer for storing incoming data. It provides functions to
 *   initialize the UART, transmit individual bytes, and receive
 *   complete strings terminated by carriage return or newline
 *   characters.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "uart.h"
#include "hw_uart.h"

static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;
static char rx_buffer[MAX_LENGTH];
static volatile uint8_t tx_head = 0;
static volatile uint8_t tx_tail = 0;
static char tx_buffer[MAX_LENGTH];
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

uart_return_state_t hw_uart_queue(uint8_t data){
	if (((tx_head +1) % MAX_LENGTH) == tx_tail){
		return BUFFER_OVERFLOW;
	}else{
		tx_buffer[tx_head]= data;
		tx_head++;
		return STATUS_OK;
	}
}
uart_return_state_t hw_uart_drain(void){
	if((UCSR0A & (1u << UDRE0))){ // UART TX available
		if(tx_head != tx_tail){	// 
			UDR0 = tx_buffer[tx_tail];
			tx_tail = (tx_tail +1) % MAX_LENGTH;
			return STATUS_OK;
		}else{
			return BUFFER_EMPTY;
		}
	}else{
		return UART_BUSY;
	}
}

uart_return_state_t hw_uart_receive_string(char* str){
	static uint8_t index = 0;
	uart_return_state_t complete = BUFFER_EMPTY;
	while(rx_head != rx_tail)
	{
		if (rx_buffer[rx_tail] == '\r' || rx_buffer[rx_tail] == '\n')
		{
			if (index > 0)
			{
				str[index]   = '\0';
				index = 0;
				complete = STATUS_OK;
			}
			rx_tail = (rx_tail + 1) % MAX_LENGTH;
		}
		else
		{
			if (index < MAX_LENGTH - 1)
			{
				str[index++] = rx_buffer[rx_tail];
			}
			rx_tail = (rx_tail + 1) % MAX_LENGTH;
		}
	}
	return complete;
}

ISR(USART_RX_vect) {
    if((rx_head + 1) % MAX_LENGTH != rx_tail) {   // buffer not full
        rx_buffer[rx_head] = UDR0;
        rx_head = (rx_head + 1) % MAX_LENGTH;
    } else {
        (void)UDR0;   // must read even if discarding — otherwise Interrupt is not cleared
        hw_uart_return_state = BUFFER_OVERFLOW;
    }
}

