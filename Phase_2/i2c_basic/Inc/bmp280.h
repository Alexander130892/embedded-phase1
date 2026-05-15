/*
 * bmp280.h
 *
 *  Created on: May 5, 2026
 *      Author: alexandervindelinckx
 */

#ifndef BMP280_H_
#define BMP280_H_

#include <stdint.h>

uint8_t bmp280_read_register(uint8_t addr);
void 	bmp280_write_register(uint8_t addr, uint8_t data);
uint8_t bmp280_read_who_am_i();
uint8_t bmp280_i2c_read_who_am_i();

#endif /* BMP280_H_ */
