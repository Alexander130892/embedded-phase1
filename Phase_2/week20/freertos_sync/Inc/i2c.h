/*
 * File:    i2c.h
 * Author:  Alexander130892
 * Date:    24-5-2026
 *
 * Description:
 *   This header file provides a driver interface for I2C communication
 *   on an STM32F446RE microcontroller, defining initialization
 *   routines, bus control functions, and read/write operations for I2C
 *   transactions at 100 kHz with bus recovery capabilities.
 */
#ifndef I2C_H_
#define I2C_H_

#include "stm32f446re.h"
#include "status.h"
#include <stdint.h>

#define I2C_FREQ_16MHZ    			0x10
#define I2C_CCR_100KHZ    			0x50
#define I2C_TRISE_100KHZ  			0x11

#define I2C_BUS_RECOVERY_CLOCKS   	9
#define I2C_BUS_RECOVERY_DELAY    	10000

#define I2C_TRISE_MASK    		0x3F

status_t 	i2c_init_gpio(void);
status_t 	i2c_init(void);
status_t 	i2c_start(void);
status_t 	i2c_restart(void);
status_t 	i2c_stop(void);
status_t 	i2c_write_byte(uint8_t data);
status_t 	i2c_read_byte(uint8_t *data);
status_t 	i2c_write_addr(uint8_t addr);
status_t	i2c_write_register(uint8_t dev_addr, uint8_t reg, uint8_t data);
status_t 	i2c_read_register(uint8_t dev_addr, uint8_t reg, uint8_t *data);
status_t	i2c_read_burst(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len);
status_t	i2c_bus_reset(void);

#endif /* I2C_H_ */
