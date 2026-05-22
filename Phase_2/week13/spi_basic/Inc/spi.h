/*
 * File:    spi.h
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This header file declares the interface for initializing and using
 *   the SPI1 peripheral, including GPIO setup for the SPI pins (SCK on
 *   PA5, MISO on PA6, MOSI on PA7, and chip select on PB6) and a
 *   byte-level transfer function.
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
