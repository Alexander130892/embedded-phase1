/*
 * spi.h
 *
 *  Created on: May 3, 2026
 *      Author: alexandervindelinckx
 */

#ifndef SPI_H_
#define SPI_H_

// SPI1_SCK 	--> PA5 -- D13
// SPI1_MISO 	-->	PA6	-- D12
// SPI1_MOSI	--> PA7 -- D11
// SPI1_CS		--> PB6 -- D10

#include <stdint.h>
#include "stm32f446re.h"
#include "drivers/status.h"

status_t spi_init(void);
status_t spi_gpio_init(void);
status_t spi_transfer(uint8_t tx_data, uint8_t *rx_data);
status_t spi_read_burst(uint8_t addr, uint8_t *rx_data, uint8_t len);

#endif /* SPI_H_ */
