// Inc/sensor_hw.hpp — the hardware "port" the SensorFsm talks to
//
// The FSM never calls a driver directly. Everything it needs from the outside
// world goes through one object satisfying SensorHw:
//   - on the board: BoardHw in main.cpp (Mpu6050<I2c1Bus> + Week 17 C functions)
//   - in host tests: FakeHw with scripted results and call counters
#pragma once

#include <concepts>
#include <cstdint>

#include "bus.hpp"          // BusStatus
#include "log_record.hpp"   // EnvSample, ImuSample

template <class H>
concept SensorHw = requires(H h, ImuSample& imu, EnvSample& env) {
    { h.imu_init() }     -> std::same_as<BusStatus>;      // MPU-6050 wake + config
    { h.imu_read(imu) }  -> std::same_as<BusStatus>;      // one 14-byte burst, scaled
    { h.env_read(env) }  -> std::same_as<bool>;           // true = temp AND pressure read OK
    { h.bus_reset() }    -> std::same_as<BusStatus>;      // I2C recovery (9 clocks + STOP + re-init)
    { h.now_ms() }       -> std::same_as<std::uint32_t>;  // timestamp for the LogRecord
};