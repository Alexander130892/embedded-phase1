#pragma once

#include <cstdint>
#include <concepts>
#include "i2c.h"          // C API, now with extern "C" guards

enum class [[nodiscard]] BusStatus : std::uint8_t {
    Ok,
    Timeout,          // bus stuck / slave holding SCL  → candidate for i2c_bus_reset()
    Nack,             // no ACK: wrong address or sensor not connected
    ArbitrationLost,  // another master / noise on the bus
    Unknown,          // status_t value this layer doesn't recognise
};

template <class B>
concept I2cBusLike = requires(B b, std::uint8_t d, std::uint8_t r, std::uint8_t v,
                              std::uint8_t* p, std::uint8_t n) {
    { b.write_reg(d, r, v) }     -> std::same_as<BusStatus>;
    { b.read_burst(d, r, p, n) } -> std::same_as<BusStatus>;
};

// Adapts the Week 17 C I2C driver to the interface Mpu6050<Bus> expects.
struct CI2cBus {
    BusStatus write_reg(std::uint8_t dev, std::uint8_t reg, std::uint8_t val)
    {
        return to_bus_status(i2c_write_register(dev, reg, val));
    }
    BusStatus read_burst(std::uint8_t dev, std::uint8_t reg, std::uint8_t* buf, std::uint8_t len)
    {
        return to_bus_status(i2c_read_burst(dev, reg, buf, len));
    }
    private:
        static constexpr BusStatus to_bus_status(status_t s)
        {
            switch (s) {
                case STATUS_OK:   return BusStatus::Ok;
                case I2C_TIMEOUT: return BusStatus::Timeout;
                case I2C_AF:      return BusStatus::Nack;
                case I2C_ARLO:    return BusStatus::ArbitrationLost;
                default:          return BusStatus::Unknown;   // SPI_TIMEOUT, UART_TIMEOUT, …
            }
        }
};

