#include <cstdint>
#include <cstring>

#include "bsp.hpp"
#include "fixed_format.hpp"
#include "i2c.h"            // C API: i2c_init_gpio(), i2c_init()
#include "i2c_bus.hpp"     // I2c1Bus adapter
#include "mpu6050.hpp"
#include "uart.hpp"

constinit Uart g_uart2{0x4000'4400u};   // USART2

namespace {

const char* to_string(BusStatus s)
{
    switch (s) {
        case BusStatus::Ok:              return "Ok";
        case BusStatus::Timeout:         return "Timeout";
        case BusStatus::Nack:            return "Nack";
        case BusStatus::ArbitrationLost: return "ArbitrationLost";
        case BusStatus::Unknown:         return "Unknown";
    }
    return "?";
}

// Prints a milli-unit value as units with 3 decimals: -981 → "-0.981"
void print_milli(const char* label, std::int32_t milli)
{
    char num[16];
    format_fixed(num, sizeof num, milli, 3);
    g_uart2.send_string(label);
    g_uart2.send_string(num);
}

void print_sample(const ImuSample& s)
{
    print_milli("accel[g] x=",   s.accel_x_mg);
    print_milli(" y=",           s.accel_y_mg);
    print_milli(" z=",           s.accel_z_mg);
    print_milli("\tgyro[dps] x=", s.gyro_x_mdps);
    print_milli(" y=",           s.gyro_y_mdps);
    print_milli(" z=",           s.gyro_z_mdps);
    g_uart2.send_string("\r\n");
}

} // namespace

int main()
{
    bsp_init();          // PA2/PA3 → AF7 (USART2)
    g_uart2.init();      // clock, 115200 8N1, RXNE IRQ
    g_uart2.send_string("CppSensorLogger up\r\n");

    (void)i2c_init_gpio();                    // Week 17 C driver: I2C1 pins + clock
    (void)i2c_init();                         // I2C1 100 kHz

    I2c1Bus bus;
    Mpu6050<I2c1Bus> imu{bus, Mpu6050<I2c1Bus>::kAddrAd0Low};

    const BusStatus st = imu.init();
    g_uart2.send_string("MPU-6050 init: ");
    g_uart2.send_string(to_string(st));
    g_uart2.send_string("\r\nType 'imu' for a sample\r\n");

    char line[Uart::kMaxLineLen + 1];
    for (;;) {
        if (!g_uart2.receive_line(line, sizeof line)) {
            continue;
        }
        if (std::strcmp(line, "imu") == 0) {
            ImuSample s{};
            const BusStatus rs = imu.read(s);
            if (rs == BusStatus::Ok) {
                print_sample(s);
            } else {
                g_uart2.send_string("read failed: ");
                g_uart2.send_string(to_string(rs));
                g_uart2.send_string("\r\n");
            }
        } else {
            g_uart2.send_string("> ");
            g_uart2.send_string(line);
            g_uart2.send_string("\r\n");
        }
    }
}