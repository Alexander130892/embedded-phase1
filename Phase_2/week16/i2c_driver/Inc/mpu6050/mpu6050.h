/*
 * File:    mpu6050.h
 * Author:  Alexander130892
 * Date:    17-5-2026
 *
 * Description:
 *   This header file defines the interface for an MPU6050 IMU driver,
 *   declaring I2C device address constants, a data structure for
 *   storing raw 16-bit accelerometer and gyroscope readings, and
 *   functions for initializing the sensor, reading motion data, and
 *   querying its WHO_AM_I register.
 */
#ifndef MPU6050_MPU6050_H_
#define MPU6050_MPU6050_H_

#include <stdint.h>

#define MPU6050_ADDR_ADO_LOW  0x68
#define MPU6050_ADDR_ADO_HIGH 0x69

typedef struct {
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x,  gyro_y,  gyro_z;
} mpu6050_data_t;

void mpu6050_init(uint8_t dev_addr);
void mpu6050_read_data(uint8_t dev_addr, mpu6050_data_t* p_data);
void mpu6050_read_who_am_i(uint8_t* data);
#endif /* MPU6050_MPU6050_H_ */
