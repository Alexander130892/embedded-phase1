#include <cstdio>
#include <cassert>
#include "fake_i2c.hpp"
#include "mpu6050.hpp"

// ---- Custom fake bodies ----

int custom_read_who_am_i(uint8_t dev_addr, uint8_t reg, uint8_t* out_value) {
    *out_value = 0x68;
    return 0;
}

int custom_read_burst(uint8_t dev_addr, uint8_t reg, uint8_t* buf, size_t len) {
    // ax=0x0102, ay=0x0304, az=0x0506, [pad,pad], gx=0x0708, gy=0x090A, gz=0x0B0C
    uint8_t pattern[14] = {0x01,0x02, 0x03,0x04, 0x05,0x06, 0x00,0x00,
                            0x07,0x08, 0x09,0x0A, 0x0B,0x0C};
    for (size_t i = 0; i < len; ++i) {
        buf[i] = pattern[i];
    }
    return 0;
}

// ---- Tests ----

void test_init_success() {
    RESET_FAKE(fake_write_register);
    fake_write_register_fake.return_val = 0;   // every write succeeds

    FakeI2c i2c;
    Mpu6050<FakeI2c> mpu(0x68, i2c);

    auto result = mpu.init();
    assert(result.has_value());
    assert(fake_write_register_fake.call_count == 4);  // PWR_MGMT_1, SMPLRT_DIV, GYRO_CONFIG, ACCEL_CONFIG

    std::printf("test_init_success: PASS\n");
}

void test_init_propagates_failure() {
    RESET_FAKE(fake_write_register);
    fake_write_register_fake.return_val = 1;   // first write fails

    FakeI2c i2c;
    Mpu6050<FakeI2c> mpu(0x68, i2c);

    auto result = mpu.init();
    assert(!result.has_value());
    assert(result.error() == ErrorCode::UNKNOWN_ERROR);
    assert(fake_write_register_fake.call_count == 1);  // stopped after first failure, didn't try the rest

    std::printf("test_init_propagates_failure: PASS\n");
}

void test_read_who_am_i() {
    RESET_FAKE(fake_read_register);
    fake_read_register_fake.custom_fake = custom_read_who_am_i;

    FakeI2c i2c;
    Mpu6050<FakeI2c> mpu(0x68, i2c);

    auto result = mpu.read_who_am_i();
    assert(result.has_value());
    assert(*result == 0x68);
    assert(fake_read_register_fake.call_count == 1);
    assert(fake_read_register_fake.arg0_val == 0x68);        // dev_addr passed through
    assert(fake_read_register_fake.arg1_val == MPU6050_WHO_AM_I);

    std::printf("test_read_who_am_i: PASS\n");
}

void test_read_data() {
    RESET_FAKE(fake_read_burst);
    fake_read_burst_fake.custom_fake = custom_read_burst;

    FakeI2c i2c;
    Mpu6050<FakeI2c> mpu(0x68, i2c);

    auto result = mpu.read_data();
    assert(result.has_value());
    assert(result->accel_x == 0x0102);
    assert(result->accel_y == 0x0304);
    assert(result->accel_z == 0x0506);
    assert(result->gyro_x  == 0x0708);
    assert(result->gyro_y  == 0x090A);
    assert(result->gyro_z  == 0x0B0C);

    std::printf("test_read_data: PASS\n");
}

int main() {
    test_init_success();
    test_init_propagates_failure();
    test_read_who_am_i();
    test_read_data();

    std::printf("All tests passed.\n");
    return 0;
}