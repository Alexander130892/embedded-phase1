#include "unity.h"
#include "mpu6050.hpp"

#include <array>
#include <cstdint>

// -----------------------------------------------------------------------------
// FakeBus: records every write, serves canned bytes for reads, and can be told
// to fail on the Nth call. Satisfies I2cBusLike, so Mpu6050<FakeBus> compiles.
// -----------------------------------------------------------------------------
struct FakeBus {
    struct Write { std::uint8_t dev, reg, val; };

    std::array<Write, 8>          writes{};
    std::size_t                   write_count = 0;

    std::array<std::uint8_t, 14>  read_data{};
    std::uint8_t                  last_read_dev = 0, last_read_reg = 0, last_read_len = 0;
    std::size_t                   read_count = 0;

    std::size_t                   fail_on_call = 0;          // 1-based; 0 = never fail
    BusStatus                     fail_status  = BusStatus::Nack;
    std::size_t                   calls        = 0;

    BusStatus write_reg(std::uint8_t dev, std::uint8_t reg, std::uint8_t val)
    {
        if (++calls == fail_on_call) { return fail_status; }
        if (write_count < writes.size()) { writes[write_count] = {dev, reg, val}; }
        ++write_count;
        return BusStatus::Ok;
    }

    BusStatus read_burst(std::uint8_t dev, std::uint8_t reg, std::uint8_t* buf, std::uint8_t len)
    {
        if (++calls == fail_on_call) { return fail_status; }
        last_read_dev = dev; last_read_reg = reg; last_read_len = len;
        ++read_count;
        for (std::uint8_t i = 0; i < len && i < read_data.size(); ++i) { buf[i] = read_data[i]; }
        return BusStatus::Ok;
    }
};
static_assert(I2cBusLike<FakeBus>);

static constexpr std::uint8_t kAddr = 0x68;

void setUp(void) {}
void tearDown(void) {}

// --- init() -------------------------------------------------------------------
static void test_init_writes_sequence_in_order(void)
{
    FakeBus bus;
    Mpu6050<FakeBus> imu{bus, kAddr};

    TEST_ASSERT_TRUE(imu.init() == BusStatus::Ok);
    TEST_ASSERT_EQUAL_UINT(4, bus.write_count);

    const FakeBus::Write expected[4] = {
        {kAddr, 0x6B, 0x00},   // PWR_MGMT_1: wake
        {kAddr, 0x19, 0x07},   // SMPLRT_DIV
        {kAddr, 0x1B, 0x00},   // GYRO_CONFIG: ±250 °/s
        {kAddr, 0x1C, 0x00},   // ACCEL_CONFIG: ±2 g
    };
    for (std::size_t i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL_HEX8(expected[i].dev, bus.writes[i].dev);
        TEST_ASSERT_EQUAL_HEX8(expected[i].reg, bus.writes[i].reg);
        TEST_ASSERT_EQUAL_HEX8(expected[i].val, bus.writes[i].val);
    }
}

static void test_init_stops_at_first_failure_and_returns_it(void)
{
    FakeBus bus;
    bus.fail_on_call = 2;                       // SMPLRT_DIV write fails
    bus.fail_status  = BusStatus::Timeout;
    Mpu6050<FakeBus> imu{bus, kAddr};

    TEST_ASSERT_TRUE(imu.init() == BusStatus::Timeout);
    TEST_ASSERT_EQUAL_UINT(1, bus.write_count); // only PWR_MGMT_1 went through
    TEST_ASSERT_EQUAL_UINT(2, bus.calls);       // nothing attempted after the failure
}

// --- who_am_i() ---------------------------------------------------------------
static void test_who_am_i_reads_register_0x75(void)
{
    FakeBus bus;
    bus.read_data[0] = 0x68;
    Mpu6050<FakeBus> imu{bus, kAddr};

    std::uint8_t id = 0;
    TEST_ASSERT_TRUE(imu.who_am_i(id) == BusStatus::Ok);
    TEST_ASSERT_EQUAL_HEX8(0x68, id);
    TEST_ASSERT_EQUAL_HEX8(kAddr, bus.last_read_dev);
    TEST_ASSERT_EQUAL_HEX8(0x75, bus.last_read_reg);
    TEST_ASSERT_EQUAL_UINT8(1, bus.last_read_len);
}

// --- read(): transaction ------------------------------------------------------
static void test_read_is_one_14_byte_burst_from_accel_xout_h(void)
{
    FakeBus bus;
    Mpu6050<FakeBus> imu{bus, kAddr};
    ImuSample s{};

    TEST_ASSERT_TRUE(imu.read(s) == BusStatus::Ok);
    TEST_ASSERT_EQUAL_UINT(1, bus.read_count);
    TEST_ASSERT_EQUAL_HEX8(kAddr, bus.last_read_dev);
    TEST_ASSERT_EQUAL_HEX8(0x3B, bus.last_read_reg);
    TEST_ASSERT_EQUAL_UINT8(14, bus.last_read_len);
}

// --- read(): decoding and scaling ---------------------------------------------
static void test_read_decodes_and_scales_all_axes(void)
{
    FakeBus bus;
    bus.read_data = {
        0x40, 0x00,   // accel X = +16384 → +1000 mg
        0xC0, 0x00,   // accel Y = -16384 → -1000 mg
        0xFF, 0x38,   // accel Z =   -200 →   -12 mg (truncated toward zero)
        0x12, 0x34,   // temperature: must be ignored
        0x00, 0x83,   // gyro X  =   +131 → +1000 mdps
        0x80, 0x00,   // gyro Y  = -32768 → -250137 mdps (full scale, no overflow)
        0x7F, 0xFF,   // gyro Z  = +32767 → +250129 mdps
    };
    Mpu6050<FakeBus> imu{bus, kAddr};
    ImuSample s{};

    TEST_ASSERT_TRUE(imu.read(s) == BusStatus::Ok);
    TEST_ASSERT_EQUAL_INT32( 1000,    s.accel_x_mg);
    TEST_ASSERT_EQUAL_INT32(-1000,    s.accel_y_mg);
    TEST_ASSERT_EQUAL_INT32(  -12,    s.accel_z_mg);
    TEST_ASSERT_EQUAL_INT32( 1000,    s.gyro_x_mdps);
    TEST_ASSERT_EQUAL_INT32(-250137,  s.gyro_y_mdps);
    TEST_ASSERT_EQUAL_INT32( 250129,  s.gyro_z_mdps);
}

static void test_read_accel_extremes(void)
{
    FakeBus bus;
    bus.read_data = {0x80,0x00, 0x7F,0xFF, 0x00,0x00, 0,0, 0,0, 0,0, 0,0};
    Mpu6050<FakeBus> imu{bus, kAddr};
    ImuSample s{};

    TEST_ASSERT_TRUE(imu.read(s) == BusStatus::Ok);
    TEST_ASSERT_EQUAL_INT32(-2000, s.accel_x_mg);   // -32768 → exactly -2 g
    TEST_ASSERT_EQUAL_INT32( 1999, s.accel_y_mg);   // +32767 → 1999.9 mg, truncated
    TEST_ASSERT_EQUAL_INT32(    0, s.accel_z_mg);
}

// --- read(): error path -------------------------------------------------------
static void test_read_error_returns_status_and_leaves_sample_untouched(void)
{
    FakeBus bus;
    bus.fail_on_call = 1;
    bus.fail_status  = BusStatus::Nack;
    Mpu6050<FakeBus> imu{bus, kAddr};
    ImuSample s{11, 22, 33, 44, 55, 66};

    TEST_ASSERT_TRUE(imu.read(s) == BusStatus::Nack);
    TEST_ASSERT_EQUAL_INT32(11, s.accel_x_mg);
    TEST_ASSERT_EQUAL_INT32(22, s.accel_y_mg);
    TEST_ASSERT_EQUAL_INT32(33, s.accel_z_mg);
    TEST_ASSERT_EQUAL_INT32(44, s.gyro_x_mdps);
    TEST_ASSERT_EQUAL_INT32(55, s.gyro_y_mdps);
    TEST_ASSERT_EQUAL_INT32(66, s.gyro_z_mdps);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_init_writes_sequence_in_order);
    RUN_TEST(test_init_stops_at_first_failure_and_returns_it);

    RUN_TEST(test_who_am_i_reads_register_0x75);

    RUN_TEST(test_read_is_one_14_byte_burst_from_accel_xout_h);
    RUN_TEST(test_read_decodes_and_scales_all_axes);
    RUN_TEST(test_read_accel_extremes);
    RUN_TEST(test_read_error_returns_status_and_leaves_sample_untouched);

    return UNITY_END();
}