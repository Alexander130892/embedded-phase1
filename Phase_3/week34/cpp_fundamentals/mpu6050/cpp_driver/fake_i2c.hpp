#ifndef FAKE_I2C_HPP_
#define FAKE_I2C_HPP_

#include <cstdint>
#include <span>
#include <expected>
#include "fff.h"
DEFINE_FFF_GLOBALS;
#include "status.hpp"

// fff fakes — simple int-based, standing in for status_t
FAKE_VALUE_FUNC(int, fake_write_register, uint8_t, uint8_t, uint8_t);
FAKE_VALUE_FUNC(int, fake_read_register, uint8_t, uint8_t, uint8_t*);
FAKE_VALUE_FUNC(int, fake_read_burst, uint8_t, uint8_t, uint8_t*, size_t);

class FakeI2c {
public:
    std::expected<void, ErrorCode> writeRegister(uint8_t dev_addr, uint8_t reg, uint8_t value) {
        int result = fake_write_register(dev_addr, reg, value);
        if (result != 0) {
            return std::unexpected(ErrorCode::UNKNOWN_ERROR);
        }
        return {};
    }

    std::expected<uint8_t, ErrorCode> readRegister(uint8_t dev_addr, uint8_t reg) {
        uint8_t value = 0;
        int result = fake_read_register(dev_addr, reg, &value);
        if (result != 0) {
            return std::unexpected(ErrorCode::UNKNOWN_ERROR);
        }
        return value;
    }

    std::expected<void, ErrorCode> readBurst(uint8_t dev_addr, uint8_t reg, std::span<uint8_t> data) {
        int result = fake_read_burst(dev_addr, reg, data.data(), data.size());
        if (result != 0) {
            return std::unexpected(ErrorCode::UNKNOWN_ERROR);
        }
        return {};
    }
};

#endif /* FAKE_I2C_HPP_ */