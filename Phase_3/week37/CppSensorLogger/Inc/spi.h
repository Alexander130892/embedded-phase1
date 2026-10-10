/*
 * File:    spi.h
 * Author:  Alexander130892
 * Date:    10-10-2026
 *
 * Description:
 *   This header file defines the SPI interface for an STM32F446RE
 *   microcontroller, providing initialization and communication
 *   functions for SPI1 peripheral with pins mapped to PA5-PA7
 *   (SCK/MISO/MOSI) and PB6 (CS). It includes function declarations
 *   for SPI setup, GPIO configuration, single-byte transfers, and
 *   burst reads from specified addresses.
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
