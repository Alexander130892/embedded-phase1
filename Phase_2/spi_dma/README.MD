# Week 14 — SPI: DMA + Multi-byte Transfers

Bare-metal STM32F446RE project — no HAL, no CMSIS. All register access via raw addresses defined in `stm32f446re.h`.

## Overview

Two SPI peripherals sharing a single SPI1 bus with independent chip selects:
- **BMP280** — temperature/pressure sensor (DMA RX burst read)
- **SH1107** — 128×128 OLED display (DMA TX framebuffer push)

Sensor data is read via DMA, compensated using factory calibration, and rendered on the OLED.

## Pin Mapping

| Function     | Pin  | Notes                        |
|-------------|------|------------------------------|
| SPI1_SCK    | PA5  | Shared clock                 |
| SPI1_MISO   | PA6  | BMP280 SDO                   |
| SPI1_MOSI   | PA7  | Shared data out              |
| BMP280 CS   | PB6  | Active low                   |
| OLED CS     | PB4  | Active low                   |
| OLED DC     | PB5  | Low = command, High = data   |
| OLED RES    | PB3  | Active low reset             |
| USART2 TX   | PA2  | Debug UART output            |

## DMA Configuration

### DMA2 Stream 0, Channel 3 — SPI1 RX (BMP280)
- Direction: peripheral → memory
- MINC enabled, PINC fixed
- 8-bit transfers
- Transfer complete interrupt → `DMA2_Stream0_IRQHandler`
- NVIC position 56 (ISER1 bit 24)

### DMA2 Stream 3, Channel 3 — SPI1 TX (SH1107)
- Direction: memory → peripheral
- MINC enabled, PINC fixed
- 8-bit transfers
- Transfer complete interrupt → `DMA2_Stream3_IRQHandler`
- NVIC position 59 (ISER1 bit 27)

## File Structure

```
week14/spi_dma/
├── Inc/
│   ├── stm32f446re.h     — Register addresses and offsets
│   ├── spi.h             — SPI, DMA RX/TX, transfer functions
│   ├── bmp280.h          — Sensor registers, calibration struct
│   ├── sh1107.h          — OLED commands, draw functions
│   ├── font8x8.h         — 8×8 bitmap font (ASCII 32–127)
│   └── uart.h            — UART debug output
├── Src/
│   ├── main.c            — Init sequence, main loop
│   ├── spi.c             — SPI init, DMA RX/TX, ISRs
│   ├── bmp280.c          — Calibration read, temp/pressure compensation
│   ├── sh1107.c          — OLED init, commands, framebuffer write
│   └── uart.c            — UART send functions
└── README.md
```

## BMP280 Sensor

### Calibration
12 factory-programmed coefficients read at boot from registers `0x88`–`0x9F` via blocking SPI. Stored in `comp_param_t` struct.

### Temperature
- Burst read 3 registers from `0xFA` (20-bit raw value)
- 32-bit integer compensation formula from datasheet
- Output in 0.01 °C units (e.g. 1935 = 19.35 °C)

### Pressure
- Burst read 3 registers from `0xF7` (20-bit raw value)
- 32-bit integer compensation formula (requires `t_fine` from temperature)
- Output in Pa (e.g. 101372 = 1013.72 hPa)

## SH1107 OLED Display

### Init Sequence
Hardware reset pulse → display off → clock/multiplex/offset/contrast/DC-DC/pre-charge/VCOM → display on.

### Framebuffer
2048 bytes (16 pages × 128 columns). Pages are vertical strips on this module. Text rendered via 8×8 font into framebuffer, pushed to display via DMA TX (128 bytes per page, 16 pages).

## Lessons Learned

### LISR/LIFCR bit positions
Stream 0 flags are at bits 5:2, Stream 3 flags are at bits 27:24. Confusing channel number (3) with stream number (0) caused the ISR to clear the wrong flags — transfer completed but interrupt never fired.

### SPI DR flush after DMA TX
Full-duplex SPI receives data on MISO during TX. After DMA TX (OLED write), unread bytes and OVR flag in SPI DR corrupt subsequent blocking reads (BMP280 calibration). Fix: read DR + SR to flush before switching to RX operations.

### M0AR reload between transfers
In non-circular mode, M0AR is not automatically reset after transfer. Must reload M0AR in the transfer function before re-enabling DMA, otherwise data writes to wrong memory location on subsequent calls.

### Dummy byte count for DMA RX
Sending N dummy bytes with NDTR=N captures N+1 bytes (including the address response). Causes stale byte in DR that shifts data on subsequent transfers. Fix: send N-1 dummy bytes.

### NVIC enable
Peripheral-side interrupt enable (TCIE in DMA SxCR) is not sufficient. Must also enable the corresponding interrupt in the NVIC ISER register.

## Known Issues

- **OLED flickering**: display shows visible refresh/flicker. Suspected charge pump or panel timing issue specific to this module. All software tuning attempted (clock divide, contrast, pre-charge, VCOM, DC-DC values). Functionally correct — data renders and updates properly.

## UART Output

```
Ready for Testing:
BMP280 OK: 0x58
Temperature: 19.35C
Pressure: 1013.72hPa
```