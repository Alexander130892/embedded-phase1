#pragma once

#include <cstdint>
#include <concepts>
#include "i2c.h"

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