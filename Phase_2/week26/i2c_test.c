
#include <stdint.h>
#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#define I2C_ADDR_ADD0_LOW   0x68
#define I2C_WHOAMI          0x75

int main(void){
    int i2c_instance = open("/dev/i2c-1", O_RDWR);
    if (i2c_instance < 0) {
        perror("open");
        return 1;
    }   
    uint8_t whoami_addr = I2C_WHOAMI;
    uint8_t reply=0;

    struct i2c_msg whoami_write={
        I2C_ADDR_ADD0_LOW,
        0,
        1,
        &whoami_addr
    };
    struct i2c_msg whoami_read={
        I2C_ADDR_ADD0_LOW,
        I2C_M_RD,
        1,
        &reply
    };
    struct i2c_msg i2c_call[2]={whoami_write,whoami_read};
    struct i2c_rdwr_ioctl_data i2c_data ={i2c_call, 2};
    int i2c_ret_status = ioctl(i2c_instance, I2C_RDWR, &i2c_data);
    if (i2c_ret_status < 0) {
        perror("i2c ret status");
        return 1;
    }  
    printf("MPU6050 WHOAMI: 0x%x\r\n", reply);

    close(i2c_instance);
}