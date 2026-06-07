/*
 * File:    uart.h
 * Author:  Alexander130892
 * Date:    7-6-2026
 *
 * Description:
 *   This header file defines configuration constants and data types
 *   for a UART (serial communication) interface operating at 9600 baud
 *   with a 16 MHz clock, including a 64-byte ring buffer and
 *   state/status enumerations for managing software-based UART
 *   operations.
 */
#ifndef UART_H
#define UART_H

#define FOSC    16000000        // Clock Speed
#define BAUD    9600            // Bits per second --> period 104 us per bit
#define PSC     1               // 1 / 16MHz = 62.5ns per tick, 
#define ARR     1663            // 104 us / 62.5ns = 1664 ticks --> ARR = 1663
#define MYUBRR  (FOSC/16/BAUD-1)  // Formula according datasheet

#define MAX_LENGTH  64          // RingBuffer Size

typedef enum { IDLE, START_SAMPLE, TRANSMITTING, RECEIVING } sw_uart_state_t;
typedef enum { STATUS_OK, FRAMING_ERROR, BUFFER_OVERFLOW, BUFFER_EMPTY} uart_return_state_t;

#endif