# Week 13 — SPI Fundamentals + BMP280 Driver

Bare-metal SPI implementation on STM32F446RE (Nucleo-F446RE), communicating with a BMP280 barometric pressure sensor. No HAL, no CubeMX — direct register access only.

## Hardware

| Component | Details |
|---|---|
| MCU | STM32F446RE (Nucleo-F446RE) |
| Sensor | BMP280 barometric pressure + temperature |
| Interface | SPI1, Mode 0 (CPOL=0, CPHA=0) |
| Baud rate | 250 kHz (PCLK/64, HSI 16 MHz) |

## Pin Mapping

| Signal | Nucleo Pin | BMP280 Pin |
|---|---|---|
| SCK | PA5 (AF5) | SCL |
| MISO | PA6 (AF5) | SDO |
| MOSI | PA7 (AF5) | SDA |
| CS | PB6 (GPIO out) | CS |

CS is managed in software (SSM=1, SSI=1 in SPI1_CR1).

## File Structure

```
week13/spi_basic/
├── Core/
│   ├── main.c               # Entry point, init sequence, read loop
│   ├── spi.c / spi.h        # STM32 SPI1 peripheral driver
│   ├── bmp280.c / bmp280.h  # BMP280 sensor driver
|   |-- uart.c / uart.h      # UART for debugging/testing
│   └── stm32f4_registers.h  # Bare-metal register definitions
```

## Driver Architecture

```
main.c
  └── bmp280.c       ← sensor protocol layer (CS management, RW bit, dummy bytes)
        └── spi.c    ← STM32 peripheral layer (CR1, SR, DR register access)
```

`spi.c` is sensor-agnostic — any SPI device can be added without modifying it.

## Key Implementation Notes

- **SPI mode**: Mode 0 (CPOL=0, CPHA=0), also compatible with Mode 3 per BMP280 datasheet
- **Baud rate**: PCLK/64 = 250 kHz — conservative for bring-up (BMP280 max: 10 MHz)
- **NSS**: Software managed via PB6 GPIO — CS idles high, pulled low per transaction
- **BSY flag**: Checked after final transfer before deasserting CS to prevent byte clipping
- **Read protocol**: Address byte with bit 7 set (`addr | 0x80`), followed by dummy `0x00` to clock in response
- **Write protocol**: Address byte with bit 7 cleared (`addr & ~0x80`), followed by data byte

## Verification

WHO_AM_I register (`0xD0`) read on boot — expected return value: `0x58`

Verified via:
- STM32CubeIDE debugger (breakpoint on return value)
- UART
- Logic analyser (PulseView): MOSI `0xD0 0x00` → MISO `0x00 0x58` --> See screenshot

## Checkpoint M7

- [x] SPI1 configured bare-metal (RCC, GPIO AF, CR1, CR2)
- [x] `spi_send_byte()` and `spi_receive_byte()` implemented — polling TXE/RXNE
- [x] `spi_transfer()` full-duplex implemented
- [x] `bmp280_read_register()` and `bmp280_write_register()` implemented
- [x] WHO_AM_I register read returns `0x58` — sensor comms verified