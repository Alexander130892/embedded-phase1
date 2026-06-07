/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    30-5-2026
 *
 * Description:
 *   This Arduino (AVR) program implements a software UART transmitter
 *   that sends three bytes (0x1, 0x2, 0x3) to an STM32 microcontroller
 *   at 9600 baud, while simultaneously receiving ASCII characters from
 *   the STM32 via software UART and echoing them to the serial monitor
 *   via hardware UART.
 */
/*
    Notes:
        Some garbage values on start-up when transmitting
            during set-up, GPIO are undefined --> to get rid of it, use external pull-up
        Added state-machine to reuse timer-isr
        Did not implement ReceiveByte() as described since I felt it cleaner to keep it in the Timer-ISR
            desired functionality is kept
        For output from STM to Arduino to Terminal, HW UART is used with polling logic iso interrupt
        Added check if Tx/Rx available since both STM/Arduino would want access to Tx/Rx line (CSMA-principle) 
        Need to set-up a master on half-duplex line (Chosen STM, arduino will check Tx/Rx line before transmitting)
        Timing coordination important, "lazy" delays of STM/Arduino need to match a bit to allow enough time for processing the bits
        writeBuffer() not implemented as we directly integrated the ringbuffer in the ISR
        receiveByte() not implemented as it seems logically same as readByte() in this config
        Refactored into 3 seperate files, main.c, sw_uart.c, hw_uart.c for clarity
        
        Tests done:
            STM output to Arduino confirmed via Arduino Serial monitor  (clean ABC - continuously)
            Arduino output to STM - confirmed via STM Serial monitor    (clean 1,2,3 at Arduino reset)
            Output/Baudrate confirmed w/ Logic Analyzer

        Future work:
            Error flags - 
                Implemented, but not really
                No retries, no error-reporting
                Only visible in debugger at the moment

    Mapping:
        Rx   -   D6  -   PD6     
        Tx   -   D7  -   PD7
        

    Wiring:
        PD6 (UnoRx) -- PA0 (STM Tx)     -- Ch 1 (Logic Analyzer)
        PD7 (UnoTx) -- PA1 (STM Rx)     -- Ch 2 (Logic Analyzer)
        GND         --  GND             -- GND  (Logic Analyzer)
        USB -- STM  -- 5V -- 5V (Uno)

*/

/*
Exercise:
    Arduino: (This main.c)
        Tx-part of transmission test
            Configure SW UART at BR = 9600
            8N1 - frameformat
            Send 3 bytes: 0x1, 0x2, 0x3
            Use interrupts and ISR where needed
        Rx
            STM will send A,B,C,'\n' in succession
            write to HW_uart to test reception succes

    STM32: Rx - part of transmission test
        Configure UART at BR = 9600 -- HW UART will be used
        8N1 - frameformat
        receive 3 bytes: 0x1, 0x2, 0x3
        Use interrupts and ISR where needed
        Show output on STM UART
        Show output with logic analyzer

        Tx
            send A,B,C,\n
    STM is master in UART to avoid conflict on half-duplex line
 */

#include "hw_uart.h"

#define WAKEUP_DELAY 10000


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

#include "uart.h"
#include "hw_uart.h"

int main(void)
{
    hw_uart_init(MYUBRR);
    sei();                  // Set Enable Interrupts
    for(volatile uint32_t d = 0; d < 10000; d++);  // blocking delay

    for(volatile uint32_t d = 0; d < 10000; d++);  // blocking delay

    for(;;) {
        char output[MAX_LENGTH];
        if(uart_receive_string(output) == STATUS_OK) {
            uint8_t index = 0;
            while(output[index] != '\0') {
                hw_uart_transmit(output[index++]);
            }
            hw_uart_transmit('\r');
            hw_uart_transmit('\n');
        }
    }
    return 0;   // never reached
}


