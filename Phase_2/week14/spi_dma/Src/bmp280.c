/*
 * File:    bmp280.c
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This file implements a driver for the BMP280 pressure and
 *   temperature sensor over SPI on an STM32F446RE microcontroller,
 *   providing functions to read/write registers (including DMA-based
 *   burst reads), load factory calibration compensation parameters,
 *   and convert raw ADC readings into compensated temperature and
 *   pressure values using the BMP280's datasheet compensation
 *   formulas.
 */
#include "stm32f446re.h"
#include "bmp280.h"
#include "spi.h"

static int32_t t_fine;

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

void bmp280_read_burst(uint8_t addr, uint32_t n_data, uint8_t* memory_addr){
	spi_dma_read(addr, n_data, memory_addr);
	while(dma_s0_flag == 0){};
	dma_s0_flag = 0;
}
void bmp280_comp_params(comp_param_t *cp){
	cp->dig_T1 = (bmp280_read_register(0x89)<<8 | bmp280_read_register(0x88) );
	cp->dig_T2 = (bmp280_read_register(0x8B)<<8 | bmp280_read_register(0x8A) );
	cp->dig_T3 = (bmp280_read_register(0x8D)<<8 | bmp280_read_register(0x8C) );
	cp->dig_P1 = (bmp280_read_register(0x8F)<<8 | bmp280_read_register(0x8E) );
	cp->dig_P2 = (bmp280_read_register(0x91)<<8 | bmp280_read_register(0x90) );
	cp->dig_P3 = (bmp280_read_register(0x93)<<8 | bmp280_read_register(0x92) );
	cp->dig_P4 = (bmp280_read_register(0x95)<<8 | bmp280_read_register(0x94) );
	cp->dig_P5 = (bmp280_read_register(0x97)<<8 | bmp280_read_register(0x96) );
	cp->dig_P6 = (bmp280_read_register(0x99)<<8 | bmp280_read_register(0x98) );
	cp->dig_P7 = (bmp280_read_register(0x9B)<<8 | bmp280_read_register(0x9A) );
	cp->dig_P8 = (bmp280_read_register(0x9D)<<8 | bmp280_read_register(0x9C) );
	cp->dig_P9 = (bmp280_read_register(0x9F)<<8 | bmp280_read_register(0x9E) );
}
void bmp280_ctrl_meas(){
	bmp280_write_register(0xF4, 0x93); // Oversampling x 8 for P and T, Normal Power mode
}

int32_t bmp280_read_temp(comp_param_t *cp){
	uint8_t buffer[4];
	uint32_t adc_T=0;
	bmp280_read_burst(0xFA, 4, buffer);
	adc_T = ((uint32_t)buffer[1] << 12 | (uint32_t)buffer[2] << 4 | (uint32_t)(buffer[3] >> 4));

	//Compensation formula
	int32_t var1, var2, T;
	var1 = ((((adc_T >> 3) - ((int32_t)cp->dig_T1 << 1))) * ((int32_t)cp->dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((int32_t)cp->dig_T1)) * ((adc_T >> 4) - ((int32_t)cp->dig_T1))) >> 12) *	((int32_t)cp->dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}

int32_t bmp280_read_pressure(comp_param_t *cp){
	uint8_t buffer[4];
	uint32_t adc_P=0;
	bmp280_read_burst(0xF7, 4, buffer);
	adc_P = ((uint32_t)buffer[1] << 12 | (uint32_t)buffer[2] << 4 | (uint32_t)(buffer[3] >> 4));

	//Compensation formula
	int32_t var1, var2;
	uint32_t p;
	var1 = (((int32_t)t_fine)>>1) - 64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)cp->dig_P6);
	var2 = var2 + ((var1*((int32_t)cp->dig_P5))<<1);
	var2 = (var2>>2)+(((int32_t)cp->dig_P4)<<16);
	var1 = (((cp->dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)cp->dig_P2) * var1)>>1))>>18;
	var1 =((((32768+var1))*((int32_t)cp->dig_P1))>>15);
	if (var1 == 0)
	{
	return 0; // avoid exception caused by division by zero
	}
	p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;
	if (p < 0x80000000)
	{
	p = (p << 1) / ((uint32_t)var1);
	}
	else
	{
	p = (p / (uint32_t)var1) * 2;
	}
	var1 = (((int32_t)cp->dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
	var2 = (((int32_t)(p>>2)) * ((int32_t)cp->dig_P8))>>13;
	p = (uint32_t)((int32_t)p + ((var1 + var2 + cp->dig_P7) >> 4));
	return p;
}


