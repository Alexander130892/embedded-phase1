/*
 * spi.h
 *
 *  Created on: May 3, 2026
 *      Author: alexandervindelinckx
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

void init_gpio(void);
void init_spi(void);
uint8_t spi_transfer(uint8_t);

// SPI1_SCK 	--> PA5
// SPI1_MISO 	-->	PA6
// SPI1_MOSI	--> PA7
// SPI1_CS		--> PB6

#endif /* SPI_H_ */
