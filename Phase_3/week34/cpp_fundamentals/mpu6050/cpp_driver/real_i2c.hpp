#ifndef REAL_I2C_HPP_
#define REAL_I2C_HPP_

#include <cstdint>
#include <span>
#include <expected>
#include "status.hpp"

class RealI2c {
public:
    std::expected<void, ErrorCode> writeRegister(uint8_t dev_addr, uint8_t reg, uint8_t value);
    std::expected<uint8_t, ErrorCode> readRegister(uint8_t dev_addr, uint8_t reg);
    std::expected<void, ErrorCode> readBurst(uint8_t dev_addr, uint8_t reg, std::span<uint8_t> data);
};

#endif /* REAL_I2C_HPP_ */