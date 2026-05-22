/*
 * File:    mpu6050.h
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This header defines the interface for an MPU6050 IMU driver,
 *   declaring the I2C device addresses, a data structure holding raw
 *   16-bit accelerometer and gyroscope readings, and functions to
 *   initialise the device, read sensor data, and query its WHO_AM_I
 *   register.
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
