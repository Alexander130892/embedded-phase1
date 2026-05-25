/*
 * File:    status.h
 * Author:  Alexander130892
 * Date:    25-5-2026
 *
 * Description:
 *   This header file defines a status enumeration type that represents
 *   various communication and timeout error codes for SPI, I2C, and
 *   UART interfaces, along with a maximum timeout constant used across
 *   the codebase.
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
