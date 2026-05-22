/*
 * File:    spi.h
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This header declares the interface for SPI1 peripheral
 *   communication on an STM32 (or similar) microcontroller, including
 *   GPIO and SPI initialization, DMA-based read/write transfers, and a
 *   blocking single-byte transfer function. It exposes two volatile
 *   flags to signal when DMA stream 0 and stream 3 receive operations
 *   have completed and data is ready to be read.
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
