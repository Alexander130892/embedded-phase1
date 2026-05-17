/*
 * File:    bmp280_i2c.c
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This file implements the I2C communication interface for the
 *   BMP280 pressure/temperature sensor on an STM32F446RE, providing
 *   functions to initialize the sensor, read the device ID, and
 *   retrieve compensated temperature and pressure readings. Notably,
 *   the pressure reading function simultaneously reads both pressure
 *   and temperature ADC values in a single burst, as temperature data
 *   is required for accurate pressure compensation.
 */
#include <string.h>
#include "stm32f446re.h"
#include "bmp280.h"
#include "bmp280_internal.h"
#include "i2c.h"


//I2C
void bmp280_i2c_read_who_am_i(uint8_t* data){
	i2c_read_register(BMP280_ADDR_SDO_LOW, WHOAMI, data);
}
static void bmp280_i2c_comp_params(uint8_t addr){
	uint8_t buf[24];
	i2c_read_burst(addr, CALIB_START, buf, 24);
	memcpy(&comp_params, buf, sizeof(comp_param_t));
}
void bmp280_i2c_init(uint8_t addr){
	// i2c_init() to be called by user
	i2c_write_register(addr, CTRL_MEAS, 0x93); // Oversampling x 8 for P and T, Normal Power mode
	bmp280_i2c_comp_params(addr);
}
int32_t bmp280_i2c_read_temp(uint8_t addr){
	uint8_t buffer[3];
	uint32_t adc_T=0;
	i2c_read_burst(addr, TEMP_MSB, buffer, 3);
	adc_T = ((uint32_t)buffer[0] << 12 | (uint32_t)buffer[1] << 4 | (uint32_t)(buffer[2] >> 4));
	return bmp280_compensate_temp(adc_T);
}
int32_t bmp280_i2c_read_pressure(uint8_t addr){
	uint8_t buffer[6];
	//T is needed for accurate P-measurement
	uint32_t adc_T=0;
	i2c_read_burst(addr, PRESS_MSB, buffer, 6);
	adc_T = ((uint32_t)buffer[3] << 12 | (uint32_t)buffer[4] << 4 | (uint32_t)(buffer[5] >> 4));
	// P-measurement
	uint32_t adc_P=0;
	adc_P = ((uint32_t)buffer[0] << 12 | (uint32_t)buffer[1] << 4 | (uint32_t)(buffer[2] >> 4));

	return bmp280_compensate_pressure(adc_P, adc_T);
}
