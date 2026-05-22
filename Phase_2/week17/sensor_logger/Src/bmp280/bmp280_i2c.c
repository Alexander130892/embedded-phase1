/*
 * File:    bmp280_i2c.c
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This file implements the I2C communication layer for the BMP280
 *   pressure and temperature sensor on an STM32F446RE microcontroller,
 *   providing functions to initialise the sensor, read its device ID,
 *   and retrieve compensated temperature and pressure readings.
 *   Notably, the pressure reading function also reads and compensates
 *   temperature internally, since an accurate temperature value is
 *   required to correctly compute the pressure compensation.
 */
#include <string.h>
#include "stm32f446re.h"
#include "bmp280.h"
#include "bmp280_internal.h"
#include "i2c.h"


//I2C
status_t bmp280_i2c_read_who_am_i(uint8_t* data){
	return i2c_read_register(BMP280_ADDR_SDO_LOW, BMP280_WHOAMI, data);
}
static status_t bmp280_i2c_comp_params(uint8_t addr){
	uint8_t buf[24];
	status_t status;
	status = i2c_read_burst(addr, BMP280_CALIB_START, buf, 24);
	if(status){
		return status;
	}else {
		memcpy(&comp_params, buf, sizeof(comp_param_t));
		return STATUS_OK;
	}
}
status_t bmp280_i2c_init(uint8_t addr){
	// i2c_init() to be called by user
	status_t status;
	status = i2c_write_register(addr, BMP280_CTRL_MEAS, 0x93); // Oversampling x 8 for P and T, Normal Power mode
	if(status){
			return status;
	}else {
		return bmp280_i2c_comp_params(addr);
	}
}
status_t bmp280_i2c_read_temp(uint8_t addr, int32_t *temp){
	uint8_t buffer[3];
	uint32_t adc_T=0;
	status_t status;
	status = i2c_read_burst(addr, BMP280_TEMP_MSB, buffer, 3);
	if(status){
				return status;
	}else {
		adc_T = ((uint32_t)buffer[0] << 12 | (uint32_t)buffer[1] << 4 | (uint32_t)(buffer[2] >> 4));
		*temp = bmp280_compensate_temp(adc_T);
		return STATUS_OK;
	}
}
status_t bmp280_i2c_read_pressure(uint8_t addr, int32_t *pressure){
	uint8_t buffer[6];
	//T is needed for accurate P-measurement
	uint32_t adc_T=0;
	status_t status;
	status = i2c_read_burst(addr, BMP280_PRESS_MSB, buffer, 6);
	if(status){
					return status;
	}else {
		adc_T = ((uint32_t)buffer[3] << 12 | (uint32_t)buffer[4] << 4 | (uint32_t)(buffer[5] >> 4));
		bmp280_compensate_temp(adc_T);
		// P-measurement
		uint32_t adc_P=0;
		adc_P = ((uint32_t)buffer[0] << 12 | (uint32_t)buffer[1] << 4 | (uint32_t)(buffer[2] >> 4));
		*pressure = bmp280_compensate_pressure(adc_P, adc_T);
		return STATUS_OK;
	}
}
