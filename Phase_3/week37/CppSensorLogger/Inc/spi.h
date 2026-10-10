/*
 * File:    spi.h
 * Author:  Alexander130892
 * Date:    10-10-2026
 *
 * Description:
 *   This header file defines the SPI (Serial Peripheral Interface)
 *   communication interface for an STM32F446RE microcontroller,
 *   declaring functions to initialize SPI peripherals and perform data
 *   transfers over the configured pins (SCK, MISO, MOSI, and CS). It
 *   provides both single-byte and burst read operations with standard
 *   status return codes.
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
