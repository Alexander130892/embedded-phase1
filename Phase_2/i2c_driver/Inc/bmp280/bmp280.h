/*
 * bmp280.h
 *	public
 *  Created on: May 5, 2026
 *      Author: alexandervindelinckx
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
