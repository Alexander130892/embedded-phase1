/*
 * File:    bmp280.c
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This file implements low-level register read/write communication
 *   with a BMP280 pressure/temperature sensor over SPI (using manual
 *   GPIO chip-select toggling on GPIOB pin 6), and includes a stub for
 *   reading the sensor's WHO_AM_I register via I2C as an alternative
 *   interface.
 */
#include "stm32f446re.h"
#include "bmp280.h"
#include "spi.h"
#include "i2c.h"

uint8_t bmp280_read_register(uint8_t addr){
	uint8_t data;
	// Set CS low
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) &= ~(1u << 6);
	// Send addr to bmp280 for data transfer
	spi_transfer(addr | 0x80);
	// read data from SDO
	data = spi_transfer(0x00);
	//Wait until BSY status cleared before reseting CS to not clip
	while( *((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET)) & (1u << 7) );
	// Re-Set CS High
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) |=  (1u << 6);
	return data;
}
void 	bmp280_write_register(uint8_t addr, uint8_t data){
	// Set CS low
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) &= ~(1u << 6);
	// Send pair (addr + command)
	spi_transfer(addr & ~(0x80));	// clip bit 7 from addr (bit 7 = 0 --> write)
	spi_transfer(data);
	//Wait until BSY status cleared before reseting CS to not clip
	while( *((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET)) & (1u << 7) );
	// Re-Set CS High
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) |=  (1u << 6);
}
uint8_t bmp280_read_who_am_i(){
	return bmp280_read_register(0xD0);
}
uint8_t bmp280_i2c_read_who_am_i(){
	return i2c_read_register(0x76, 0xD0);
//	i2c_start();
//	// Send address + write
//	i2c_write_addr(0x76 << 1 | 0);
//
//	// Send register 0xD0
//	i2c_write_byte(0xD0);
//
//	// REPEATED START
//	i2c_restart();
//
//	// Send address + read
//	i2c_write_addr(0x76 << 1 | 1);
//
//	// Read byte
//	return i2c_read_byte();
}
