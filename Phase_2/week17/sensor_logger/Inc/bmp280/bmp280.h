/*
 * File:    bmp280.h
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This header file defines the interface for a BMP280 barometric
 *   pressure and temperature sensor driver, declaring initialization
 *   and data-reading functions for both SPI and I2C communication
 *   protocols. It exposes functions to read the device ID,
 *   temperature, and pressure, along with I2C address constants and
 *   the SPI chip-select pin definition.
 */
#ifndef BMP280_H_
#define BMP280_H_

#include <stdint.h>
#include "status.h"

#define BMP280_ADDR_SDO_LOW  0x76
#define BMP280_ADDR_SDO_HIGH 0x77

#define BMP280_CS_PIN    6

//SPI
status_t bmp280_spi_init(void);
status_t bmp280_spi_gpio_init(void);
status_t bmp280_spi_read_who_am_i(uint8_t * data);
status_t bmp280_spi_read_temp(int32_t *temp);
status_t bmp280_spi_read_pressure(int32_t *pressure);

status_t bmp280_i2c_read_who_am_i(uint8_t* data);
status_t bmp280_i2c_init(uint8_t addr);
status_t bmp280_i2c_read_temp(uint8_t addr, int32_t *temp);
status_t bmp280_i2c_read_pressure(uint8_t addr, int32_t *pressure);


#endif /* BMP280_H_ */
