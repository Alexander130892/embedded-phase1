/*
 * File:    mpu6050.c
 * Author:  Alexander130892
 * Date:    25-5-2026
 *
 * Description:
 *   This file implements a driver for the MPU6050 inertial measurement
 *   unit (IMU), providing functions to initialize the device, read
 *   accelerometer and gyroscope data via I2C, and verify device
 *   identity. The initialization routine configures power management,
 *   sample rate, and sensor sensitivities with delays between register
 *   writes.
 */
#include "mpu6050_internal.h"
#include "mpu6050.h"
#include "status.h"
#include "i2c.h"

status_t mpu6050_init(uint8_t dev_addr){
		status_t status = STATUS_OK;
	    status = i2c_write_register(dev_addr, MPU6050_PWR_MGMT_1, 0x00);
	    if(status){
	    	return status;
	    }
	    for(volatile int i = 0; i < MPU6050_STARTUP_DELAY; i++);
	    status = i2c_write_register(dev_addr, MPU6050_SMPLRT_DIV, MPU6050_SMPLRT_DIV_VALUE);  // SMPLRT_DIV
	    if(status){
			return status;
		}
	    for(volatile int i = 0; i < MPU6050_STARTUP_DELAY; i++);
	    status = i2c_write_register(dev_addr, MPU6050_GYRO_CONFIG, MPU6050_GYRO_CONFIG_VALUE);  // GYRO_CONFIG
	    if(status){
			return status;
		}
	    for(volatile int i = 0; i < MPU6050_STARTUP_DELAY; i++);
	    status = i2c_write_register(dev_addr, MPU6050_ACCEL_CONFIG, MPU6050_ACCEL_CONFIG_VALUE);  // ACCEL_CONFIG
	    if(status){
			return status;
		}
	    for(volatile int i = 0; i < MPU6050_STARTUP_DELAY; i++);
	    return STATUS_OK;
}
status_t mpu6050_read_data(uint8_t dev_addr, mpu6050_data_t* p_data){
	uint8_t buff[14];
	status_t status = STATUS_OK;
	status = i2c_read_burst(dev_addr, MPU6050_ACCEL_XOUT_H, buff, 14);
	if(status){
		return status;
	}
	p_data->accel_x = (int16_t) (buff[0] << 8 | buff[1]);
	p_data->accel_y = (int16_t) (buff[2] << 8 | buff[3]);
	p_data->accel_z = (int16_t) (buff[4] << 8 | buff[5]);

	p_data->gyro_x = (int16_t) (buff[8] << 8  | buff[9]);
	p_data->gyro_y = (int16_t) (buff[10] << 8 | buff[11]);
	p_data->gyro_z = (int16_t) (buff[12] << 8 | buff[13]);
	return STATUS_OK;
}
status_t mpu6050_read_who_am_i(uint8_t* data){
	return i2c_read_register(MPU6050_ADDR_ADO_LOW, MPU6050_WHO_AM_I, data);
}
