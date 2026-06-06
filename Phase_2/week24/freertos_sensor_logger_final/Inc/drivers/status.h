/*
 * status.h
 *
 *  Created on: May 16, 2026
 *      Author: alexandervindelinckx
 */

#ifndef STATUS_H_
#define STATUS_H_


#define MAX_TIMEOUT		10000
typedef enum {
    STATUS_OK      	= 0,
    SPI_TIMEOUT 	= 1,
	I2C_TIMEOUT 	= 2,
	I2C_AF			= 3,
	I2C_ARLO		= 4,
	UART_TIMEOUT	= 5
} status_t;


#endif /* STATUS_H_ */
