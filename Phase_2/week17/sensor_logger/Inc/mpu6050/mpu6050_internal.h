/*
 * File:    mpu6050_internal.h
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This internal header file defines the register addresses and
 *   configuration values used to interface with the MPU6050 IMU
 *   sensor, including registers for gyroscope, accelerometer, power
 *   management, and device identity. It also specifies default
 *   configuration values and a startup delay constant for driver
 *   initialization.
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
