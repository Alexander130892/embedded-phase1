/*
 * spi.c
 *
 *  Created on: May 3, 2026
 *      Author: alexandervindelinckx
 */

#include "spi.h"
#include "stm32f446re.h"


void init_spi(void){
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

}
void init_gpio(void){
	// MODER
	// Clear bits
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET)) 	&= 	~(
																		 	(3u << 2*5)		// AF for PA5
																		|	(3u << 2*6)		// AF for PA6
																		|	(3u << 2*7));	// AF for PA7
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	&= 		~(3u << 2*6);	// GPIO for PB6
	// Set bits
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET)) 	|= 	(
																		 	(2u << 2*5)		// AF (10) for PA5
																		|	(2u << 2*6)		// AF for PA6
																		|	(2u << 2*7));	// AF for PA7
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	|= 		(1u << 2*6);	// GPIO (01) for PB6
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
	// Set CS (Default to active low)
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET))	|= 		(1u << 6); 		// PB6 -  (CS idle high (active low logic) so communication does not start before SPI is enabled
}
uint8_t spi_transfer(uint8_t data){
	// Wait for TXE = 1 (transmit buffer empty --> Ready for Tx)
	while( !((*((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET))) & (1u << 1))){};
	// Write to DR
	*((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_DR_OFFSET)) = data;
	// Wait for RXNE = 1 (Receive buffer NOT empty --> Ready for Rx)
	while( !((*((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET))) & (1u << 0))){};
	// Read and return DR
	return *((volatile uint8_t *)(SPI1_BASE_ADDR + SPIx_DR_OFFSET));
}

