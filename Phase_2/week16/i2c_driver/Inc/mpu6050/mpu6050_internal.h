/*
 * File:    mpu6050_internal.h
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This internal header defines register address constants for the
 *   MPU6050 IMU sensor, specifically for the accelerometer output,
 *   gyroscope output, power management, and device identity registers.
 *   It is intended for use within the MPU6050 driver implementation to
 *   avoid hardcoding raw register addresses throughout the code.
 */
#ifndef MPU6050_MPU6050_INTERNAL_H_
#define MPU6050_MPU6050_INTERNAL_H_

#define	ACCEL_XOUT_H	0x3B
#define	GYRO_ZOUT_L		0x48
#define PWR_MGMT_1		0x6B
#define WHO_AM_I		0x75



#endif /* MPU6050_MPU6050_INTERNAL_H_ */
