/*
 * File:    i2c.c
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This file implements a bare-metal I2C1 driver for an STM32
 *   microcontroller, handling GPIO initialisation (PB8/PB9 in
 *   open-drain alternate function mode) and low-level I2C operations
 *   including start/stop conditions, byte read/write, and
 *   single-register read/write transactions.
 */
#include "i2c.h"

void init_i2c_gpio(void){
	// MODER
	// Clear bits
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	&= 	~(
																		 	(3u << 2*8)		// AF for PB8
																		|	(3u << 2*9)		// AF for PB9
																		);
	// Set bits
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	|= 	(
																		 	(2u << 2*8)		// AF (10) for PB8
																		|	(2u << 2*9)		// AF for PB9
																		);
	// AFRL
	// Clear bits
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_AFRH_OFFSET)) 	&= 	~(
																			(15u << 4*0)	// AF for PB8
																		|	(15u << 4*1));	// AF for PB9
	// Set bits
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_AFRH_OFFSET)) 	|= 	(
																			(4u << 4*0)		// AF 4 for I2C1 for PB8
																		|	(4u << 4*1));	// AF 4 for PB9
	// PUPDR
	// Clear bits
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_PUPDR_OFFSET)) 	&= 	~(
																			(3u << 2*8)
																		|	(3u << 2*9));
	// Set bits
	// EXTERNAL 4.7 Kohm used
	//	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_PUPDR_OFFSET)) 	|= 	(
	//																			(1u << 2*8)		// 01 for pull-up
	//																		|	(1u << 2*9));	// 01 for pull-up
	//OTYPER
	// Set bits
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_OTYPER_OFFSET)) 	|= 	(
																				(1u << 8)		// Open-drain
																			|	(1u << 9));		// open-drain
}
void i2c_init(void){
	I2C1_CR1 |= I2C_CR1_SWRST;   // assert reset
	I2C1_CR1 &= ~I2C_CR1_SWRST;  // release reset

	//Disable beofre configuring
	I2C1_CR1	&= ~(I2C_CR1_PE);
	//CR1
		//Clear bits
	I2C1_CR1	&= ~(
						I2C_CR1_SWRST
					|	I2C_CR1_ACK
					|	I2C_CR1_START
					|	I2C_CR1_STOP
					);
		//Set bits
			// N/A

	//CR2
		//Clear bits
	I2C1_CR2	&= ~(
							I2C_CR2_FREQ_MASK

						);
		//Set bits
	I2C1_CR2	|= (
							0x10 	// 16 MHz (Same as APB)
						);
	//CCR
	I2C1_CCR 	&=	~(
						I2C_CCR_FS 		// Standard mode
					|	I2C_CCR_MASK
					); 	// reset

	I2C1_CCR 	|=	(0x50 << 0);	//CCR = f_APB1 / (2 * f_I2C) = 80


	//TRISE
	I2C1_TRISE &= ~(0x3F);	// reset
	I2C1_TRISE |= (0x11);	// TRISE = (t_rise_max × f_APB1) + 1 = 17
	//Enable after configuring
	I2C1_CR1	|= (I2C_CR1_PE);

}
void i2c_start(){
	while(I2C1_SR2 & I2C_SR2_BUSY); // wait until bus is free
	I2C1_CR1 |= I2C_CR1_START;		// generate Start condition
	while(!(I2C1_SR1 & I2C_SR1_SB));
}
void i2c_restart(){
	// while(I2C1_SR2 & I2C_SR2_BUSY); // No check for busy --> we are in control already
	I2C1_CR1 |= I2C_CR1_START;		// generate Start condition
	while(!(I2C1_SR1 & I2C_SR1_SB));
}
void i2c_stop(){
	I2C1_CR1 |= I2C_CR1_STOP;				// 	Generate Stop condition
}
void i2c_write_byte(uint8_t data){
	while(!(I2C1_SR1 & I2C_SR1_TXE)); 	// Wait until Transfer Buffer is Empty
	I2C1_DR = data;
	while(!(I2C1_SR1 & I2C_SR1_BTF)); 	// Wait until Byte Transfer is complete
}
uint8_t i2c_read_byte(void){
	I2C1_CR1 &= ~I2C_CR1_ACK; 			// Clear ACK before reading
	while(!(I2C1_SR1 & I2C_SR1_RXNE));	// Wait for RXNE
	I2C1_CR1	|=	I2C_CR1_STOP;		// Generate Stop -- BEFORE reading DR to be on time
	return I2C1_DR;
}
void i2c_write_addr(uint8_t addr){
	while(!(I2C1_SR1 & I2C_SR1_SB));	// Wait untill start bit is set
	I2C1_DR = addr;
	while(!(I2C1_SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)));  // wait for address sent
	(void)I2C1_SR1;                      // read SR1
	(void)I2C1_SR2;                      // read SR2 — clears ADDR
}
void i2c_write_register(uint8_t dev_addr, uint8_t reg, uint8_t data){
	i2c_start();
	i2c_write_addr(dev_addr << 1 | 0); 	// write, caller passes 7 bit addr
	i2c_write_byte(reg);
	i2c_write_byte(data);
	i2c_stop();
}
uint8_t i2c_read_register(uint8_t dev_addr, uint8_t reg){
	uint8_t data;
	i2c_restart();
	i2c_write_addr(dev_addr << 1 | 0);	// write mode first, to call the correct register
	i2c_write_byte(reg);
	i2c_restart();						// restart before switching modes
	i2c_write_addr(dev_addr << 1 | 1); 	// read, caller passes 7 bit addr
	data = i2c_read_byte();
	i2c_stop();
	return data;
}
