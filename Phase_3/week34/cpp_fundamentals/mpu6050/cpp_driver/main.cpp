#include "real_i2c.hpp"
#include "mpu6050.hpp"

int main() {
    RealI2c i2c;
    Mpu6050<RealI2c> mpu(MPU6050_ADDR_ADO_LOW, i2c);

    auto init_result = mpu.init();
    if (!init_result) {
        printf("init failed, error code: %d\n",
                    static_cast<int>(init_result.error()));
        return 1;
    }
    printf("init OK\n");

    auto who_am_i = mpu.read_who_am_i();
    if (!who_am_i) {
        printf("read_who_am_i failed, error code: %d\n",
                    static_cast<int>(who_am_i.error()));
        return 1;
    }
    printf("WHO_AM_I = 0x%02X\n", *who_am_i);

    auto data = mpu.read_data();
    if (!data) {
        printf("read_data failed, error code: %d\n",
                    static_cast<int>(data.error()));
        return 1;
    }
    printf("accel: %d %d %d  gyro: %d %d %d\n",
                data->accel_x, data->accel_y, data->accel_z,
                data->gyro_x, data->gyro_y, data->gyro_z);

    return 0;
}