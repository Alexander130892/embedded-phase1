/*
 * spi.c
 *
 *  Created on: May 3, 2026
 *      Author: alexandervindelinckx
 */

#include "spi.h"


status_t spi_init(void){
	//init clocks
	*((volatile uint32_t *)(RCC_BASE_ADDR + RCC_APB2ENR_OFFSET))	|= 	(1u << 12);		// Bit 12 for SPI1
	//CR1
	//CLEAR BITS
	*((volatile uint32_t*)(SPI1_BASE_ADDR + SPIx_CR1_OFFSET)) &= ~(
																		(1u << 15) 	// BIDIMODE > Clear for FULL-duplex mode
																	| 	(1u << 13) 	// CRCEN > No need for CRC
																	|	(1u << 11) 	// DFF > 8-bit data frame format
																	|	(1u << 10) 	// RXONLY > 0 = Full Duplex
																	|	(1u << 7) 	// LSBFIRST > 0 = MSB-first
																	|	(7u << 3)	// BAUDRATE --> Reset
																	|	(1u << 1)	// CPOL --> Mode 0
																	|	(1u << 0)	// CPHA --> Mode 0
																);
	//SET BITS
	*((volatile uint32_t*)(SPI1_BASE_ADDR + SPIx_CR1_OFFSET)) |=  (
																		(1u << 9) 	// SSM > Enable for Software Slave Management
																	|	(1u << 8)	// SSI > setting SSI=1 tells the peripheral internally "NSS is high, I'm the master"
																	|	(5u << 3)	// BAUDRATE --> f_plck/64 = 16Mhz/64 = 250kHz
																	|	(1u << 2)	// MSTR --> Set as master
																);


	//CR2
	//CLEAR BITS
	*((volatile uint32_t*)(SPI1_BASE_ADDR + SPIx_CR2_OFFSET)) &= ~(
																		(1u << 7) 	// TXEIE  > No interrupt-mode
																	| 	(1u << 6) 	// RXNEIE > No interrupt-mode
																	|	(1u << 5) 	// ERRIE  > No interrupt-mode
																	|	(1u << 4) 	// FRF 	  > Motorola Mode (Frame Format)
																	|	(1u << 2) 	// SS0E   > Multi-master mode enabled
																	|	(1u << 1)	// TXDMAEN > Tx buffer DMA Disabled
																	|	(1u << 0)	// RXDMAEN > Rx buffer DMA Disabled
																);

	//ENABLE SPI
	*((volatile uint32_t*)(SPI1_BASE_ADDR + SPIx_CR1_OFFSET)) |=  (1u << 6); 	// SPE --> Enable SPI
	return STATUS_OK;
}
status_t spi_gpio_init(void){
	//Enable Clock
	*((volatile uint32_t *)(RCC_BASE_ADDR + RCC_AHB1ENR_OFFSET)) 	|= 	(1u << 0);			//  Bit 0 for GPIOA
	// MODER
	// Clear bits
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET)) 	&= 	~(
																		 	(3u << 2*5)		// AF for PA5
																		|	(3u << 2*6)		// AF for PA6
																		|	(3u << 2*7));	// AF for PA7

	// Set bits
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET)) 	|= 	(
																		 	(2u << 2*5)		// AF (10) for PA5
																		|	(2u << 2*6)		// AF for PA6
																		|	(2u << 2*7));	// AF for PA7

	// AFRL
	// Clear bits
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_AFRL_OFFSET)) 	&= 	~(
																			(15u << 4*5)	// AF for PA5
																		|	(15u << 4*6)	// AF for PA6
																		|	(15u << 4*7));	// AF for PA7
	// Set bits
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_AFRL_OFFSET)) 	|= 	(
																			(5u << 4*5)		// AF 5 for SPI for PA5
																		|	(5u << 4*6)		// AF 5 for PA6
																		|	(5u << 4*7));	// AF 5 for PA7
	return STATUS_OK;
}
status_t spi_transfer(uint8_t tx_data, uint8_t *rx_data){
	uint32_t timeout=0;
	// Wait for TXE = 1 (transmit buffer empty --> Ready for Tx)
	while( !((*((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET))) & (1u << 1))){
		timeout++;
		if(timeout > MAX_TIMEOUT) return SPI_TIMEOUT;
	};
	// Write to DR
	*((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_DR_OFFSET)) = tx_data;
	// Wait for RXNE = 1 (Receive buffer NOT empty --> Ready for Rx)
	timeout=0;
	while( !((*((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET))) & (1u << 0))){
		timeout++;
		if(timeout > MAX_TIMEOUT) return SPI_TIMEOUT;
	};
	// Read and return DR
	*rx_data = *((volatile uint8_t *)(SPI1_BASE_ADDR + SPIx_DR_OFFSET));
	return STATUS_OK;
}
status_t spi_read_burst(uint8_t addr, uint8_t *rx_data, uint8_t len){
	status_t status=STATUS_OK;
	uint8_t dummy;
	status = spi_transfer(addr, &dummy);
	if(status){
		return status;
	}
	for(uint8_t byte = 0; byte < len ; byte++ ){
		status = spi_transfer(dummy, &rx_data[byte]);
		if(status){
			return status;
		}
	}
	return status;
}
