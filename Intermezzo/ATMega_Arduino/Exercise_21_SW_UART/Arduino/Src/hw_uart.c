#include "sw_uart.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//const uint8_t tx_data[] = {0x01, 0x02, 0x03}; - No longer need, was used for ISR test
// volatile uint8_t tx_index = 0; - No longer need, was used for ISR test


// ----- HW UART -----
uart_return_state_t hw_uart_init(uint16_t ubrr){ // HW UART
    //Set baud rate
    UBRR0H  = (uint8_t)(ubrr>>8);
    UBRR0L  = (uint8_t)ubrr;
    // Enable UART data register empty interrupt (and USART Receive Complete Interrupt -- not needed)
    // UCSR0B  |= (1u << UDRIE0);       //| (1u << RXCIE0);    -- Interrupt mode
    // Set frame format: 8data, 1stop bit 
    UCSR0C  |= (0 << USBS0)|(3u << UCSZ00);
    // Enable transmitter 
    UCSR0B  |= (1u << TXEN0);       //|(1u << RXEN0) -- Rx if needed
    return STATUS_OK;
}
uart_return_state_t hw_uart_transmit(uint8_t data){
    while(!(UCSR0A & (1u << UDRE0))); //Wait until UART is not busy
    UDR0 = data;
    return STATUS_OK;
}

// Interrupt mode - no longer needed
// ISR(USART_UDRE_vect) {
//     if(tx_index < 3) {
//         UDR0 = tx_data[tx_index++];   // 2 operations
//     } else {
//         UCSR0B &= ~(1u << UDRIE0);    // done
//     }
// }
// No longer Needed
// ISR(USART_RX_vect) {
//     uint8_t received = UDR0;    // must read UDR0 to clear the flag
//     if(received) {
//         PORTB |=  (1u << PB5);  // LED on
//     } else {
//         PORTB &= ~(1u << PB5);  // LED off
//     }
// }
