/*
 * File:    i2c.h
 * Author:  Alexander130892
 * Date:    16-5-2026
 *
 * Description:
 *   This header file declares the interface for an I2C driver
 *   targeting the STM32F446RE microcontroller, providing functions for
 *   bus initialization, GPIO setup, start/stop/restart conditions,
 *   byte-level read/write operations, and higher-level register access
 *   and burst reads for communicating with I2C peripheral devices.
 */
#ifndef I2C_H_
#define I2C_H_

#include "stm32f446re.h"
#include <stdint.h>

void 	init_i2c_gpio(void);
void 	i2c_init();
void 	i2c_start();
void 	i2c_restart();
void 	i2c_stop();
void 	i2c_write_byte(uint8_t data);
uint8_t i2c_write_addr(uint8_t addr);
uint8_t i2c_read_byte();
uint8_t	i2c_write_register(uint8_t dev_addr, uint8_t reg, uint8_t data);
uint8_t i2c_read_register(uint8_t dev_addr, uint8_t reg, uint8_t *data);
uint8_t	i2c_read_burst(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len);
uint8_t	i2c_bus_reset();

#endif /* I2C_H_ */
