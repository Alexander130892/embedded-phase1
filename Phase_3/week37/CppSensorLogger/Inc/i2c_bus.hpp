#pragma once

#include <cstdint>
#include "i2c.h"          // C API, now with extern "C" guards
#include "bus.hpp"

// Adapts the Week 17 C I2C driver to the interface Mpu6050<Bus> expects.
struct I2c1Bus {
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

