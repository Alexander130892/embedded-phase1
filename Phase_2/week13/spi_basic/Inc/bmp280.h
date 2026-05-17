/*
 * File:    bmp280.h
 * Author:  Alexander130892
 * Date:    6-5-2026
 *
 * Description:
 *   This header file defines the interface for communicating with a
 *   BMP280 sensor, providing functions to read and write registers and
 *   check the device identity.
 */
#ifndef BMP280_H_
#define BMP280_H_

#include <stdint.h>

uint8_t bmp280_read_register(uint8_t addr);
void 	bmp280_write_register(uint8_t addr, uint8_t data);
uint8_t bmp280_read_who_am_i();

#endif /* BMP280_H_ */
