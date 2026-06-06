/*
 * File:    mpu6050.h
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This header file defines the interface for an MPU6050 6-axis
 *   inertial measurement unit (IMU) driver, providing functions to
 *   initialize the device, read accelerometer and gyroscope data, and
 *   verify device identity. It includes the necessary data structures
 *   and I2C address constants for communicating with the sensor.
 */
#ifndef MPU6050_MPU6050_H_
#define MPU6050_MPU6050_H_


#include <stdint.h>
#include "drivers/status.h"

#define MPU6050_ADDR_ADO_LOW  0x68
#define MPU6050_ADDR_ADO_HIGH 0x69

typedef struct {
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x,  gyro_y,  gyro_z;
} mpu6050_data_t;

status_t mpu6050_init(uint8_t dev_addr);
status_t mpu6050_read_data(uint8_t dev_addr, mpu6050_data_t* p_data);
status_t mpu6050_read_who_am_i(uint8_t* data);
#endif /* MPU6050_MPU6050_H_ */
