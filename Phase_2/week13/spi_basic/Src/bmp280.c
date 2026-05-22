/*
 * File:    bmp280.c
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This file implements low-level SPI communication functions for a
 *   BMP280 pressure/temperature sensor on an STM32F446RE
 *   microcontroller, providing register read and write operations via
 *   manual chip-select (PB6) toggling. It includes a helper to read
 *   the WHO_AM_I identification register (0xD0) to verify sensor
 *   presence.
 */
#include "stm32f446re.h"
#include "bmp280.h"
#include "spi.h"

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
