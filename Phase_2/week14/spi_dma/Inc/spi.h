/*
 * File:    spi.h
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This header declares the interface for an SPI1 peripheral driver
 *   using DMA-based transfers, providing functions to initialize GPIO
 *   pins, SPI, and DMA channels for both transmit and receive, along
 *   with blocking single-byte transfer support. It also exposes
 *   volatile DMA completion flags (`dma_s0_flag`, `dma_s3_flag`) used
 *   to signal when DMA read/write operations have finished.
 */
#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

extern volatile uint8_t dma_s0_flag; //1 = ready to be read, 0 nothing ready
extern volatile uint8_t dma_s3_flag; //1 = ready to be read, 0 nothing ready

void init_gpio(void);
void init_spi(void);
void init_dma_rx(uint8_t* memory_addr, uint32_t n_data);
void init_dma_tx(uint8_t* memory_addr, uint32_t n_data);
void spi_dma_read(uint32_t addr, uint32_t n_data, uint8_t* memory_addr);
void spi_dma_write(uint8_t* memory_addr, uint32_t n_data);
uint8_t spi_transfer(uint8_t);

// SPI1_SCK 	--> PA5
// SPI1_MISO 	-->	PA6
// SPI1_MOSI	--> PA7
// SPI1_CS		--> PB6

#endif /* SPI_H_ */
