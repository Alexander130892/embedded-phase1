#ifndef STATUS_HPP_
#define STATUS_HPP_

#include <cstdint>

constexpr uint32_t MAX_TIMEOUT = 10000;

enum class ErrorCode : uint8_t {
    STATUS_OK,
    I2C_ARLO,
    I2C_TIMEOUT,
    I2C_AF,
    UART_RB_FULL,
    UART_RB_EMPTY,
    UNKNOWN_ERROR
};

#endif /* STATUS_HPP_ */