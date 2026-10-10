// Inc/log_record.hpp
#pragma once
#include <cstdint>
#include "mpu6050.hpp"   // ImuSample
#include "bmp280.h"

struct LogRecord {
    std::uint32_t timestamp_ms;  // systick at the start of the tick
    EnvSample     env;
    ImuSample     imu;
    bool          env_valid;     // false → env read failed this tick
    bool          imu_valid;     // false → imu read failed this tick
};