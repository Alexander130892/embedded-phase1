#include "fixed_format.hpp"

namespace {

// 10^9 is the largest power of ten that fits in uint32_t (max ~4.29e9).
constexpr uint8_t kMaxFracDigits = 9;

uint32_t pow10(uint8_t n)
{
    uint32_t p = 1;
    while (n-- > 0) { 
        p *= 10; 
    }
    return p;
}

// do/while: zero still has one digit.
uint8_t count_digits(uint32_t x)
{
    uint8_t n = 0;
    do { 
        x /= 10; ++n; 
    } while (x != 0);
    return n;
}

// Writes exactly `width` digits of x into dst, zero-padded on the left.
void write_digits(char* dst, uint32_t x, uint8_t width)
{
    for (uint8_t i = width; i > 0; --i) {
        dst[i - 1] = static_cast<char>('0' + (x % 10));
        x /= 10;
    }
}

} // namespace

std::size_t format_fixed(char* out, std::size_t cap, int32_t value, uint8_t frac_digits)
{
    if (out == nullptr || cap == 0) { 
        return 0; 
    }
    out[0] = '\0';
    if (frac_digits > kMaxFracDigits) { 
        return 0; 
    }

    // Sign handled once; magnitude in unsigned so INT32_MIN negates without UB.
    const bool     neg = value < 0;
    const uint32_t m   = neg ? 0u - static_cast<uint32_t>(value)
                             : static_cast<uint32_t>(value);

    const uint32_t scale   = pow10(frac_digits);
    const uint32_t ipart   = m / scale;
    const uint32_t fpart   = m % scale;
    const uint8_t  idigits = count_digits(ipart);

    const std::size_t len = (neg ? 1u : 0u) + idigits
                          + (frac_digits > 0 ? 1u + frac_digits : 0u);
    if (cap < len + 1) { 
        return 0; 
    }

    std::size_t pos = 0;
    if (neg) { 
        out[pos++] = '-'; 
    }
    write_digits(&out[pos], ipart, idigits);
    pos += idigits;
    if (frac_digits > 0) {
        out[pos++] = '.';
        write_digits(&out[pos], fpart, frac_digits);
        pos += frac_digits;
    }
    out[pos] = '\0';
    return pos;
}