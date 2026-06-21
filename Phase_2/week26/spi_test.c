/*
 * File:    spi_test.c
 * Author:  Alexander130892
 * Date:    21-6-2026
 *
 * Description:
 *   This program opens the SPI device `/dev/spidev0.0`, configures it
 *   to SPI mode 0 at 500 kHz, and sends a read command to query the
 *   WHOAMI register (0xD0) of a BMP280 sensor, printing the received
 *   chip ID. It demonstrates basic SPI communication on Linux using
 *   ioctl system calls.
 */


#include <stdint.h>
#include <stdio.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#define SPI_WHOAMI          0xD0
#define SPI_READBIT_MASK    0x80
#define SPI_DUMMYBYTE       0x00

int main(void){
    int spi_instance = open("/dev/spidev0.0", O_RDWR);
    if (spi_instance < 0) {
        perror("open");
        return 1;
    }   
    // set SPI mode
    uint8_t mode = SPI_MODE_0;
    int spi_mode_ret_status = ioctl(spi_instance, SPI_IOC_WR_MODE, &mode);
    if (spi_mode_ret_status < 0) {
        perror("spi_mode_ret_status");
        return 1;
    } 
    uint32_t speed = 500000;  // 500 kHz, conservative
    int spi_speed_ret_status = ioctl(spi_instance, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (spi_speed_ret_status < 0) {
        perror("spi_speed_ret_status");
        return 1;
    }
    //Create message
    uint8_t reg_addr = SPI_WHOAMI;
    uint8_t tx[2] = { reg_addr | SPI_READBIT_MASK, SPI_DUMMYBYTE };  // read bit set, dummy byte
    uint8_t rx[2] = { SPI_DUMMYBYTE, SPI_DUMMYBYTE };
    struct spi_ioc_transfer transfer = {0};  // zero-initialize all fields first
    transfer.tx_buf = (uintptr_t)tx;
    transfer.rx_buf = (uintptr_t)rx;
    transfer.len = 2;
    // Message
    int spi_ret_status = ioctl(spi_instance, SPI_IOC_MESSAGE(1), &transfer);
    if (spi_ret_status < 0) {
        perror("spi ret status");
        return 1;
    }  
    printf("BMP280 WHOAMI: 0x%x\r\n", rx[1]);

    close(spi_instance);
}