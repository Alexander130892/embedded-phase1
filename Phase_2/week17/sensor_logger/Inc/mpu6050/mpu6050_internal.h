/*
 * mpu6050_internal.h
 *
 *  Created on: May 16, 2026
 *      Author: alexandervindelinckx
 */

#ifndef MPU6050_MPU6050_INTERNAL_H_
#define MPU6050_MPU6050_INTERNAL_H_

//Registers
#define 	MPU6050_SMPLRT_DIV				0x19
#define 	MPU6050_GYRO_CONFIG				0x1B
#define 	MPU6050_ACCEL_CONFIG			0x1C
#define		MPU6050_ACCEL_XOUT_H			0x3B
#define		MPU6050_GYRO_ZOUT_L				0x48
#define 	MPU6050_PWR_MGMT_1				0x6B
#define 	MPU6050_WHO_AM_I				0x75

//Values
#define 	MPU6050_STARTUP_DELAY   		100000
#define 	MPU6050_SMPLRT_DIV_VALUE		0x07
#define 	MPU6050_GYRO_CONFIG_VALUE		0x00
#define 	MPU6050_ACCEL_CONFIG_VALUE		0x00

#endif /* MPU6050_MPU6050_INTERNAL_H_ */
