/*
 * File:    status.h
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This header defines a `status_t` enumeration of error/status codes
 *   for peripheral communication interfaces (SPI, I2C, and UART),
 *   along with a maximum timeout constant. It serves as a shared
 *   status reporting type used across the project's peripheral
 *   drivers.
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
