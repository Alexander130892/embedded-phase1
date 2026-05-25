/*
 * File:    mpu6050.h
 * Author:  Alexander130892
 * Date:    25-5-2026
 *
 * Description:
 *   This header file defines the interface for communicating with an
 *   MPU6050 inertial measurement unit (IMU), providing structures and
 *   functions to initialize the sensor and read accelerometer and
 *   gyroscope data. The file declares the sensor's I2C addresses and
 *   data structure, along with functions for initialization, data
 *   reading, and device identification.
 */
#ifndef MPU6050_MPU6050_H_
#define MPU6050_MPU6050_H_


#include <stdint.h>
#include "status.h"

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
