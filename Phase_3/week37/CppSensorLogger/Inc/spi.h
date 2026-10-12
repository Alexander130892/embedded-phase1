/*
 * File:    spi.h
 * Author:  Alexander130892
 * Date:    11-10-2026
 *
 * Description:
 *   This header file provides the interface for SPI (Serial Peripheral
 *   Interface) communication on an STM32F446RE microcontroller,
 *   declaring functions for initialization, GPIO setup, and data
 *   transfer operations. The SPI peripheral is configured to use pins
 *   PA5-PA7 for clock/MISO/MOSI and PB6 for chip select, compatible
 *   with Arduino pin mappings D13/D12/D11/D10.
 */
#ifndef SPI_H_
#define SPI_H_

#ifdef __cplusplus
extern "C" {
#endif
// SPI1_SCK 	--> PA5 -- D13
// SPI1_MISO 	-->	PA6	-- D12
// SPI1_MOSI	--> PA7 -- D11
// SPI1_CS		--> PB6 -- D10

#include <stdint.h>
#include "stm32f446re.h"
#include "status.h"

status_t spi_init(void);
status_t spi_gpio_init(void);
status_t spi_transfer(uint8_t tx_data, uint8_t *rx_data);
status_t spi_read_burst(uint8_t addr, uint8_t *rx_data, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /* SPI_H_ */
