#include <cstdint>

#include "bsp.hpp"
#include "fixed_format.hpp"
#include "i2c.h"            // C API: i2c_init_gpio(), i2c_init()
#include "i2c_bus.hpp"      // C++ adapter: CI2cBus, BusStatus
#include "uart.hpp"

constinit Uart g_uart2{0x4000'4400u};   // USART2

namespace {

constexpr std::uint8_t kMpu6050Addr   = 0x68;   // 7-bit, AD0 low
constexpr std::uint8_t kRegWhoAmI     = 0x75;
constexpr std::uint8_t kWhoAmIExpect  = 0x68;

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

void print_number(std::int32_t value)
{
    char num[16];
    format_fixed(num, sizeof num, value, 0);
    g_uart2.send_string(num);
}

} // namespace

int main()
{
    bsp_init();          // PA2/PA3 → AF7 (USART2)
    g_uart2.init();      // clock, 115200 8N1, RXNE IRQ

    // Startup: proves TX + format_fixed on the target
    char num[16];
    format_fixed(num, sizeof num, -150, 2);
    g_uart2.send_string("CppSensorLogger up, format_fixed(-150, 2) = ");
    g_uart2.send_string(num);                 // expect "-1.50"
    g_uart2.send_string("\r\n");

    // I2C link check: C driver (i2c.c) called through the C++ adapter
    (void)i2c_init_gpio();                    // Week 17 C driver: I2C1 pins + clock
    (void)i2c_init();                         // I2C1 100 kHz

    CI2cBus bus;
    std::uint8_t who_am_i = 0;
    const BusStatus st = bus.read_burst(kMpu6050Addr, kRegWhoAmI, &who_am_i, 1);

    g_uart2.send_string("MPU-6050 WHO_AM_I: status=");
    g_uart2.send_string(to_string(st));
    g_uart2.send_string(", value=");
    print_number(who_am_i);                   // decimal: expect 104 (0x68)
    g_uart2.send_string(who_am_i == kWhoAmIExpect ? "  OK\r\n" : "  MISMATCH\r\n");

    // Echo: proves RX ISR → ring_buffer → receive_line
    char line[Uart::kMaxLineLen + 1];
    for (;;) {
        if (g_uart2.receive_line(line, sizeof line)) {
            g_uart2.send_string("> ");
            g_uart2.send_string(line);
            g_uart2.send_string("\r\n");
        }
    }
}