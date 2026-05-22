
/*
    Notes:
        
    Mapping:
        Rx 0   -   D0  -   PD0   --   //using PD0/PD1 means UART TX/RX is not available
        Tx 1   -   D1  -   PD1
        

    Wiring:
    PD0 (UnoRx) -- Pxx (STM Tx)     -- Ch 1 (Logic Analyzer)
    PD1 (UnoTx) -- Pxx (STM Rx)     -- Ch 2 (Logic Analyzer)
    GND         --  GND             -- GND  (Logic Analyzer)
    USB -- STM  -- 5V -- 5V (Uno)

*/

/*
Exercise:
    Arduino: (This main.c)
        Tx-part of transmission test
            Configure UART at BR = 9600
            8N1 - frameformat
            Send 3 bytes: 0x1, 0x2, 0x3
            Use interrupts and ISR where needed
        Rx
            STM will send true/false
            Turn on/off internal LED

    STM32: Rx - part of transmission test
        Configure UART at BR = 9600
        8N1 - frameformat
        receive 3 bytes: 0x1, 0x2, 0x3
        Use interrupts and ISR where needed
        Show output on STM UART
        Show output with logic analyzer

        Tx
            send 0x00 / 0x1 to turn on/off UNO internal LED in random intervals
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

#define FOSC    16000000        // Clock Speed
#define BAUD    9600            // Bits per second
#define MYUBRR  (FOSC/16/BAUD-1)  // Formula according datasheet

const uint8_t tx_data[] = {0x01, 0x02, 0x03};
volatile uint8_t tx_index = 0;

void USART_init(uint16_t ubrr);

int main(void)
{
    USART_init(MYUBRR);
    DDRB |= (1u << PB5);   // PB5 as output
    sei();    // Set Enable Interrupts

    for(;;) { // infinite loop
        
    }
    return 0;   // never reached
}

void USART_init(uint16_t ubrr)
{
    //Set baud rate
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;
    // Enable UART data register empty interrupt and USART Receive Complete Interrupt
    UCSR0B |= (1u << UDRIE0) | (1u << RXCIE0);    
    // Set frame format: 8data, 1stop bit 
    UCSR0C |= (0 << USBS0)|(3u << UCSZ00);
    // Enable receiver and transmitter 
    UCSR0B |= (1u << RXEN0)|(1u << TXEN0);
}

ISR(USART_UDRE_vect) {
    if(tx_index < 3) {
        UDR0 = tx_data[tx_index++];   // 2 operations
    } else {
        UCSR0B &= ~(1u << UDRIE0);    // done
    }
}

ISR(USART_RX_vect) {
    uint8_t received = UDR0;    // must read UDR0 to clear the flag
    if(received) {
        PORTB |=  (1u << PB5);  // LED on
    } else {
        PORTB &= ~(1u << PB5);  // LED off
    }
}