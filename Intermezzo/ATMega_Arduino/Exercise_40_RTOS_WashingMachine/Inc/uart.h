/*
 * File:    uart.h
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This header file defines configuration constants and data types
 *   for a UART (serial communication) interface operating at 9600 baud
 *   with a 16 MHz clock, including buffer management and error state
 *   definitions. It calculates the necessary timer prescaler and
 *   auto-reload values for bit-rate timing on an embedded
 *   microcontroller.
 */
#ifndef UART_H
#define UART_H

#define FOSC    16000000u        // Clock Speed
#define BAUD    9600u            // Bits per second --> period 104 us per bit
#define PSC     1u               // 1 / 16MHz = 62.5ns per tick, 
#define ARR     1663u            // 104 us / 62.5ns = 1664 ticks --> ARR = 1663
#define MYUBRR  ((FOSC)/(16u)/(BAUD)-1u)  // Formula according datasheet

#define UART_BUFFER_SIZE  64          // RingBuffer Size

typedef enum { SW_UART_IDLE, START_SAMPLE, TRANSMITTING, RECEIVING } sw_uart_state_t; // not used here
typedef enum { STATUS_OK, FRAMING_ERROR, BUFFER_OVERFLOW, BUFFER_EMPTY, UART_BUSY} uart_return_state_t;

#endif