#pragma once

#include <cstddef>
#include <cstdint>

// Formats `value` as a fixed-point decimal with `frac_digits` digits after the point.
//   format_fixed(buf, sizeof buf, -150, 2)  ->  "-1.50", returns 5
// Contract (like snprintf):
//   - On success: writes len chars + '\0', returns len. Requires cap >= len + 1.
//   - On failure (cap too small, frac_digits > 9): writes '\0' to out[0] if cap > 0, returns 0.
std::size_t format_fixed(char* out, std::size_t cap, int32_t value, uint8_t frac_digits);