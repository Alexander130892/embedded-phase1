// Src/main.cpp — wiring: drivers -> BoardHw -> SensorFsm -> Subject -> UartLogger -> USART2
//
//   systick (1 s) Tick
//   UART "log on/off" > SensorFsm<BoardHw> -- publish(LogRecord)> Subject > UartLogger > USART2
//                         │          │
//                         │           BoardHw: Mpu6050<I2c1Bus> (C++), BMP280 + I2C/SPI/systick (Week 17 C)
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "bmp280.h"        // Week 17 C driver (extern "C")
#include "bsp.hpp"
#include "i2c.h"           // Week 17 C driver (extern "C")
#include "i2c_bus.hpp"     // I2c1Bus adapter
#include "log_record.hpp"
#include "mpu6050.hpp"
#include "sensor_fsm.hpp"
#include "sensor_hw.hpp"
#include "spi.h"           // Week 17 C driver (extern "C")
#include "subject.hpp"
#include "systick.h"       // Week 17 C driver — needs extern "C" guards, see note
#include "uart.hpp"
#include "uart_logger.hpp"

constinit Uart g_uart2{0x4000'4400u};   // USART2 — file scope, external linkage (the ISR uses it)

namespace {

constexpr std::uint32_t kTickPeriodMs = 1000;   // one LogRecord per second, as in Week 17
constexpr std::size_t   kMaxObservers = 2;      // UartLogger + one spare

// ---------------------------------------------------------------------------
// BoardHw: the real implementation of the SensorHw port.
// The only place where the FSM's world meets the actual drivers.
// ---------------------------------------------------------------------------
struct BoardHw {
    Mpu6050<I2c1Bus>& imu;

    BusStatus imu_init()
    {
        // forward to the Mpu6050
        return imu.init();
    }

    BusStatus imu_read(ImuSample& s)
    {
        // Forward to the Mpu6050
        return imu.read(s);;
    }

    bool env_read(EnvSample& e)
    {
        // Both Week 17 C calls must return STATUS_OK
        return(bmp280_spi_read_temp(&e.temp_centi_c) == STATUS_OK && bmp280_spi_read_pressure(&e.press_pa) == STATUS_OK);
    }
    BusStatus bus_reset()
    {
        return (i2c_bus_reset() == STATUS_OK) ? BusStatus::Ok : BusStatus::Timeout;
    }

    std::uint32_t now_ms()
    {
        // Week 17 systick
        return systick_get_ms();
    }
};
static_assert(SensorHw<BoardHw>, "BoardHw must satisfy the SensorHw port");

using Records = Subject<LogRecord, kMaxObservers>;
using Fsm     = SensorFsm<BoardHw, kMaxObservers>;

const char* to_string(State s)
{
    switch (s) {
        case State::Init:       return "Init";
        case State::Logging:    return "Logging";
        case State::Idle:       return "Idle";
        case State::Recovering: return "Recovering";
    }
    return "?";
}

// Non-CSV lines start with '#'
void info(const char* a, const char* b = "")
{
    (void)g_uart2.send_string("# ");
    (void)g_uart2.send_string(a);
    (void)g_uart2.send_string(b);
    (void)g_uart2.send_string("\r\n");
}

void handle_command(const char* line, Fsm& fsm)
{
    if (std::strcmp(line, "log on") == 0) {
        fsm.handle(Event::LogOn);
    } else if (std::strcmp(line, "log off") == 0) {
        fsm.handle(Event::LogOff);
    } else if (std::strcmp(line, "status") == 0) {
        // falls through to the state report below
    } else {
        info("unknown command: ", line);
        return;
    }
    info("state: ", to_string(fsm.state()));
}

} // namespace

int main()
{
    // --- hardware bring-up (order as in Week 17) ---------------------------
    bsp_init();                     // PA2/PA3 -> USART2
    g_uart2.init();
    systick_init();                 // 1 ms tick

    (void)spi_gpio_init();          // SPI1 + BMP280 (C driver)
    (void)bmp280_spi_gpio_init();
    (void)spi_init();
    const status_t bmp_st = bmp280_spi_init();

    (void)i2c_init_gpio();          // I2C1 (C driver); the MPU-6050 itself is started by the FSM
    (void)i2c_init();

    // --- objects (locals of main: main never returns, so they live forever) ---
    I2c1Bus          bus;
    Mpu6050<I2c1Bus> imu{bus, Mpu6050<I2c1Bus>::kAddrAd0Low};
    BoardHw          hw{imu};

    Records    records;
    UartLogger logger{g_uart2};
    Fsm        fsm{hw, records};

    // --- wiring ---------------------------------------------------------------
    info("CppSensorLogger v1.0 - commands: log on | log off | status");
    if (bmp_st != STATUS_OK) {
        info("BMP280 init failed - env columns will be empty");
    }
    if (!records.subscribe(Records::Callback::create<UartLogger, &UartLogger::on_record>(logger))) {
        info("subscribe failed: increase kMaxObservers");
    }

    logger.print_header();
    fsm.start();                    // MPU init: Logging, or Recovering if the sensor is absent
    info("state: ", to_string(fsm.state()));

    // --- super-loop -------------------------------------------------------------
    std::uint32_t last_tick = systick_get_ms();
    char line[Uart::kMaxLineLen + 1];

    for (;;) {
        // Unsigned subtraction: still correct when the ms counter wraps after ~49.7 days.
        if (systick_get_ms() - last_tick >= kTickPeriodMs) {
            last_tick += kTickPeriodMs;   // += (not = now): no drift, ticks stay on a 1 s grid
            fsm.handle(Event::Tick);
        }
        if (g_uart2.receive_line(line, sizeof line)) {
            handle_command(line, fsm);
        }
    }
}