/*
 * File:    status.h
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   Defines a `status_t` enumeration of return codes for peripheral
 *   communication errors (SPI, I2C, and UART timeouts and faults)
 *   along with a maximum timeout constant, intended for use as a
 *   shared status/error type across the codebase.
 */
#ifndef STATUS_H_
#define STATUS_H_


#define MAX_TIMEOUT		100000
typedef enum {
    STATUS_OK      	= 0,
    SPI_TIMEOUT 	= 1,
	I2C_TIMEOUT 	= 2,
	I2C_AF			= 3,
	I2C_ARLO		= 4,
	UART_TIMEOUT	= 5
} status_t;


#endif /* STATUS_H_ */
