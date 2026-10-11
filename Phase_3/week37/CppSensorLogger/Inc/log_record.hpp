// Inc/log_record.hpp
#pragma once
#include <cstdint>
#include "mpu6050.hpp"   // ImuSample

struct EnvSample {
    std::int32_t temp_centi_c;   // 0.01 °C  (Bosch compensation output)
    std::int32_t press_pa;       // Pa
};

struct LogRecord {
    std::uint32_t timestamp_ms;  // systick at the start of the tick
    EnvSample     env;
    ImuSample     imu;
    bool          env_valid;     // false → env read failed this tick
    bool          imu_valid;     // false → imu read failed this tick
};