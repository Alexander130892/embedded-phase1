# Week 15 — I2C Fundamentals + BMP280 Driver

## Objective
Implement a bare-metal I2C driver on STM32F446RE and verify communication with a BMP280 pressure/temperature sensor by reading the WHO_AM_I register (0xD0 → expected 0x58).

## Hardware
- STM32F446RE Nucleo board
- BMP280 breakout module (GY-BM E/P 280, I2C address 0x76, SDO pulled low onboard)
- External 4.7 kΩ pull-up resistors on SCL and SDA

## Pin Assignment
| Signal | Pin  |
|--------|------|
| SCL    | PB8  |
| SDA    | PB9  |

> PB6/PB7 were originally considered but conflicted with the existing SPI1_CS assignment. Moved to PB8/PB9 (also AF4, same peripheral).

## Driver Architecture
```
[ bmp280.c ]           ← sensor logic, register addresses
       ↓
[ i2c.c / i2c.h ]     ← generic transport layer
       ↓
[ STM32 I2C1 peripheral ]
```

## I2C Driver API
```c
void    i2c_start();
void    i2c_restart();                                          // No BUSY check — bus already owned
void    i2c_stop();
void    i2c_write_addr(uint8_t addr);                          // Handles SB + ADDR flags
void    i2c_write_byte(uint8_t data);                          // Polls TXE
uint8_t i2c_read_byte(void);                                   // Clears ACK, polls RXNE, generates STOP
void    i2c_write_register(uint8_t dev_addr, uint8_t reg, uint8_t data);
uint8_t i2c_read_register(uint8_t dev_addr, uint8_t reg);
```

### Register read transaction sequence
```
START → addr+W → reg → REPEATED START → addr+R → read → STOP
```

## Timing Configuration (Standard Mode 100 kHz, APB1 = 16 MHz)
| Register | Value | Formula |
|----------|-------|---------|
| CR2 FREQ | 0x10  | APB1 clock in MHz |
| CCR      | 0x50  | f_APB1 / (2 × f_I2C) = 80 |
| TRISE    | 0x11  | (t_rise_max × f_APB1) + 1 = 17 |

## Result
BMP280 WHO_AM_I returns `0x58` ✓

---

## Lessons Learned

### 1. OTYPER and PUPDR both required for I2C GPIO
Open-drain (OTYPER) prevents the pin from driving high — but that leaves the line floating when released. Pull-ups (PUPDR or external) define a known idle state (high). Both are needed. External 4.7 kΩ resistors are more reliable than internal STM32 pull-ups (~40–50 kΩ) especially at 100 kHz.

### 2. SWRST needed on I2C init
If a previous run was interrupted mid-transaction, the I2C peripheral can boot with the BUSY flag stuck high. Adding a software reset at the start of `i2c_init()` clears this reliably:
```c
I2C1_CR1 |= I2C_CR1_SWRST;
I2C1_CR1 &= ~I2C_CR1_SWRST;
```

### 3. SB is cleared by writing DR, not by reading SR1
After generating START and waiting for SB, SB is cleared by the act of writing the address to DR. This is why address transmission belongs in `i2c_write_addr()`, not `i2c_start()`.

### 4. ADDR flag clearing sequence must happen after address transmission
After the address byte is ACKed, ADDR must be cleared by reading SR1 then SR2 in sequence. If this is skipped, the peripheral holds SCL low indefinitely (clock stretching), and subsequent TXE/BTF flags never appear.

### 5. i2c_start() vs i2c_restart()
`i2c_start()` checks the BUSY flag before generating START — correct for the beginning of a new transaction. For a repeated START (mid-transaction), BUSY is intentionally set. Using `i2c_start()` for the repeated START causes an infinite wait. Solution: separate `i2c_restart()` function that skips the BUSY check.

### 6. `i2c_write_addr()` is separate from `i2c_write_byte()`
`i2c_write_byte()` polls TXE before writing DR. At the start of a transaction after START, TXE is not the right flag — the address transmission is triggered differently. Mixing address and data writes into the same function causes the driver to hang on TXE before the address is ever sent.

### 7. Use an I2C scanner to verify hardware before debugging software
When the driver hangs and it's unclear whether the issue is hardware or software, a scanner that polls `ADDR | AF` for each address (0x08–0x77) identifies whether any device is responding and at which address. This saved significant debug time.

### 8. Breadboard wiring causes intermittent failures
Loose connections on a breadboard can cause the driver to fail reproducibly in a way that looks like a software bug. Always verify continuity and re-seat wires before deep debugging.

### 9. Named bit definitions pay off on I2C
SR1 alone has 8+ flags polled in a typical read transaction. Named definitions (`I2C_SR1_ADDR`, `I2C_SR1_TXE`, etc.) map directly to RM terminology, making the driver readable and debuggable without constantly cross-referencing bit positions.

---

## Next — Week 16
- `i2c_read_burst()` for multi-byte reads
- BMP280 compensation parameters + raw temperature/pressure
- MPU6050 driver (WHO_AM_I + 6-axis burst read)
- Error recovery: ARLO, AF, BUSY flag handling