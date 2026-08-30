#include "real_i2c.hpp"
#include <cassert>
#include "../c_driver/i2c.h"
#include "../c_driver/status.h"

std::expected<void, ErrorCode> RealI2c::writeRegister(uint8_t dev_addr, uint8_t reg, uint8_t value) {
    status_t status = i2c_write_register(dev_addr, reg, value);
    if (status != STATUS_OK) {
        switch (status) {
            case I2C_TIMEOUT:
                return std::unexpected(ErrorCode::I2C_TIMEOUT);
            case I2C_AF:
                return std::unexpected(ErrorCode::I2C_AF);
            case I2C_ARLO:
                return std::unexpected(ErrorCode::I2C_ARLO);
            default:
                assert(false && "Unknown Error!\n");
                return std::unexpected(ErrorCode::UNKNOWN_ERROR);
        }
    }
    return {};
}

std::expected<uint8_t, ErrorCode> RealI2c::readRegister(uint8_t dev_addr, uint8_t reg) {
    uint8_t return_val;
    status_t status = i2c_read_register(dev_addr, reg, &return_val);
    if (status != STATUS_OK) {
        switch (status) {
            case I2C_TIMEOUT:
                return std::unexpected(ErrorCode::I2C_TIMEOUT);
            case I2C_AF:
                return std::unexpected(ErrorCode::I2C_AF);
            case I2C_ARLO:
                return std::unexpected(ErrorCode::I2C_ARLO);
            default:
                assert(false && "Unknown Error!\n");
                return std::unexpected(ErrorCode::UNKNOWN_ERROR);
        }
    }
    return return_val;
}

std::expected<void, ErrorCode> RealI2c::readBurst(uint8_t dev_addr, uint8_t reg, std::span<uint8_t> data) {
    status_t status = i2c_read_burst(dev_addr, reg, data.data(), data.size());
    if (status != STATUS_OK) {
        switch (status) {
            case I2C_TIMEOUT:
                return std::unexpected(ErrorCode::I2C_TIMEOUT);
            case I2C_AF:
                return std::unexpected(ErrorCode::I2C_AF);
            case I2C_ARLO:
                return std::unexpected(ErrorCode::I2C_ARLO);
            default:
                assert(false && "Unknown Error!\n");
                return std::unexpected(ErrorCode::UNKNOWN_ERROR);
        }
    }
    return {};
}