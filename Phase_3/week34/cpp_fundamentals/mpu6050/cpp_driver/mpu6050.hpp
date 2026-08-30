#ifndef MPU6050_HPP_
#define MPU6050_HPP_

#include <cstdint>
#include <array>
#include <span>
#include <expected>
#include "status.hpp"
#include "../c_driver/mpu6050.h"
#include "../c_driver/mpu6050_internal.h"

template <typename I2CTransport>
class Mpu6050 {
public:
    Mpu6050(uint8_t dev_addr, I2CTransport& transport)
        : dev_addr_(dev_addr), transport_(transport) {}

    struct mpu6050_data_t {
        int16_t accel_x, accel_y, accel_z;
        int16_t gyro_x,  gyro_y,  gyro_z;
    };

    std::expected<void, ErrorCode> init() {
        std::expected<void, ErrorCode> status = this->transport_.writeRegister(this->dev_addr_, MPU6050_PWR_MGMT_1, 0x00);
        if (!status) {
            return std::unexpected(status.error());
        }
        for (volatile int i = 0; i < MPU6050_STARTUP_DELAY; i = i + 1);

        status = this->transport_.writeRegister(this->dev_addr_, MPU6050_SMPLRT_DIV, MPU6050_SMPLRT_DIV_VALUE);
        if (!status) {
            return std::unexpected(status.error());
        }
        for (volatile int i = 0; i < MPU6050_STARTUP_DELAY; i = i + 1);

        status = this->transport_.writeRegister(this->dev_addr_, MPU6050_GYRO_CONFIG, MPU6050_GYRO_CONFIG_VALUE);
        if (!status) {
            return std::unexpected(status.error());
        }
        for (volatile int i = 0; i < MPU6050_STARTUP_DELAY; i = i + 1);

        status = this->transport_.writeRegister(this->dev_addr_, MPU6050_ACCEL_CONFIG, MPU6050_ACCEL_CONFIG_VALUE);
        if (!status) {
            return std::unexpected(status.error());
        }
        for (volatile int i = 0; i < MPU6050_STARTUP_DELAY; i = i + 1);

        return {};
    }

    std::expected<mpu6050_data_t, ErrorCode> read_data() {
        std::array<uint8_t, 14> storage;
        std::span<uint8_t, 14> buff(storage);
        std::expected<void, ErrorCode> status;
        status = this->transport_.readBurst(this->dev_addr_, MPU6050_ACCEL_XOUT_H, buff);
        if (!status) {
            return std::unexpected(status.error());
        }
        mpu6050_data_t ret_val;
        ret_val.accel_x = (int16_t) (buff[0] << 8 | buff[1]);
        ret_val.accel_y = (int16_t) (buff[2] << 8 | buff[3]);
        ret_val.accel_z = (int16_t) (buff[4] << 8 | buff[5]);
        ret_val.gyro_x  = (int16_t) (buff[8] << 8  | buff[9]);
        ret_val.gyro_y  = (int16_t) (buff[10] << 8 | buff[11]);
        ret_val.gyro_z  = (int16_t) (buff[12] << 8 | buff[13]);
        return ret_val;
    }

    std::expected<uint8_t, ErrorCode> read_who_am_i() {
        std::expected<uint8_t, ErrorCode> status;
        status = this->transport_.readRegister(this->dev_addr_, MPU6050_WHO_AM_I);
        if (!status) {
            return std::unexpected(status.error());
        }
        return status;
    }

private:
    uint8_t dev_addr_;
    I2CTransport& transport_;
};

#endif /* MPU6050_HPP_ */