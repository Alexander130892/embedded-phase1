/*
 * File:    spi.h
 * Author:  Alexander130892
 * Date:    6-5-2026
 *
 * Description:
 *   This header file defines the interface for SPI (Serial Peripheral
 *   Interface) communication functions including GPIO and SPI
 *   initialization, data transfer, and specifies the pin assignments
 *   for SPI1 peripheral (SCK on PA5, MISO on PA6, MOSI on PA7, CS on
 *   PB6).
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
