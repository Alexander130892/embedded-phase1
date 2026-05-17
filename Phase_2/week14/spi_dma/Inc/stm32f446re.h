/*
 * File:    stm32f446re.h
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This header file defines base addresses and register offsets for
 *   the STM32F446RE microcontroller's peripherals, including RCC,
 *   GPIO, TIM2, NVIC, SYSCFG, EXTI, USART2, SPI1, and DMA2, enabling
 *   direct memory-mapped register access without relying on vendor HAL
 *   libraries. It serves as a low-level hardware abstraction layer for
 *   bare-metal programming on the STM32F446RE.
 */
#ifndef STM32F46RE_H_
#define STM32F46RE_H_

//for Peripheral clocks
#define RCC_BASE_ADDR      	0x40023800
#define RCC_APB1ENR_OFFSET 	0x40		// TIM2 bit0
#define RCC_APB2ENR_OFFSET 	0x44		// SYSCFG bit 14, SPI1
#define RCC_AHB1ENR_OFFSET 	0x30		// GPIOA

// TIM2 registers:
#define TIM2_BASE_ADDR 		0x40000000
#define TIMx_CR1_OFFSET 	0x00		// Bit 4 DIR = default up = 0  // Bit 0 CEN = counter enable
#define TIMx_DIER_OFFSET	0x0C		// Bit 0 > interrupt enable
#define TIMx_SR_OFFSET		0x10		// Bit 0, update interrupt flag UIF
#define TIMx_EGR_OFFSET		0x14		// Bit 0 = Update Generation
#define TIMx_CNT_OFFSET		0x24 		// (16 bit) counter
#define TIMx_PSC_OFFSET		0x28 		// 16 bit Prescaler
#define TIMx_ARR_OFFSET 	0x2C 		// Auto reload value 16 bit
#define TIMx_CCMR1_OFFSET	0x18		// 110 bit 6:4 for PWM
#define TIMx_CCR1_OFFSET	0x34		// Capture compare register (duty cycle) D = CCR/(ARR+1)
#define TIMx_CCER_OFFSET	0x20		// Enable

//for GPIOA
#define GPIOA_BASE_ADDR   	0x40020000
#define GPIOx_MODER_OFFSET	0x00 		// write 01 for correct PA pin TBC
#define	GPIOx_IDR_OFFSET	0x10
#define GPIOx_ODR_OFFSET	0x14 		// to correct pin TBC
#define GPIOx_AFRL_OFFSET   0x20		// Alternate function
#define GPIOx_OSPEEDR_OFFSET 0x08   	// Output speed — set high speed for SPI pins
#define GPIOx_PUPDR_OFFSET   0x0C   	// Pull-up/pull-down
#define GPIOx_OTYPER_OFFSET  0x04   	// Output type (push-pull vs open-drain)

// For GPIOB + offset see above
#define GPIOB_BASE_ADDR	  	0x40020400

//NVIC
#define NVIC_BASE_ADDR		0xE000E100
#define NVIC_ISER0_OFFSET	0x00
#define TIM2_POS			28			// bit position in ISER0
#define NVIC_ISER1_OFFSET   0x04
#define EXTI15_10_POS       8    		// bit position in ISER1
#define USART2_POS  		6  			// bit position in ISER1
#define DMA2_S0_POS  		24 			// bit position in ISER1
#define DMA2_S3_POS  		27 			// bit position in ISER1

//SYSCNG
#define SYSCFG_BASE_ADDR 		0x40013800
#define SYSCFG_EXTICR3_OFFSET	0x10	//0001 for PB10 in [11:8]

//EXTI
#define EXTI_BASE_ADDR		0x40013C00
#define	EXTI_IMR_OFFSET		0x00		//bit 10 for PB10
#define	EXTI_RTSR_OFFSET	0x08		//Bit 10 for rising edge detection on PB10
#define	EXTI_PR_OFFSET		0x14		//Bit 10 Pending Register PB10

//USART
#define USART2_BASE_ADDR   	0x40004400
#define USART_SR_OFFSET		0x00 		// 8bit for data transfer
#define USART_DR_OFFSET		0x04 		// 8bit for data transfer
#define	USART_BRR_OFFSET	0x08 		// __Baudrate__ --> 0x8B for desired BR [15:4]_Mantissa - [3:0] Fraction
#define USART_CR1_OFFSET	0x0C 		// Control register

//SPI1
#define SPI1_BASE_ADDR		0x40013000
#define SPIx_CR1_OFFSET		0x00 		// Control register
#define SPIx_CR2_OFFSET     0x04    	// SSOE, RXNEIE, TXEIE, ERRIE
#define SPIx_SR_OFFSET		0x08 		// Status register
#define SPIx_DR_OFFSET		0x0C 		// Data register

//DMA2
#define DMA2_BASE_ADDR		0x40026400
// SPI1_RX is mapped to DMA2, Stream 0, Channel 3
#define DMA_S0CR_OFFSET		(0x10+(0*0x18)) 	// Stream 0 Config Register
#define DMA_S0NDTR_OFFSET	(0x14+(0*0x18)) 	// Stream 0 Number of Data to Transfer Register
#define DMA_S0PAR_OFFSET	(0x18+(0*0x18)) 	// Stream 0 Peripheral Address Register
#define DMA_S0M0AR_OFFSET	(0x1C+(0*0x18)) 	// Stream 0 Memory0 Address Register
#define DMA_S0M1AR_OFFSET	(0x20+(0*0x18)) 	// Stream 0 Memory1 Address Register
// SPI1_TX is mapped to DMA2, Stream 3, Channel 3
#define DMA_S3CR_OFFSET		(0x10+(3*0x18)) 	// Stream 3 Config Register
#define DMA_S3NDTR_OFFSET	(0x14+(3*0x18)) 	// Stream 3 Number of Data to Transfer Register
#define DMA_S3PAR_OFFSET	(0x18+(3*0x18)) 	// Stream 3 Peripheral Address Register
#define DMA_S3M0AR_OFFSET	(0x1C+(3*0x18)) 	// Stream 3 Memory0 Address Register
#define DMA_S3M1AR_OFFSET	(0x20+(3*0x18)) 	// Stream 3 Memory1 Address Register
//Interrupts / Flags
#define DMA_LISR_OFFSET		(0x0) 				// Low Interrupt Status Register (Bit 27:24 for STREAM 3)
#define DMA_LIFCR_OFFSET	(0x8) 				// Low Interrupt Flag Clear Register (Bit 27:24 for STREAM 3)
//High interrupt not implemented

#endif /* STM32F46RE_H_ */
