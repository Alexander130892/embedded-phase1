/*
 * File:    i2c.c
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This file implements a bare-metal I2C1 driver for an STM32
 *   microcontroller, handling peripheral and GPIO initialization
 *   (PB8/PB9 in open-drain alternate function mode) as well as
 *   low-level bus operations including start/restart/stop condition
 *   generation and single-byte read/write with polling and timeout
 *   protection.
 */
#include "i2c.h"


status_t i2c_init_gpio(void){
	// Enable clock
	*((volatile uint32_t *)(RCC_BASE_ADDR + RCC_AHB1ENR_OFFSET)) 	|= 	(1u << 1);		//  Bit 1 for GPIOB
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
	return STATUS_OK;
}
status_t i2c_init(void){
	//Enable clock
	*((volatile uint32_t *)(RCC_BASE_ADDR + RCC_APB1ENR_OFFSET))	|= 	(1u << 21);		// Bit 21 for I2C1
	//Force reset
	I2C1_CR1 |= I2C_CR1_SWRST;   // assert reset
	I2C1_CR1 &= ~I2C_CR1_SWRST;  // release reset

	//Disable before configuring
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
						I2C_FREQ_16MHZ 	// 16 MHz (Same as APB)
						);
	//CCR
	I2C1_CCR 	&=	~(
						I2C_CCR_FS 		// Standard mode
					|	I2C_CCR_MASK
					); 	// reset

	I2C1_CCR 	|=	(I2C_CCR_100KHZ);	//CCR = f_APB1 / (2 * f_I2C) = 80


	//TRISE
	I2C1_TRISE &= ~(I2C_TRISE_MASK);	// reset
	I2C1_TRISE |= (I2C_TRISE_100KHZ);	// TRISE = (t_rise_max × f_APB1) + 1 = 17
	//Enable after configuring
	I2C1_CR1	|= (I2C_CR1_PE);
	return STATUS_OK;
}
status_t i2c_start(void){
	uint32_t timeout=0;
	status_t status=STATUS_OK;
	if(I2C1_SR2 & I2C_SR2_BUSY){
		status=i2c_bus_reset();
	}
	if(status){
		return status;
	}
	I2C1_CR1 |= I2C_CR1_START;			// generate Start condition
	while(!(I2C1_SR1 & I2C_SR1_SB)){
		timeout++;
		if(timeout > MAX_TIMEOUT) return I2C_TIMEOUT;
	}
	return STATUS_OK;
}
status_t i2c_restart(void){
	uint32_t timeout=0;
	// while(I2C1_SR2 & I2C_SR2_BUSY); // No check for busy --> we are in control already
	I2C1_CR1 |= I2C_CR1_START;		// generate Start condition
	while(!(I2C1_SR1 & I2C_SR1_SB)){
		timeout++;
		if(timeout > MAX_TIMEOUT) return I2C_TIMEOUT;
	}
	return STATUS_OK;
}
status_t i2c_stop(void){
	I2C1_CR1 |= I2C_CR1_STOP;				// 	Generate Stop condition
	return STATUS_OK;
}
status_t i2c_write_byte(uint8_t data){
	uint32_t timeout=0;
	while(!(I2C1_SR1 & I2C_SR1_TXE)){// Wait until Transfer Buffer is Empty
		timeout++;
		if(timeout > MAX_TIMEOUT) return I2C_TIMEOUT;
	}
	I2C1_DR = data;
	timeout=0;
	while(!(I2C1_SR1 & I2C_SR1_BTF)){ 	// Wait until Byte Transfer is complete
		timeout++;
		if(timeout > MAX_TIMEOUT) return I2C_TIMEOUT;
	}
	return STATUS_OK;
}
status_t i2c_read_byte(uint8_t *data){
	uint32_t timeout=0;
	I2C1_CR1 &= ~I2C_CR1_ACK; 			// Clear ACK before reading
	while(!(I2C1_SR1 & I2C_SR1_RXNE)){	// Wait for RXNE
		timeout++;
		if(timeout > MAX_TIMEOUT) return I2C_TIMEOUT;
	}
	I2C1_CR1	|=	I2C_CR1_STOP;		// Generate Stop -- BEFORE reading DR to be on time
	*data = I2C1_DR;
	return STATUS_OK;
}
status_t i2c_write_addr(uint8_t addr){
	uint32_t timeout=0;
	while(!(I2C1_SR1 & I2C_SR1_SB)){	// Wait until start bit is set
		timeout++;
		if(timeout > MAX_TIMEOUT) return I2C_TIMEOUT;
	}
	I2C1_DR = addr;
	timeout=0;
	while(!(I2C1_SR1 & (I2C_SR1_ADDR | I2C_SR1_AF))){  // wait for address sent
		timeout++;
		if(timeout > MAX_TIMEOUT) return I2C_TIMEOUT;
	}
	if(I2C1_SR1 & I2C_SR1_AF){
			I2C1_SR1 &= ~I2C_SR1_AF; // reset before returning
			return I2C_AF; // Acknowledge Failure
	}
	(void)I2C1_SR1;                      // read SR1
	(void)I2C1_SR2;                      // read SR2 — clears ADDR
	return STATUS_OK;
}
status_t i2c_write_register(uint8_t dev_addr, uint8_t reg, uint8_t data){
	status_t status=STATUS_OK;
	status=i2c_start();
	if(status){
		return status;
	}
	status = i2c_write_addr(dev_addr << 1 | 0);
	if(status){
		return status;
	}
	status = i2c_write_byte(reg);
	if(status){
		return status;
	}
	status = i2c_write_byte(data);
	if(status){
		return status;
	}
	return i2c_stop();
}
status_t i2c_read_register(uint8_t dev_addr, uint8_t reg, uint8_t *data){
	status_t status=STATUS_OK;
	status = i2c_start();
	if(status){
		return status;
	}
	status = i2c_write_addr(dev_addr << 1 | 0);
	if(status){
		return status;
	}
	// write mode first, to call the correct register
	status = i2c_write_byte(reg);
	if(status){
		return status;
	}
	status = i2c_restart();			// restart before switching modes
	if(status){
		return status;
	}
	status = i2c_write_addr(dev_addr << 1 | 1);
	if(status){
		return status;
	}
	// read, caller passes 7 bit addr
	status = i2c_read_byte(data);
	if(status){
			return status;
	}
	return i2c_stop();
}
status_t i2c_read_burst(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len){
	if (len == 1){
		return i2c_read_register(addr, reg, buf);
	}else{
		status_t status=STATUS_OK;
		status = i2c_start();
		if(status){
			return status;
		}
		status = i2c_write_addr(addr << 1 | 0);
		if(status){
			return status;
		}
		// write mode first, to call the correct register
		status=i2c_write_byte(reg);
		if(status){
			return status;
		}
		status=i2c_restart();
		if(status){
			return status;
		}
		if (len == 2){
			// Manual address phase for read — need POS/ACK set before ADDR clears
			I2C1_DR = addr << 1 | 1;
			uint32_t timeout=0;
			while(!(I2C1_SR1 & (I2C_SR1_ADDR | I2C_SR1_AF))){  // wait for ADDR
				timeout++;
				if(timeout > MAX_TIMEOUT) return I2C_TIMEOUT;
			}
			if(I2C1_SR1 & I2C_SR1_AF){
				I2C1_SR1 &= ~I2C_SR1_AF; // reset before returning
				return I2C_AF; // Acknowledge Failure
			}
			I2C1_CR1 |= I2C_CR1_POS;        //
			I2C1_CR1 &= ~I2C_CR1_ACK;       //
			(void)I2C1_SR1;                  // clear ADDR
			(void)I2C1_SR2;
			//		Wait BTF                  both bytes received, SCL stretched, bus paused
			timeout=0;
			while(!(I2C1_SR1 & I2C_SR1_BTF)){
				timeout++;
				if(timeout > MAX_TIMEOUT) return I2C_TIMEOUT;
			}
			//	Set STOP
			status = i2c_stop();
			if(status){
				return status;
			}
			//	Read DR (byte 1)
			buf[0] = (uint8_t) I2C1_DR;
			//	Read DR (byte 2)
			buf[1] = (uint8_t) I2C1_DR;
			//	Set POS=0 					Clear POS before returning
			I2C1_CR1 &= ~I2C_CR1_POS;
			return STATUS_OK;
		} else{
			status = i2c_write_addr(addr << 1 | 1);
			if(status){
				return status;
			}
			// First N-2 bytes
			I2C1_CR1 |= I2C_CR1_ACK;
			for(uint8_t byte = 0; byte < len -2 ; byte++){
				uint32_t timeout=0;
				while(!(I2C1_SR1 & I2C_SR1_RXNE)){	// Wait for RXNE
					timeout++;
					if(timeout > MAX_TIMEOUT) return I2C_TIMEOUT;
				}
				buf[byte] = I2C1_DR;
			}
			// Last 2 bytes
			//	Wait BTF                  both bytes received, SCL stretched, bus paused
			uint32_t timeout=0;
			while(!(I2C1_SR1 & I2C_SR1_BTF)){
				timeout++;
				if(timeout > MAX_TIMEOUT) return I2C_TIMEOUT;
			}
			// Set ACK low
			I2C1_CR1 &= ~I2C_CR1_ACK;
			//	Read DR (byte N-1)
			buf[len-2] = (uint8_t) I2C1_DR;
			//	Wait BTF                  both bytes received, SCL stretched, bus paused
			timeout=0;
			while(!(I2C1_SR1 & I2C_SR1_BTF)){
				timeout++;
				if(timeout > MAX_TIMEOUT) return I2C_TIMEOUT;
			}
			// Set STOP High
			I2C1_CR1 |= I2C_CR1_STOP;
			//	Read DR (byte N-1)
			buf[len-1] = (uint8_t) I2C1_DR;
			// Set ACK High before returning (reseting CR register to correct state)
			I2C1_CR1 |= I2C_CR1_ACK;
			return STATUS_OK;
		}
	}
}
status_t i2c_bus_reset(void){
//	SWRST — reset the peripheral
	I2C1_CR1 |= I2C_CR1_SWRST;
	I2C1_CR1 &= ~I2C_CR1_SWRST;
//	Reconfigure SCL/SDA as GPIO output temporarily
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	&= 	~(
																			 	(3u << 2*8)		// AF for PB8
																			|	(3u << 2*9)		// AF for PB9
																			);
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	|= 	(
																				 	(1u << 2*8)		// 01 output for PB8
																				|	(1u << 2*9)		// output for PB9
																				);
//	Toggle SCL up to 9 times until SDA goes high
	uint8_t count=0;
	while(!(*(volatile uint32_t *)(GPIOB_BASE_ADDR+GPIOx_IDR_OFFSET) & (1u<<8))){
		if(count == I2C_BUS_RECOVERY_CLOCKS){
			return i2c_init();
		}
		*(volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET) &= ~(1u<<9);
		for(volatile uint32_t delay = 0 ; delay < I2C_BUS_RECOVERY_DELAY ; delay++); // small delay
		*(volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET) |= (1u<<9);
		count ++;
	}
//	Generate a STOP condition manually
	*(volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET) |= (1u<<9);	//SCL high first
	*(volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET) |= (1u<<8); // SDA after --> Manual STOP condition
//	Re-initialise the peripheral
	return i2c_init();
}
