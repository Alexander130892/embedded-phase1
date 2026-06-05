/*
 * File:    status.h
 * Author:  Alexander130892
 * Date:    31-5-2026
 *
 * Description:
 *   This header file defines a status enumeration and timeout constant
 *   used for error reporting across various communication protocols
 *   (SPI, I2C, UART) in the embedded system. It provides standardized
 *   status codes to indicate successful operations or specific timeout
 *   and communication failure conditions.
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
