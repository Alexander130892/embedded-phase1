/*
 * File:    spi.c
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This file initializes and configures SPI1 on an STM32F446RE
 *   microcontroller as a master device in full-duplex, Mode 0,
 *   MSB-first operation at 250 kHz, along with the associated GPIO
 *   pins (PA5–PA7 for SPI signals via AF5, PB3–PB6 for chip-select
 *   lines). It also provides a `spi_transfer` function for byte-level
 *   SPI communication and declares DMA stream flags, suggesting
 *   support for DMA-based transfers elsewhere.
 */
#include "spi.h"
#include "stm32f446re.h"

volatile uint8_t dma_s0_flag = 0;
volatile uint8_t dma_s3_flag = 0;

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
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	&= 		~(3u << 2*5);	// GPIO for PB5
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	&= 		~(3u << 2*4);	// GPIO for PB4
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	&= 		~(3u << 2*3);	// GPIO for PB3
	// Set bits
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET)) 	|= 	(
																		 	(2u << 2*5)		// AF (10) for PA5
																		|	(2u << 2*6)		// AF for PA6
																		|	(2u << 2*7));	// AF for PA7
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	|= 		(1u << 2*6);	// GPIO (01) for PB6
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	|= 		(1u << 2*5);	// GPIO (01) for PB5
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	|= 		(1u << 2*4);	// GPIO (01) for PB4
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	|= 		(1u << 2*3);	// GPIO (01) for PB3
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
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET))	|= 		(1u << 4); 		// PB4 -  (CS idle high (active low logic) so communication does not start before SPI is enabled

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
void init_dma_rx(uint8_t* memory_addr, uint32_t n_data){
	//CR1
		//Clear EN before reconfiguring
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S0CR_OFFSET)) &=  ~(1u << 0); 	// EN --> Disable DMA2
		//Wait until EN = 0
		while(*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S0CR_OFFSET)) &  (1u << 0));
		//CLEAR BITS
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S0CR_OFFSET)) &= ~(
																			(7u << 25) 	// CHSEL --> Reset before setting to Ch 3
																		| 	(3u << 23) 	// MBURST -> Single Transfer
																		|	(3u << 21) 	// PBURST -> Single Transfer
																		|	(1u << 19) 	// CT 	  -> 0 Current target is M0 (Single/double buffer mode)
																		|	(1u << 18) 	// DBM 	  -> 0 = Single Buffer Mode
																		|	(3u << 16)	// Priority level --> 00 = Low
																		|	(1u << 15)	// PINCOS --> 0 = Offset size for PAR is linked to PSIZE
																		|	(3u << 13)	// MSZIE --> 00 = 8bit
																		|	(3u << 11)	// PSZIE --> 00 = 8bit
																		|	(1u << 9)	// PINC --> 0 = Peripheral address Pointer fixed
																		|	(1u << 8)	// CIRC --> 0 = Circular mode disabled
																		|	(3u << 6)	// DIR --> Data transfer direction 00 = Per->Mem
																		|	(1u << 5)	// PFCTRL --> 0 = Peripheral flow controller, 0 = DMA is flow controller
																		|	(1u << 3)	// HTIE --> 0 = Half Transfer Interrupt Enable
																		|	(1u << 2)	// TEIE --> 0 = Transfer Error Interrupt Enable
																		|	(1u << 1)	// DMEIE --> 0 = Direct Mode Error Interrupt Enable
																		|	(1u << 0)	// EN --> 0 = Enabled

																	);
		//SET BITS
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S0CR_OFFSET)) |=  (
																			(3u << 25) 	// CHSEL --> Channel 3
																		|	(1u << 10)	// MINC --> 1 = Memory address Pointer NOT fixed
																		|	(1u << 4)	// TCIE --> 1 = Transfer complete interrupt enable
																	);

	//NDTR
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S0NDTR_OFFSET)) = n_data;
	//PAR
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S0PAR_OFFSET)) = SPI1_BASE_ADDR + SPIx_DR_OFFSET;
	//M0AR
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S0M0AR_OFFSET)) = (volatile uint32_t) memory_addr;
	//Configure NVIC
		*((volatile uint32_t*)(NVIC_BASE_ADDR + NVIC_ISER1_OFFSET))		|= (0x1 << DMA2_S0_POS);

}
void init_dma_tx(uint8_t* memory_addr, uint32_t n_data){
	//CR1
		//Clear EN before reconfiguring
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S3CR_OFFSET)) &=  ~(1u << 0); 	// EN --> Disable DMA2
		//Wait until EN = 0
		while(*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S3CR_OFFSET)) &  (1u << 0));
		//CLEAR BITS
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S3CR_OFFSET)) &= ~(
																			(7u << 25) 	// CHSEL --> Reset before setting to Ch 3
																		| 	(3u << 23) 	// MBURST -> Single Transfer
																		|	(3u << 21) 	// PBURST -> Single Transfer
																		|	(1u << 19) 	// CT 	  -> 0 Current target is M0 (Single/double buffer mode)
																		|	(1u << 18) 	// DBM 	  -> 0 = Single Buffer Mode
																		|	(3u << 16)	// Priority level --> 00 = Low
																		|	(1u << 15)	// PINCOS --> 0 = Offset size for PAR is linked to PSIZE
																		|	(3u << 13)	// MSZIE --> 00 = 8bit
																		|	(3u << 11)	// PSZIE --> 00 = 8bit
																		|	(1u << 9)	// PINC --> 0 = Peripheral address Pointer fixed
																		|	(1u << 8)	// CIRC --> 0 = Circular mode disabled
																		|	(3u << 6)	// DIR --> Data transfer direction 01 = Mem->PER
																		|	(1u << 5)	// PFCTRL --> 0 = Peripheral flow controller, 0 = DMA is flow controller
																		|	(1u << 3)	// HTIE --> 0 = Half Transfer Interrupt Enable
																		|	(1u << 2)	// TEIE --> 0 = Transfer Error Interrupt Enable
																		|	(1u << 1)	// DMEIE --> 0 = Direct Mode Error Interrupt Enable
																		|	(1u << 0)	// EN --> 0 = Enabled

																	);
		//SET BITS
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S3CR_OFFSET)) |=  (
																			(3u << 25) 	// CHSEL --> Channel 3
																		|	(1u << 10)	// MINC --> 1 = Memory address Pointer NOT fixed
																		|	(1u << 6)	// DIR --> Data transfer direction 01 = Mem->PER
																		|	(1u << 4)	// TCIE --> 1 = Transfer complete interrupt enable
																	);

	//NDTR
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S3NDTR_OFFSET)) = n_data;
	//PAR
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S3PAR_OFFSET)) = SPI1_BASE_ADDR + SPIx_DR_OFFSET;
	//M0AR
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S3M0AR_OFFSET)) = (volatile uint32_t) memory_addr;
	//Configure NVIC
		*((volatile uint32_t*)(NVIC_BASE_ADDR + NVIC_ISER1_OFFSET))		|= (0x1 << DMA2_S3_POS);

}
void spi_dma_read(uint32_t addr, uint32_t n_data, uint8_t* memory_addr){
	//Flush
	(void)*((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_DR_OFFSET));  // Flush stale RX data
	// Prepare DMA
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_LIFCR_OFFSET)) |= (
																			(1u << 5) 	// CTCIF0 --> Set Clear Transfer Complete Interrupt Flag
																		| 	(1u << 4) 	// CHTIF0 --> Set Clear Half Transfer Interrupt Flag
																		|	(1u << 3) 	// CTEIF0 --> Set Clear Transfer Error Interrupt Flag
																		|	(1u << 2) 	// CDMEIF0 --> Set Clear Direct Mode Error Interrupt Flag
																	);
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S0NDTR_OFFSET)) = n_data;			// Number of data transmission to be received
		//M0AR
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S0M0AR_OFFSET)) = (volatile uint32_t) memory_addr;
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S0CR_OFFSET)) 		|=  (1u << 0); 	// EN --> Enable DMA2
	// Prepare SPI
		*((volatile uint32_t*)(SPI1_BASE_ADDR + SPIx_CR2_OFFSET)) 		|= (1u << 0); 	//RXDMAEN --> Rx Buffer DMA Enable
		*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) 	&= ~(1u << 6);	// Set CS low
	// Trigger
		*((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_DR_OFFSET)) = (addr | 0x80);
		while(n_data -1 > 0){
			while( !((*((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET))) & (1u << 1))){};
			*((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_DR_OFFSET)) = 0x00;
			n_data--;
			// Wait for TXE = 1 (transmit buffer empty --> Ready for Tx)

		}
	// return void (goes straight into memory)
}
void spi_dma_write(uint8_t* memory_addr, uint32_t n_data){
	// Prepare SPI
			*((volatile uint32_t*)(SPI1_BASE_ADDR + SPIx_CR2_OFFSET)) 		|= (1u << 1); 	// TXDMAEN --> Tx Buffer DMA Enable
			*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) 	&= ~(1u << 4);	// Set CS low
	// Prepare DMA
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_LIFCR_OFFSET)) |= (
																			(1u << 27) 	// CTCIF3 --> Set Clear Transfer Complete Interrupt Flag
																		| 	(1u << 26) 	// CHTIF3 --> Set Clear Half Transfer Interrupt Flag
																		|	(1u << 25) 	// CTEIF3 --> Set Clear Transfer Error Interrupt Flag
																		|	(1u << 24) 	// CDMEIF3 --> Set Clear Direct Mode Error Interrupt Flag
																	);
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S3NDTR_OFFSET)) = n_data;			// Number of data transmission to be received


	//M0AR
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S3M0AR_OFFSET)) = (volatile uint32_t) memory_addr;
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S3CR_OFFSET)) 		|=  (1u << 0); 	// EN --> Enable DMA2


	// return void (CPU just needs to prepare DMA, enable it and return)
}

void DMA2_Stream0_IRQHandler(){
		//DMA
		*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_LIFCR_OFFSET)) 	|= (
	                                                                             (1u << 5)     // CTCIF0 --> Set Clear Transfer Complete Interrupt Flag
	                                                                        |    (1u << 4)     // CHTIF0 --> Set Clear Half Transfer Interrupt Flag
	                                                                        |    (1u << 3)     // CTEIF0 --> Set Clear Transfer Error Interrupt Flag
	                                                                        |    (1u << 2)     // CDMEIF0 --> Set Clear Direct Mode Error Interrupt Flag
	                                                                    );

	    *((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S0CR_OFFSET)) &= ~(1u << 0);     // EN --> Disable DMA2
	    //SPI
	    *((volatile uint32_t*)(SPI1_BASE_ADDR + SPIx_CR2_OFFSET)) &= ~(1u << 0);     //RXDMAEN --> __Rx__ Buffer DMA Disable
	    //Wait until BSY status cleared before reseting CS to not clip
		while( *((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET)) & (1u << 7) );
		// Re-Set CS High
		*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) |=  (1u << 6);
		dma_s0_flag = 1; //helper to show data is available
}
void DMA2_Stream3_IRQHandler(){

	//DMA
	*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_LIFCR_OFFSET)) 	|= (
																			 (1u << 27)     // CTCIF3 --> Set Clear Transfer Complete Interrupt Flag
																		|    (1u << 26)     // CHTIF3 --> Set Clear Half Transfer Interrupt Flag
																		|    (1u << 25)     // CTEIF3 --> Set Clear Transfer Error Interrupt Flag
																		|    (1u << 24)     // CDMEIF3 --> Set Clear Direct Mode Error Interrupt Flag
																	);

	*((volatile uint32_t*)(DMA2_BASE_ADDR + DMA_S3CR_OFFSET)) &= ~(1u << 0);     // EN --> Disable DMA2
	//SPI
	*((volatile uint32_t*)(SPI1_BASE_ADDR + SPIx_CR2_OFFSET)) &= ~(1u << 1);     // TXDMAEN --> Tx Buffer DMA Disable
	//Wait until BSY status cleared before reseting CS to not clip
	while( *((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET)) & (1u << 7) );
	(void)*((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_DR_OFFSET));  // Flush stale RX
	(void)*((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET));  // Clear OVR flag
	// Re-Set CS High
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) |=  (1u << 4);
	dma_s3_flag = 1; //helper to show data is available
}
