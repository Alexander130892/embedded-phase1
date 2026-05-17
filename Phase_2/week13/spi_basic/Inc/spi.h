/*
 * File:    spi.h
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This header file declares the interface for initializing and using
 *   SPI1 communication on an STM32 microcontroller, including GPIO
 *   setup and a byte-transfer function. It maps the SPI1 pins to PA5
 *   (SCK), PA6 (MISO), PA7 (MOSI), and PB6 (chip select).
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
