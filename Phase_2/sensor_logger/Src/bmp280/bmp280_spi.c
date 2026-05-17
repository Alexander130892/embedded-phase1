/*
 * bmp280.c
 *
 *  Created on: May 5, 2026
 *      Author: alexandervindelinckx
 */


#include "stm32f446re.h"
#include "bmp280.h"
#include "bmp280_internal.h"
#include "status.h"
#include "spi.h"
#include "string.h"

static void     cs_low(void);
static void     cs_high(void);
static status_t bmp280_spi_read_register(uint8_t addr, uint8_t *data);
static status_t bmp280_spi_write_register(uint8_t addr, uint8_t data);
static status_t bmp280_spi_comp_params(void);

//SPI
status_t bmp280_spi_init(void){
	// spi_init() to be called by user
	status_t status;
	status = bmp280_spi_write_register(BMP280_CTRL_MEAS, BMP280_CTRL_MEAS_VALUE); // Oversampling x 8 for P and T, Normal Power mode
	if(status){
		return status;
	}
	return bmp280_spi_comp_params();
}
status_t bmp280_spi_gpio_init(){
	// Enable clock
	*((volatile uint32_t *)(RCC_BASE_ADDR + RCC_AHB1ENR_OFFSET)) 	|= 	(1u << 1);		//  Bit 1 for GPIOB
	// MODER
	// Clear bits
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	&= 	~(
																			(3u << 2*BMP280_CS_PIN)		// AF for PB6
																		);
	// Set bits
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_MODER_OFFSET)) 	|= 	(
																			(1u << 2*BMP280_CS_PIN)		// GPIO for PB6
																		);
	//IDLE HIGH
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) 	|= 	(
																			(1u << BMP280_CS_PIN)		// GPIO for PB6
																		);
	return STATUS_OK;
}
status_t bmp280_spi_read_who_am_i(uint8_t * data){
	return bmp280_spi_read_register(BMP280_WHOAMI, data);
}
static void cs_low(){
	// Set CS low
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) &= ~(1u << BMP280_CS_PIN);
}
static void cs_high(){
	// Set CS High
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) |= (1u << BMP280_CS_PIN);
}
static status_t bmp280_spi_read_register(uint8_t addr, uint8_t * data){
	// Set CS low
	cs_low();
	// Send addr to bmp280 for data transfer
	status_t status = STATUS_OK;
	uint8_t dummy;
	status = spi_transfer(addr | BMP280_SPI_READ, &dummy);
	if(status){
		cs_high();
		return status;
	}
	// read data from SDO
	status = spi_transfer(0x00, data);
	if(status){
		cs_high();
		return status;
	}
	//Wait until BSY status cleared before reseting CS to not clip
	uint32_t timeout=0;
	while( *((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET)) & (1u << 7) ){
		timeout++;
		if(timeout > MAX_TIMEOUT) {
			cs_high();
			return SPI_TIMEOUT;
		}
	}
	// Re-Set CS High
	cs_high();
	return status;
}
static status_t	bmp280_spi_write_register(uint8_t addr, uint8_t data){
	// Set CS low
	cs_low();
	// Send pair (addr + command)
	status_t status = STATUS_OK;
	uint8_t dummy;
	status = spi_transfer(addr & BMP280_SPI_WRITE, &dummy);	// clip bit 7 from addr (bit 7 = 0 --> write)
	if(status){
		cs_high();
		return status;
	}
	status = spi_transfer(data, &dummy);
	if(status){
		cs_high();
		return status;
	}
	//Wait until BSY status cleared before reseting CS to not clip
	uint32_t timeout=0;
	while( *((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET)) & (1u << 7) ){
		timeout++;
		if(timeout > MAX_TIMEOUT) {
			cs_high();
			return SPI_TIMEOUT;
		}
	}
	// Re-Set CS High
	cs_high();
	return status;
}
static status_t bmp280_spi_comp_params(void){
	uint8_t buf[24];
	status_t status;
	cs_low();
	status = spi_read_burst(BMP280_CALIB_START, buf, 24);
	cs_high();
	if(status){
		return status;
	}
	memcpy(&comp_params, buf, sizeof(comp_param_t));
	return STATUS_OK;
}
status_t bmp280_spi_read_temp(int32_t *temp){
	uint8_t buffer[3];
	uint32_t adc_T=0;
	status_t status;
	cs_low();
	status = spi_read_burst(BMP280_TEMP_MSB, buffer, 3);
	cs_high();
	if(status){
		return status;
	}
	adc_T = ((uint32_t)buffer[0] << 12 | (uint32_t)buffer[1] << 4 | (uint32_t)(buffer[2] >> 4));
	*temp = bmp280_compensate_temp(adc_T);
	return STATUS_OK;
}
status_t bmp280_spi_read_pressure(int32_t *pressure){
	uint8_t buffer[6];
	//T is needed for accurate P-measurement
	uint32_t adc_T=0;
	status_t status;
	cs_low();
	status = spi_read_burst(BMP280_PRESS_MSB, buffer, 6);
	cs_high();
	if(status){
		return status;
	}
	adc_T = ((uint32_t)buffer[3] << 12 | (uint32_t)buffer[4] << 4 | (uint32_t)(buffer[5] >> 4));
	bmp280_compensate_temp(adc_T);
	// P-measurement
	uint32_t adc_P=0;
	adc_P = ((uint32_t)buffer[0] << 12 | (uint32_t)buffer[1] << 4 | (uint32_t)(buffer[2] >> 4));
	*pressure = bmp280_compensate_pressure(adc_P, adc_T);
	return STATUS_OK;
}


