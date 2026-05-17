# Week 17 — sensor_logger

Multi-sensor polling data logger over UART. BMP280 (temperature + pressure) on SPI, MPU-6050 (accelerometer + gyroscope) on I²C. CSV output with millisecond timestamp, `log on` / `log off` command parser.

Seed project for the Week 18 FreeRTOS port.

---

## Hardware

| Signal       | Pin  | Header |
|--------------|------|--------|
| SPI1_SCK     | PA5  | D13    |
| SPI1_MISO    | PA6  | D12    |
| SPI1_MOSI    | PA7  | D11    |
| BMP280_CS    | PB6  | D10    |
| I2C1_SCL     | PB8  | D15    |
| I2C1_SDA     | PB9  | D14    |
| USART2_TX    | PA2  | D1     |
| USART2_RX    | PA3  | D0     |

**BMP280** — SPI, WHO_AM_I = 0x58  
**MPU-6050** — I²C address 0x68 (ADO low), WHO_AM_I = 0x68

---

## Project Structure

```
sensor_logger/
├── Inc/
│   ├── status.h          # Project-wide status_t enum
│   ├── spi.h
│   ├── i2c.h
│   ├── uart.h
│   ├── systick.h
│   ├── stm32f446re.h
│   ├── bmp280/
│   │   ├── bmp280.h          # Public interface
│   │   └── bmp280_internal.h # Register map, comp_params, private only
│   └── mpu6050/
│       ├── mpu6050.h
│       └── mpu6050_internal.h
├── Src/
│   ├── main.c
│   ├── spi.c
│   ├── i2c.c
│   ├── uart.c
│   ├── systick.c
│   ├── syscalls.c
│   ├── sysmem.c
│   ├── bmp280/
│   │   ├── bmp280_spi.c
│   │   ├── bmp280_i2c.c
│   │   └── bmp_core.c        # Compensation math, shared between transports
│   └── mpu6050/
│       └── mpu6050.c
├── Startup/
│   └── startup_stm32f446retx.s
├── STM32F446RETX_FLASH.ld
├── Makefile
└── README.md
```

---

## CSV Output Format

```
timestamp_ms, temp_int.frac, pressure_int.frac, ax, ay, az, gx, gy, gz
```

Example:
```
12368,16.54,1010.37,-0.26,0.27,-0.90,-2.65,0.06,0.14
```

- Temperature: fixed-point, divide by 100 for °C
- Pressure: fixed-point, divide by 100 for hPa
- Accel: scaled by 16384 LSB/g (±2g range)
- Gyro: scaled by 131 LSB/°/s (±250°/s range)
- MPU-6050 temperature registers (buff[6:7]) deliberately skipped — temperature sourced from BMP280

---

## UART Commands

| Command   | Effect                        |
|-----------|-------------------------------|
| `log on`  | Start CSV output              |
| `log off` | Stop CSV output, keep running |

Baud rate: 115200. Commands terminated by `\r` or `\n`.

---

## Build & Flash

Requires `arm-none-eabi-gcc` and `st-flash` (via `brew install stlink`).

```bash
make          # build
make flash    # flash via ST-Link
make clean    # remove build artifacts
make size     # show Flash/RAM usage
make disasm   # disassemble ELF
```

---

## Architecture Decisions

### CS management belongs in the device driver, not the transport layer
`spi.c` is bus-generic — it knows nothing about chip select. Each device driver (`bmp280_spi.c`) owns its CS pin via `cs_low()`/`cs_high()`. This scales cleanly to multiple SPI devices without coordination at the transport layer.

For FreeRTOS: the mutex protecting an SPI transaction must span CS low → CS high, owned by the device driver, not `spi.c`.

### Public/internal header split
`bmp280.h` — public interface only, safe to include anywhere.  
`bmp280_internal.h` — register map, compensation struct, static helper declarations. Never included outside the module.

### status_t lives above the protocol layer
A timeout is a timeout regardless of whether bytes travelled over SPI or I²C. `status.h` defines a single `status_t` enum used by all drivers. Avoids pulling `spi.h` into `bmp280.h`.

### Compensation math is transport-agnostic
`bmp_core.c` holds the BMP280 compensation formulas shared between SPI and I2C transports. `t_fine` and `comp_params` live there as file-scope statics, exposed to driver files only via `bmp280_internal.h`.

### SPI pushes complexity upward; I²C pushes it downward
`spi.c` is thin — the protocol has no addressing, ACK, or bus management. Complexity lives in the device driver (CS, R/W bit encoding, dummy bytes).  
`i2c.c` is dense — start/stop conditions, ACK, ARLO, bus recovery, 2-byte edge case all handled at the transport layer. Device drivers are correspondingly simpler.

This asymmetry becomes important for FreeRTOS mutex scope.

### Clock enables belong in the module that owns the peripheral
Each `xxx_init()` enables its own RCC clock. `main.c` never touches RCC. Prevents hidden coupling where callers must know which clocks to enable before calling init.

### GPIO init is separate from peripheral init
`spi_gpio_init()` and `spi_init()` are separate functions, both called explicitly by the user. Same for I²C and UART. Keeps pin configuration visible and changeable without touching peripheral logic.

---

## Lessons Learned

**Wiring before debugging code.** Two separate wiring bugs consumed significant debug time — I²C SCL connected to SPI SCK (same wire colour), and CS pin MODER never set to output. Verify continuity and pin modes before suspecting software.

**ODR going low in the debugger does not mean the pin is driving low.** If MODER is wrong (input mode), the ODR bit is set but the pin is high-impedance. Always verify MODER alongside ODR.


**`_printf_float` vs `_print_float`.** A single missing `f` silently produces empty float fields in `snprintf` output with no compiler warning. If floats print as blank, check the linker flag spelling first.

**static functions need forward declarations in C.** Unlike C++, calling a `static` function before its definition in the same `.c` file produces an implicit declaration error. Add forward declarations at the top of the file.


**SysTick timestamp continuity across log on/off.** `tick_ms` keeps incrementing regardless of log state. Timestamps remain monotonic even when logging is paused — useful for offline analysis.

---

## Known Limitations / Week 18 TODO

- `log on`/`log off` UX is poor in polling loop — UART input and CSV output share the same thread and interfere. FreeRTOS will fix this with separate tasks.
- Timeout on I²C bus left without bus reset on mid-transaction failure — flagged with comment in `i2c_read_burst()`. Full cleanup deferred.
- DMA SPI functions (`init_dma_rx`, `init_dma_tx`, `spi_dma_read`, `spi_dma_write`, ISRs) parked in `spi.c` — will be activated for FreeRTOS port.
- SysTick `tick_ms` will overflow after ~49 days. Unsigned subtraction handles this correctly for interval timing but absolute timestamps will wrap.
- Busy-wait delays in `mpu6050_init()` should be replaced with `vTaskDelay()` in Week 18.