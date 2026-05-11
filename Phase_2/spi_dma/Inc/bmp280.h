/*
 * File:    bmp280.h
 * Author:  Alexander130892
 * Date:    10-5-2026
 *
 * Description:
 *   This header file defines the interface for communicating with a
 *   BMP280 environmental sensor, providing functions to read
 *   temperature and pressure measurements along with compensation
 *   parameters needed for accurate sensor data conversion. It includes
 *   low-level register access functions and higher-level functions for
 *   sensor configuration and calibrated data retrieval.
 */
#ifndef BMP280_H_
#define BMP280_H_

#include <stdint.h>

typedef struct {
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;
}comp_param_t; //Compensation parameters



uint8_t bmp280_read_register(uint8_t addr);
void 	bmp280_write_register(uint8_t addr, uint8_t data);
uint8_t bmp280_read_who_am_i();
void bmp280_read_burst(uint8_t addr, uint32_t n_data, uint8_t* memory_addr);
void bmp280_ctrl_meas();
void bmp280_config();
void bmp280_comp_params(comp_param_t *cp);
void bmp280_ctrl_meas();
int32_t bmp280_read_temp(comp_param_t *cp);
int32_t bmp280_read_pressure(comp_param_t *cp);

#endif /* BMP280_H_ */
