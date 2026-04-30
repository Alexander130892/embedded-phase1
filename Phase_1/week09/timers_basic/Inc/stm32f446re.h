/*
 * stm32f46re.h
 *
 *  Created on: Apr 18, 2026
 *      Author: alexandervindelinckx
 */

#ifndef STM32F46RE_H_
#define STM32F46RE_H_
// AHB1
#define GPIOA_BASE_ADDR    0x40020000

// RCC
#define RCC_BASE_ADDR      0x40023800

//for Peripheral clocks
#define RCC_APB1ENR_OFFSET 	0x40	// Timer
#define RCC_AHB1ENR_OFFSET 	0x30	// GPIOA

// TIM2 registers:
#define TIM2_BASE_ADDR 		0x40000000
#define TIMx_CR1_OFFSET 	0x00
	// Bit 4 DIR = default up = 0
	// Bit 0 CEN = counter enable
#define TIMx_SR_OFFSET		0x10
	// Bit 0, update interrupt flag UIF
#define TIMx_EGR_OFFSET		0x14
	//Bit 0 = Update Generation
#define TIMx_CNT_OFFSET		0x24 		// (16 bit) counter
#define TIMx_PSC_OFFSET		0x28 		// 16 bit Prescaler
#define TIMx_ARR_OFFSET 	0x2C 		// Auto reload value 16 bit


//for LED D2 = PA10
#define GPIOA_MODER_OFFSET	0x00 	// write 01 for correct PA pin TBC
#define GPIOA_ODR_OFFSET	0x14 	// to correct pin TBC



#endif /* STM32F46RE_H_ */
