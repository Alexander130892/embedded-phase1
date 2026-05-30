/*
 * File:    sw_uart.c
 * Author:  Alexander130892
 * Date:    30-5-2026
 *
 * Description:
 *   This file implements a software-based UART (serial communication)
 *   interface for an AVR microcontroller using GPIO pins and timer
 *   interrupts to bit-bang serial data transmission and reception. It
 *   manages serial communication through a circular buffer,
 *   timer-based sampling/transmission, and external interrupts to
 *   detect incoming data.
 */

#include "sw_uart.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t tx_byte = 0;
volatile uint8_t head = 0;
volatile uint8_t tail = 0;
volatile sw_uart_state_t        sw_uart_state = IDLE;
volatile uart_return_state_t    sw_uart_return_state = STATUS_OK;
volatile uint8_t rx_ready = 0;
char buffer[MAX_LENGTH];

// ----- SW  UART -----
uart_return_state_t sw_uart_init(uint16_t ubrr){ // SW UART
    // Set GPIO
    DDRD    |= (1u << PD7);     //  Output, Tx
    DDRD    &= ~(1u << PD6);    //  Input, Rx
    PORTD   |= (1u << PD7);     //  Internal Pull up activated
    PCMSK2 |= (1u << PCINT22);   // PD6 = PCINT22
    PCICR  |= (1u << PCIE2);     // enable PCINT2 group
    // Init Timers
    TCCR1B  &= ~(0x7 << CS12);  // Reset timer
    TCCR1B  |= (1u << WGM12);   // CTC mode, TOP = OCR1A
    OCR1A   = ubrr;             // 104us at 16MHz
    TIMSK1  |= (1u << OCIE1A);  // output compare A match interrupt
    //Enable timer before UART transmission starts in sendByte
    return STATUS_OK;
}

uart_return_state_t sendByte(uint8_t data){
    while(TCCR1B & (1 << CS10));   // wait if timer still running --> Tx in progress
    tx_byte = data;
    TCCR1B |= (1u << CS10);        // start timer --> kicks off timer ISR
    sw_uart_state = TRANSMITTING;
    return STATUS_OK;
}
uart_return_state_t readByte(uint8_t* data){
    if(head != tail){
        *data = buffer[tail];
        tail = (tail + 1) % MAX_LENGTH;
        return STATUS_OK;
    }else{
        return BUFFER_EMPTY;
    }
}

uart_return_state_t uart_receive_string(char* str){
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
bool available(void) {
    return head != tail;
}
void clearBuffer(void) {
    cli();              // disable interrupts so ISR doesnt write whilst main is clearing buffer
    head = tail;
    buffer[head] = 0;
    sei();              // re-enable interrupts
}

// Interrupt Handlers
ISR(TIMER1_COMPA_vect){
    static uint8_t bit_counter = 0;
    uint8_t bit;
    switch(sw_uart_state)
    {
        case START_SAMPLE:
            if(PIND & (1u << PD6)){    // Glitch
                sw_uart_state = IDLE;   // Reset
                PCICR |= (1u << PCIE2); // Re-activate external interrupts
            }else if((head + 1) % MAX_LENGTH == tail){
                sw_uart_return_state = BUFFER_OVERFLOW;
                sw_uart_state = IDLE; 
            }else{
                OCR1A  = ARR;           // Reset period to full --> Sample in middle of bit
                TCNT1  = 0;             // reset counter
                bit_counter++;
                sw_uart_state = RECEIVING;   // Rx confirmed
                buffer[head] = 0;
                sw_uart_return_state = STATUS_OK;
            }
            break;
        case RECEIVING:
            switch (bit_counter)
            {
            case 9:    // Reset, stop period
                if(!((PIND >> PD6) & 1u)){
                    sw_uart_return_state = FRAMING_ERROR;
                }else{
                    head = (head + 1) % MAX_LENGTH;
                }
                bit_counter = 0;
                TCCR1B &= ~(1 << CS10);     // stop timer
                sw_uart_state = IDLE;
                rx_ready = 1;
                PCICR |= (1u << PCIE2); // Re-activate external interrupts
                break;
            default:    // Receive the correct bit and store it in correct place
                bit = (PIND >> PD6) & 1u;
                buffer[head] |= bit << (bit_counter - 1);
                bit_counter++;
                break;
            }
            break;
        case TRANSMITTING:
            switch (bit_counter)
            {
            case 0:     //  Start bit
                PORTD   &= ~(1u << PD7);
                bit_counter++;
                break;
            case 9:     //  Stop bit
                PORTD   |= (1u << PD7);
                TCCR1B  &= ~(1 << CS10);
                bit_counter = 0;
                sw_uart_state = IDLE;
                break;
            default:    // Send the correct bit
                if((tx_byte >> (bit_counter - 1)) & 1u){
                        PORTD |=  (1u << PD7);
                }else{
                    PORTD &= ~(1u << PD7);
                }
                bit_counter++;
                    break;
                }
            break;
        case IDLE:
            break;
        default:
            break;
    } 
}
ISR(PCINT2_vect){
    //Enable timer with ARR half period to be able to sample start bit
     if(!(PIND & (1u << PD6))){     // confirm falling edge, not rising
        PCICR &= ~(1u << PCIE2);    // disable external interrupt, otherwise it will fire again during Rx
        OCR1A  = ARR/2;
        TCNT1  = 0;                 // reset counter
        TCCR1B |= (1u << CS10);
        sw_uart_state = START_SAMPLE;
    }
}
