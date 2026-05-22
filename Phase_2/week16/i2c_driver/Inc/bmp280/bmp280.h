/*
 * File:    bmp280.h
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This header file defines the interface for a BMP280 barometric
 *   pressure and temperature sensor driver, supporting both SPI and
 *   I2C communication protocols. It declares functions for
 *   initializing the sensor, reading its device ID, and retrieving
 *   compensated temperature and pressure values, along with the two
 *   possible I2C addresses determined by the SDO pin state.
 */
#ifndef BMP280_H_
#define BMP280_H_

#include <stdint.h>

#define BMP280_ADDR_SDO_LOW  0x76
#define BMP280_ADDR_SDO_HIGH 0x77

//SPI
uint8_t bmp280_spi_read_who_am_i();
void 	bmp280_spi_config();
void 	bmp280_spi_ctrl_meas();
int32_t bmp280_spi_read_temp();
int32_t bmp280_spi_read_pressure();

//I2C
void 	bmp280_i2c_read_who_am_i(uint8_t* data);
void 	bmp280_i2c_init(uint8_t addr);
int32_t bmp280_i2c_read_temp(uint8_t addr);
int32_t bmp280_i2c_read_pressure(uint8_t addr);

#endif /* BMP280_H_ */
