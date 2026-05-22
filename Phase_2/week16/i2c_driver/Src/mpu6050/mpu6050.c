/*
 * File:    mpu6050.c
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This file implements a driver for the MPU-6050 IMU sensor over
 *   I2C, providing functions to initialize the device (waking it from
 *   sleep and configuring sample rate, gyroscope, and accelerometer
 *   settings) and to read raw 16-bit accelerometer and gyroscope data
 *   via a burst read. It also includes a utility function to read the
 *   WHO_AM_I register for device identification.
 */
#include "mpu6050_internal.h"
#include "mpu6050.h"
#include "i2c.h"

void mpu6050_init(uint8_t dev_addr){
	    i2c_write_register(dev_addr, PWR_MGMT_1, 0x00);
	    for(volatile int i = 0; i < 100000; i++);
	    i2c_write_register(dev_addr, 0x19, 0x07);  // SMPLRT_DIV
	    for(volatile int i = 0; i < 10000; i++);
	    i2c_write_register(dev_addr, 0x1B, 0x00);  // GYRO_CONFIG
	    for(volatile int i = 0; i < 10000; i++);
	    i2c_write_register(dev_addr, 0x1C, 0x00);  // ACCEL_CONFIG
	    for(volatile int i = 0; i < 10000; i++);

}
void mpu6050_read_data(uint8_t dev_addr, mpu6050_data_t* p_data){
	uint8_t buff[14];
	i2c_read_burst(dev_addr, ACCEL_XOUT_H, buff, 14);
	p_data->accel_x = (int16_t) (buff[0] << 8 | buff[1]);
	p_data->accel_y = (int16_t) (buff[2] << 8 | buff[3]);
	p_data->accel_z = (int16_t) (buff[4] << 8 | buff[5]);

	p_data->gyro_x = (int16_t) (buff[8] << 8  | buff[9]);
	p_data->gyro_y = (int16_t) (buff[10] << 8 | buff[11]);
	p_data->gyro_z = (int16_t) (buff[12] << 8 | buff[13]);
}
void mpu6050_read_who_am_i(uint8_t* data){
	i2c_read_register(MPU6050_ADDR_ADO_LOW, WHO_AM_I, data);
}
