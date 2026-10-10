
#include "csv_format.hpp"
#include <cstdint>
#include "fixed_format.hpp"

namespace {

// Appends pieces to a fixed buffer; remembers the first failure.
class LineWriter {
public:
    LineWriter(char* out, std::size_t cap) : out_{out}, cap_{cap} {}

    void text(const char* s)
    {
        while (ok_ && *s != '\0') {
            put(*s++);
        }
    }

    void fixed(std::int32_t value, std::uint8_t frac_digits)
    {
        if (!ok_) {
            return;
        }
        // format_fixed writes value + '\0' at the current position.
        const std::size_t n = format_fixed(out_ + len_, cap_ - len_, value, frac_digits);
        if (n == 0) {
            ok_ = false;
            return;
        }
        len_ += n;
    }

    void unsigned_dec(std::uint32_t value)
    {
        char tmp[10];                 // 4294967295 has 10 digits
        std::size_t n = 0;
        do {
            tmp[n++] = static_cast<char>('0' + value % 10u);
            value /= 10u;
        } while (value != 0u);
        while (ok_ && n > 0) {
            put(tmp[--n]);
        }
    }

    // Terminates the line; returns its length, or 0 (with out[0] = '\0') on failure.
    std::size_t finish()
    {
        if (ok_ && len_ < cap_) {
            out_[len_] = '\0';
            return len_;
        }
        if (cap_ > 0) {
            out_[0] = '\0';
        }
        return 0;
    }

private:
    void put(char c)
    {
        if (len_ + 1 >= cap_) {       // always keep room for the '\0'
            ok_ = false;
            return;
        }
        out_[len_++] = c;
    }

    char*       out_;
    std::size_t cap_;
    std::size_t len_{0};
    bool        ok_{true};
};

} // namespace

std::size_t format_csv(char* out, std::size_t cap, const LogRecord& r)
{
    if (out == nullptr || cap == 0) {
        return 0;
    }
    LineWriter w{out, cap};

    w.unsigned_dec(r.timestamp_ms);       // uint32: format_fixed takes int32 and would
                                          // go negative after 2^31 ms (~24.8 days)
    w.text(",");
    if (r.env_valid) {
        w.fixed(r.env.temp_centi_c, 2);   // 1971   → "19.71"   °C
        w.text(",");
        w.fixed(r.env.press_pa, 2);       // 101140 → "1011.40" hPa (Pa / 100)
    } else {
        w.text(",");
    }

    const std::int32_t imu_fields[6] = {
        r.imu.accel_x_mg,  r.imu.accel_y_mg,  r.imu.accel_z_mg,
        r.imu.gyro_x_mdps, r.imu.gyro_y_mdps, r.imu.gyro_z_mdps,
    };
    for (const std::int32_t v : imu_fields) {
        w.text(",");
        if (r.imu_valid) {
            w.fixed(v, 3);                // milli → 3 decimals: -981 → "-0.981"
        }
    }

    w.text("\r\n");
    return w.finish();
}