# Week 16 — I2C: Multi-byte Reads + Error Handling

**Time:** ~10 hrs  
**Hardware:** STM32F446RE Nucleo, BMP280 (pressure/temperature), MPU-6050 (6-axis IMU)  
**Repo path:** `week16/i2c_driver/`

---

## What Was Built

A bare-metal I2C driver stack for STM32F4, structured in clean layers:

```
main.c
  └── bmp280_i2c.c / mpu6050_i2c.c    (device drivers)
        └── i2c.c                      (transport library)
              └── stm32f446re.h        (register map)
```

Each layer only knows about the one below it. Device drivers call `i2c_read_burst()` — they never touch I2C registers directly. 

---

## The Two-Byte Read Problem

This is the most non-obvious part of the STM32 I2C peripheral and deserves a full explanation.

### Background: ACK/NACK timing

The hardware sends ACK/NACK on the **9th clock pulse** of each byte, then sets `RXNE`. By the time your polling loop wakes up and reads `DR`, the ACK/NACK is already on the wire. The hardware looks at `CR1.ACK` just before the 9th clock — not after.

### The pipeline

The I2C peripheral has a two-stage pipeline: a shift register and `DR`. When a byte finishes shifting in, it transfers to `DR` (triggering `RXNE`) and the shift register immediately starts receiving the next byte.

### Why N≥3 bytes is straightforward

For N bytes, use `BTF` at the N-2 boundary. At that point:
- Byte N-2 is in `DR`
- Byte N-1 is in the shift register
- **Byte N has not started yet**

Clearing `ACK` here gives the hardware a full byte-time before byte N's 9th clock. No race condition.

### Why 2-byte is non-obvious

With only 2 bytes, there is no "N-2 buffer." The moment `RXNE` fires for byte 1, byte 2 is **already being received in the shift register**. Clearing `ACK` at this point may be too late.

**Solution: the `POS` bit**

`POS=1` shifts the meaning of `ACK`: instead of controlling the byte currently in the shift register, it controls the *next* byte. Setting `POS=1` and `ACK=0` **before clearing `ADDR`** (before any bytes start arriving) guarantees the hardware has byte 2's NACK configured in time.

### The correct sequences

**Single byte:**
```c
I2C1_CR1 &= ~I2C_CR1_ACK;          // NACK the only byte
// clear ADDR
while(!(I2C1_SR1 & I2C_SR1_RXNE));
I2C1_CR1 |= I2C_CR1_STOP;          // STOP before reading DR
return I2C1_DR;
```

**Two bytes:**
```c
// Before clearing ADDR:
I2C1_CR1 |= I2C_CR1_POS;           // ACK controls next byte
I2C1_CR1 &= ~I2C_CR1_ACK;          // NACK byte 2
// clear ADDR (releases SCL, bytes start flowing)
while(!(I2C1_SR1 & I2C_SR1_BTF));  // both bytes received, SCL stretched
i2c_stop();
buf[0] = I2C1_DR;                   // read byte 1 (byte 2 auto-transfers to DR)
buf[1] = I2C1_DR;                   // read byte 2
I2C1_CR1 &= ~I2C_CR1_POS;          // restore POS
```

**N>2 bytes:**
```c
// clear ADDR, ACK=1 (default)
for (i = 0; i < len-2; i++) {
    while(!(I2C1_SR1 & I2C_SR1_RXNE));
    buf[i] = I2C1_DR;
}
// BTF endgame:
while(!(I2C1_SR1 & I2C_SR1_BTF));  // byte N-2 in DR, N-1 in shift register
I2C1_CR1 &= ~I2C_CR1_ACK;          // byte N not yet started — plenty of time
buf[len-2] = I2C1_DR;              // releases SCL, byte N-1 → DR, byte N starts
while(!(I2C1_SR1 & I2C_SR1_BTF));  // byte N-1 in DR, byte N in shift register
I2C1_CR1 |= I2C_CR1_STOP;
buf[len-1] = I2C1_DR;
I2C1_CR1 |= I2C_CR1_ACK;           // restore for next transaction
```

---

## Software Bus Reset

A stuck `BUSY` flag means a slave is holding `SDA` low mid-transfer. `SWRST` resets the STM32 peripheral but doesn't help the slave — it's still waiting for clock pulses to finish its byte.

**Solution: clock toggling**

```
1. SWRST — reset peripheral registers
2. Reconfigure SCL/SDA as GPIO output
3. Toggle SCL up to 9 times until SDA goes high
4. Manually generate STOP (SDA rises while SCL high)
5. Re-initialise peripheral with i2c_init()
```

9 pulses is the maximum needed — a slave can be mid-byte on the last bit of a full 8-bit transfer plus ACK. `i2c_bus_reset()` is called automatically from `i2c_start()` when `SR2.BUSY` is detected.

---

## Driver Architecture

### Files

| File | Purpose |
|---|---|
| `i2c.c` / `i2c.h` | Transport layer — `i2c_start`, `i2c_read_burst`, `i2c_bus_reset`, etc. |
| `bmp280/bmp280.h` | Public BMP280 API |
| `bmp280/bmp280_internal.h` | Private types, register map, shared prototypes |
| `bmp280/bmp280_core.c` | Compensation math, shared static state (`comp_params`, `t_fine`) |
| `bmp280/bmp280_i2c.c` | BMP280 I2C backend |
| `mpu6050/mpu6050.h` | Public MPU-6050 API |
| `mpu6050/mpu6050_internal.h` | Private types and register map |
| `mpu6050/mpu6050_i2c.c` | MPU-6050 I2C driver |

### Design decisions

- Register addresses never leak into public headers
- `comp_param_t` is private — loaded once in `init()`, stored as `static` in `bmp280_core.c`
- `i2c_init()` is the caller's responsibility — device drivers don't own the transport
- Single-master bus assumed — `ARLO` not handled (documented in `i2c.h`)

---

## Sensor Configuration

### BMP280
| Register | Value | Meaning |
|---|---|---|
| `CTRL_MEAS` (0xF4) | `0x93` | Oversampling ×8 T+P, normal mode |

### MPU-6050
| Register | Value | Meaning |
|---|---|---|
| `PWR_MGMT_1` (0x6B) | `0x00` | Wake from sleep, internal oscillator |
| `SMPLRT_DIV` (0x19) | `0x07` | 1 kHz sample rate |
| `GYRO_CONFIG` (0x1B) | `0x00` | ±250 °/s full scale |
| `ACCEL_CONFIG` (0x1C) | `0x00` | ±2g full scale |

**Sensitivity:**
- Accelerometer: 16384 LSB/g
- Gyroscope: 131 LSB/°/s

---

## Lessons Learned

### 1. Power cycle ≠ peripheral reset for stuck bus
A hung I2C transaction (mid-byte crash) leaves the slave holding `SDA` low. A processor reset doesn't fix this — the slave doesn't know the master reset. Only toggling SCL until the slave finishes its byte releases the bus. Always implement `i2c_bus_reset()`.

### 2. `POS` and `ACK` must be set before `ADDR` is cleared
`i2c_write_addr()` clears `ADDR` internally, releasing `SCL` and starting byte reception immediately. Setting `POS`/`ACK` after the call is too late for 2-byte reads. The 2-byte manual address phase must be inlined.

### 3. `__attribute__((packed))` required for `memcpy` into structs
Without packing, the compiler may insert padding bytes between struct fields, breaking a direct `memcpy` from a byte buffer. Always pack structs that map directly to hardware register blocks.

### 4. Wrong register address killed the MPU-6050 init
`PWR_MGMT_1` was `#define`d as `0x68` — the device's I2C address — instead of `0x6B`. The write returned success (the transaction completed without error) but landed in the wrong register. Lesson: always read back what you write during bring-up.

### 5. FPU must be enabled before any float operation
The Cortex-M4 FPU is disabled at reset. Using `sprintf` with `%f` without enabling it first causes a hard hang. Enable with a CPACR write at the very start of `main()`, before `init_clocks()`.

### 6. Big-endian sensor data on a little-endian MCU
Both BMP280 and MPU-6050 send multi-byte values MSB first. ARM is little-endian. Direct `memcpy` into a `uint16_t` reverses the bytes silently. Always reconstruct manually: `(MSB << 8) | LSB`.

### 7. Debugger masks timing bugs
Several hangs only appeared at full run speed — stepping through the debugger added enough delay between I2C transactions that the sensor had time to respond. Always test at full speed after debugger validation.

---

## Results

```
BMP280 OK: 0x60
Temperature: 15.82C
Pressure: 1007.55hPa

MPU6050 OK: 0x68
a_x: -0.47g  a_y: 0.60g  a_z: -0.56g
g_x: -0.34°/s  g_y: -0.56°/s  g_z: -0.89°/s
```