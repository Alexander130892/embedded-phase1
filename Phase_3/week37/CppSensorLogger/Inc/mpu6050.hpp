#pragma once

#include <array>
#include <cstdint>

#include "bus.hpp"

// Decoded IMU sample in physical units (fixed point).
struct ImuSample {
    std::int32_t accel_x_mg,  accel_y_mg,  accel_z_mg;    // milli-g
    std::int32_t gyro_x_mdps, gyro_y_mdps, gyro_z_mdps;   // milli-degrees per second
};

// MPU-6050 6-axis IMU. Header-only: Bus is chosen at compile time
// (I2c1Bus in firmware, a fake bus in host tests).
template <I2cBusLike Bus>
class Mpu6050 {
public:
    static constexpr std::uint8_t kAddrAd0Low  = 0x68;   // 7-bit address, AD0 = GND
    static constexpr std::uint8_t kWhoAmIValue = 0x68;

    constexpr Mpu6050(Bus& bus, std::uint8_t addr) : bus_{bus}, addr_{addr} {}

    // Wake up, 1 kHz / (1 + 7) = 125 Hz sample rate, ±250 °/s, ±2 g.
    BusStatus init()
    {
        BusStatus status = bus_.write_reg(addr_, kRegPwrMgmt1, 0x00);
        if(status != BusStatus::Ok){
            return status;
        }
        status = bus_.write_reg(addr_, kRegSmplrtDiv, kSmplrtDiv);
        if(status != BusStatus::Ok){
            return status;
        } 
        status = bus_.write_reg(addr_, kRegGyroConfig, kGyroFs250); 
        if(status != BusStatus::Ok){
            return status;
        }
        status = bus_.write_reg(addr_, kRegAccelConfig, kAccelFs2g);
        if(status != BusStatus::Ok){
            return status;
        }
        return BusStatus::Ok;
    }

    BusStatus who_am_i(std::uint8_t& id)
    {
        return bus_.read_burst(addr_, kRegWhoAmI, &id, 1);
    }

    // One 14-byte burst from ACCEL_XOUT_H: accel X/Y/Z, temp, gyro X/Y/Z,
    // each big-endian int16. Converted to milli-g / milli-dps.
    BusStatus read(ImuSample& out)
    {
        std::array<std::uint8_t, 14> buf{};
        const BusStatus st = bus_.read_burst(addr_, kRegAccelXoutH, buf.data(),
                                             static_cast<std::uint8_t>(buf.size()));
        if (st != BusStatus::Ok) {
            return st;               // out untouched on error
        }
        out.accel_x_mg = to_mg(be16(buf[0], buf[1]));
        out.accel_y_mg = to_mg(be16(buf[2], buf[3]));
        out.accel_z_mg = to_mg(be16(buf[4], buf[5]));

        out.gyro_x_mdps = to_mdps(be16(buf[8], buf[9]));
        out.gyro_y_mdps = to_mdps(be16(buf[10], buf[11]));
        out.gyro_z_mdps = to_mdps(be16(buf[12], buf[13]));
        return BusStatus::Ok;
    }

private:
    // --- Register map (RM-MPU-6000A) ---------------------------------------
    static constexpr std::uint8_t kRegSmplrtDiv   = 0x19;
    static constexpr std::uint8_t kRegGyroConfig  = 0x1B;
    static constexpr std::uint8_t kRegAccelConfig = 0x1C;
    static constexpr std::uint8_t kRegAccelXoutH  = 0x3B;
    static constexpr std::uint8_t kRegPwrMgmt1    = 0x6B;
    static constexpr std::uint8_t kRegWhoAmI      = 0x75;

    // --- Configuration written by init() ------------------------------------
    static constexpr std::uint8_t kSmplrtDiv = 0x07;
    static constexpr std::uint8_t kGyroFs250 = 0x00;   // FS_SEL = 0
    static constexpr std::uint8_t kAccelFs2g = 0x00;   // AFS_SEL = 0

    // --- Scale factors: MUST match the ranges above (one place) -------------
    static constexpr std::int32_t kAccelLsbPerG   = 16384;   // ±2 g
    static constexpr std::int32_t kGyroLsbPerDps  = 131;     // ±250 °/s

    // Big-endian byte pair --> signed 16-bit value.
    static constexpr std::int16_t be16(std::uint8_t hi, std::uint8_t lo)
    {
        const auto u = static_cast<std::uint16_t>((hi << 8) | lo);   // int → the 16 raw bits
        return static_cast<std::int16_t>(u);                         // reinterpret as signed (defined in C++20)
    }

    static constexpr std::int32_t to_mg(std::int16_t raw)
    {
        std::int32_t raw_widen = static_cast<std::int32_t>(raw) * 1000;
        return raw_widen/kAccelLsbPerG;
    }
    static constexpr std::int32_t to_mdps(std::int16_t raw)
    {
        return static_cast<std::int32_t>(raw) * 1000 / kGyroLsbPerDps;
    }

    Bus&         bus_;
    std::uint8_t addr_;
};