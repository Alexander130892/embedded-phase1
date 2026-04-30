/*
 * File:    stm32f446re.h
 * Author:  Alexander130892
 * Date:    29-4-2026
 *
 * Description:
 *   This header file defines memory-mapped register addresses and
 *   offsets for STM32F446RE microcontroller peripherals including
 *   GPIO, timers, RCC clock control, NVIC interrupts, EXTI external
 *   interrupts, and USART communication. It serves as a hardware
 *   abstraction layer providing symbolic names for direct register
 *   access instead of using hard-coded memory addresses.
 */
#ifndef STM32F46RE_H_
#define STM32F46RE_H_

#define GPIOA_BASE_ADDR   	0x40020000

// RCC
#define RCC_BASE_ADDR      	0x40023800

//for Peripheral clocks
#define RCC_APB1ENR_OFFSET 	0x40	// Timer
#define RCC_AHB1ENR_OFFSET 	0x30	// GPIOA
#define RCC_APB2ENR_OFFSET 	0x44	// SYSCFG bit 14

// TIM2 registers:
#define TIM2_BASE_ADDR 		0x40000000
#define TIMx_CR1_OFFSET 	0x00
	// Bit 4 DIR = default up = 0
	// Bit 0 CEN = counter enable
#define TIMx_DIER_OFFSET	0x0C	//bit 0 > interrupt enable

#define TIMx_SR_OFFSET		0x10		// Bit 0, update interrupt flag UIF
#define TIMx_EGR_OFFSET		0x14		// Bit 0 = Update Generation
#define TIMx_CNT_OFFSET		0x24 		// (16 bit) counter
#define TIMx_PSC_OFFSET		0x28 		// 16 bit Prescaler
#define TIMx_ARR_OFFSET 	0x2C 		// Auto reload value 16 bit
#define TIMx_CCMR1_OFFSET	0x18		// 110 bit 6:4 for PWM
#define TIMx_CCR1_OFFSET	0x34		// Capture compare register (duty cycle) D = CCR/(ARR+1)
#define TIMx_CCER_OFFSET	0x20		// Enable


//for LED D2 = PA10
#define GPIOA_MODER_OFFSET	0x00 	// write 01 for correct PA pin TBC
#define GPIOA_ODR_OFFSET	0x14 	// to correct pin TBC
#define GPIOA_AFRL_OFFSET   0x20	// Alternate function (0x7 for USART)

//NVIC
#define NVIC_BASE_ADDR		0xE000E100
#define NVIC_ISER0_OFFSET	0x00
#define TIM2_POS			28


#define NVIC_ISER1_OFFSET   0x04
#define EXTI15_10_POS       8    	// bit position within ISER1
#define USART2_POS  		6  		// bit position in ISER1

//SYSCNG
#define SYSCFG_BASE_ADDR 		0x40013800
#define SYSCFG_EXTICR3_OFFSET	0x10			//0001 for PB10 in [11:8]

//EXTI
#define EXTI_BASE_ADDR		0x40013C00
#define	EXTI_IMR_OFFSET		0x00		//bit 10 for PB10
#define	EXTI_RTSR_OFFSET	0x08		//Bit 10 for rising edge detection on PB10
#define	EXTI_PR_OFFSET		0x14		//Bit 10 Pending Register PB10


//USART
#define USART2_BASE_ADDR   	0x40004400
#define USART_SR_OFFSET		0x00 // 8bit for data transfer
#define USART_DR_OFFSET		0x04 // 8bit for data transfer
#define	USART_BRR_OFFSET	0x08 // __Baudrate__ --> 0x8B for desired BR [15:4]_Mantissa - [3:0] Fraction
#define USART_CR1_OFFSET	0x0C // Control register


#endif /* STM32F46RE_H_ */
