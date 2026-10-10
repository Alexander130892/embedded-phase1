// LogRecord → one CSV line (pure, no hardware)
#pragma once

#include <cstddef>

#include "log_record.hpp"

// Column header matching format_csv's output, including the line ending.
inline constexpr const char* kCsvHeader =
    "t_ms,T_C,P_hPa,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps\r\n";

// Worst case: 10-digit timestamp + 8 fields of at most 12 chars ("-2147483.648")
// + 8 commas + "\r\n" + '\0' = 117. Rounded up.
inline constexpr std::size_t kCsvLineMax = 128;

// Writes one line: "t_ms,T,P,ax,ay,az,gx,gy,gz\r\n"
//   T in °C (2 decimals), P in hPa (2 decimals), accel in g (3), gyro in dps (3).
//   An invalid sensor (env_valid / imu_valid == false) leaves its fields empty: "1000,,,0.012,..."
// Contract (same as format_fixed / snprintf-style):
//   returns the line length (excluding '\0');
//   if the line does not fit in cap, writes nothing usable: out[0] = '\0' and returns 0.
//   Never a truncated line — a cut-off CSV row would shift or corrupt columns downstream.
std::size_t format_csv(char* out, std::size_t cap, const LogRecord& r);