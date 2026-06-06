# Week 24 — FreeRTOS Multi-Sensor Logger: Final Deliverable

## Goal

Polish and publish the FreeRTOS multi-sensor logger built during Weeks 18–23. The refactor
restructures the Week 23 codebase into a clean, modular directory layout, introduces a
dedicated `kernel_objects` module for shared RTOS state, pushes register-level operations
into the driver layer, and verifies the full toolchain (compile → flash → serial output)
independently of STM32CubeIDE using a hand-written Makefile.

**M13 checkpoint:** FreeRTOS deliverable repo published. Full toolchain verified from VS Code. ✅

---

## Hardware

| Component | Details |
|-----------|---------|
| MCU | STM32F446RE Nucleo-64 (ARM Cortex-M4, 16 MHz internal clock) |
| IMU | MPU-6050 — I2C1, PB8 (SCL) / PB9 (SDA), address 0x68 |
| Pressure/Temp | BMP280 — SPI1, PA5/PA6/PA7 (SCK/MISO/MOSI), CS on PB6 |
| UART | USART2 — PA2 (TX) / PA3 (RX), 115200 baud, ST-Link virtual COM |
| Programmer | ST-Link v2 (on-board) via `st-flash` |

---

## Project Structure

```
freertos_sensor_logger/
├── main.c
├── Inc/
│   ├── FreeRTOSConfig.h
│   ├── drivers/
│   │   ├── i2c.h / spi.h / uart.h / timer.h / status.h
│   │   ├── mpu6050/  mpu6050.h  mpu6050_internal.h
│   │   └── bmp280/   bmp280.h   bmp280_internal.h
│   ├── tasks/        task_i2c.h  task_spi.h  task_log.h  task_cmd.h
│   ├── app/          watchdog.h  hooks.h
│   └── rtos/         kernel_objects.h
├── Src/              (mirrors Inc/ structure)
├── Startup/          startup_stm32f446retx.s
├── third_party/
│   ├── FreeRTOS/     Include/  Source/  Port/
│   └── CMSIS/
└── Makefile
```

`Inc/` and `Src/` mirror each other exactly. `third_party/` holds all vendor files
(FreeRTOS kernel, CMSIS, STM32 headers) — untouched, never modified.

---

## Architecture

```
                    TIM2 IRQ (100 Hz)
                         │
                         │ vTaskNotifyGiveFromISR
                         ▼
                   ┌─────────────┐
                   │  vI2CTask   │  reads MPU-6050 via I2C
                   │  (100 Hz)   │  sets b_timeout_mpu6050 = false on good read
                   └──────┬──────┘
                          │ xQueueSend → xQueue_mpu6050 (depth 5)
                          │
    SW Timer (1 Hz)       │
    vBMP280Timer          │
    xTaskNotifyGive       │
         │                │
         ▼                │
   ┌─────────────┐        │
   │  vSPITask   │  reads BMP280 via SPI
   │   (1 Hz)    │  sets b_timeout_bmp280 = false on good read
   └──────┬──────┘
          │ xQueueSend → xQueue_bmp280 (depth 5)
          │                │
          └────────────────┼──────────────────┐
                           ▼                  │
                    ┌─────────────┐           │
                    │  vLogTask   │◄──────────┘
                    │             │
                    │  dequeues both sensors
                    │  formats CSV over UART
                    │  checks watchdog flags
                    └──────┬──────┘
                           │ xSemaphoreTake/Give → xMutexUart
                           ▼
                        UART TX

    SW Timer (5 s)
    vHeartBeatTimer  [in task_log.c]
         │
         ├─ if b_timeout_mpu6050 still true → xTaskNotifyGive(xLogTaskHandle)
         ├─ else → set b_timeout_mpu6050 = true
         ├─ if b_timeout_bmp280  still true → xTaskNotifyGive(xLogTaskHandle)
         └─ else → set b_timeout_bmp280  = true

    USART2 IRQ
         │ xQueueSendFromISR → xQueueUARTRx (depth 20)
         ▼
   ┌─────────────┐
   │  vCmdTask   │  commands: log on/off · hwm · mem
   └─────────────┘

    Shared state (app/watchdog.c):
      volatile bool b_timeout_mpu6050
      volatile bool b_timeout_bmp280

    Kernel objects (rtos/kernel_objects.c):
      xQueue_mpu6050 · xQueue_bmp280 · xQueueUARTRx
      xMutexUart
      xTimerBMP280 · xTimerHeartBeat
      xI2CTaskHandle · xSPITaskHandle · xLogTaskHandle · xCmdTaskHandle
      pxTaskStatusArray[N_TASKS]
```

---

## Task Summary

| Task | Stack | Priority | Wake-up mechanism |
|------|-------|----------|-------------------|
| vI2CTask | 128 words | 1 | `ulTaskNotifyTake` — TIM2 IRQ @ 100 Hz |
| vSPITask | 128 words | 1 | `ulTaskNotifyTake` — SW timer @ 1 Hz |
| vLogTask | 512 words | 1 | `xQueueReceive` (50 ms timeout) + notify |
| vCmdTask | 256 words | 1 | `xQueueReceive` (100 ms timeout) |

All tasks run at equal priority. The FreeRTOS timer daemon and idle task use default stack sizes
configured in `FreeRTOSConfig.h`. Heap: `heap_4`, 25 KB. Peak heap usage ~7 KB.

---

## Build Instructions

### Prerequisites

```bash
brew install arm-none-eabi-gcc stlink
```

### Build & Flash

```bash
make build     # compile → main.elf + main.hex
make flash     # st-flash write main.bin 0x08000000
make clean     # remove all build artefacts
make size      # print section sizes
make disasm    # open disassembly in less
```

### Serial Monitor

```bash
screen /dev/tty.usbmodem* 115200
# Exit: Ctrl+A then Ctrl+\
```

### Commands (over serial)

| Command | Effect |
|---------|--------|
| `log on` | Start sensor acquisition and CSV output |
| `log off` | Stop timers and TIM2 |
| `hwm` | Print per-task stack high-water marks |
| `mem` | Print current and minimum-ever free heap |

---

## Key Design Decisions

**`kernel_objects.h` / `kernel_objects.c` for shared RTOS state.**
All queue handles, mutex, timer handles, task handles, and `pxTaskStatusArray` are defined in
a single translation unit (`kernel_objects.c`) and declared `extern` in `kernel_objects.h`.
Any module that needs a handle includes one header. This decouples shared state from `main.c`
— which is a startup file, not an ownership boundary — and makes the handle set portable
across future projects.

**`third_party/` for all vendor files.**
FreeRTOS kernel sources, port files, and CMSIS headers are never modified and live entirely
under `third_party/`. Application code never reaches into that directory except via include
paths. A FreeRTOS version bump touches exactly one directory.

**Timer callbacks are minimal shims.**
`vBMP280Timer` and `vHeartBeatTimer` execute in the timer daemon task context and must not
block. Both callbacks are therefore one-liners: `xTaskNotifyGive` only. All real work happens
in the notified task. This was a hard lesson from Week 23 — blocking inside a callback starves
all other software timers.

**Hardware timer for MPU-6050, software timer for BMP280.**
TIM2 fires at exactly 100 Hz with no scheduler jitter — required for IMU data integrity.
BMP280 runs at 1 Hz where a few milliseconds of software timer jitter is irrelevant. Using
a software timer for BMP280 avoids consuming a second hardware timer peripheral.

**Two-flag watchdog pattern.**
`volatile bool b_timeout_mpu6050 / b_timeout_bmp280` live in `app/watchdog.c` — neutral
shared state written by three tasks (`vCmdTask` on `log on`, sensor tasks on good reads) and
read by `vLogTask`. `volatile` is sufficient on Cortex-M4: ARM guarantees atomic aligned
byte writes, and these flags are never read-modify-written by the sensor tasks.

**`timer2_enable()` / `timer2_disable()` in the driver layer.**
TIM2 CEN bit manipulation was originally scattered across task files as raw register writes.
Moved into `drivers/timer.c` as `timer2_enable()` and `timer2_disable()`. Task code now
calls a named function rather than touching hardware registers directly, keeping the driver
boundary clean.

**Stack sizing after a 15-minute stability run.**
Stack sizes are set from HWM readings taken after sustained operation, not at startup.
HWM values only move in one direction — they represent true worst-case depth over the full
run. Final sizes include 25–90% headroom above measured HWM. `vLogTask` required 512 words
due to `snprintf` formatting machinery and FreeRTOS queue receive overhead at `-O0`.
`vCmdTask` required 256 words after local string buffers were moved to `static` storage.

---

## Lessons Learned

- **`main.c` is a startup file, not a variable owner.** Keeping queue and task handles as
  globals in `main.c` and using `extern` against it couples every module to the internals of
  your init code. A dedicated `kernel_objects` module breaks that coupling cleanly.

- **`extern` declarations belong in the header, definitions in the `.c` file.** The header
  advertises what exists; the `.c` allocates storage. Every consumer includes one header and
  gets both the FreeRTOS type definitions and the `extern` declarations in one step.

- **Explicit includes over transitive ones.** Best practice is for every `.c` file to include
  what it directly uses, regardless of what comes in transitively. Transitive reliance breaks
  silently when a header is refactored.

- **Timer callbacks are not tasks.** They run in the daemon task — blocking inside one starves
  all other software timers. Every callback must be a minimal signal: `xTaskNotifyGive` or
  flag write only. The real work belongs in the woken task.

  - **`volatile` qualifier must match between declaration and definition.** Declaring
  `extern volatile bool` in the header and defining `bool` in the `.c` file produces a
  `conflicting type qualifiers` error. Declaration and definition must be identical.

- **Stack HWM should be read at the end of a stability run, not the beginning.** Values only
  move in one direction. A reading at startup reflects only the initial call depth, not
  worst-case behaviour under sustained load.

- **`-O0` inflates stack usage significantly.** With no optimization, every local variable
  gets its own stack slot and temporaries are never reused. `snprintf` in particular is heavy.
  Size stacks from HWM readings at the actual optimization level used in the build, not from
  intuition.

- **`static` local buffers move data off the task stack.** Large local arrays like
  `command[64]` consume stack on every function entry. Declaring them `static` moves them to
  `.bss` / `.data`, at the cost of losing re-entrancy — acceptable for single-instance tasks.

- **`TIM2_IRQHandler` in the header is harmless but unnecessary.** The hardware jumps to the
  handler via the vector table directly. The linker's weak/strong resolution handles the
  override — no other translation unit needs to call the IRQ handler by name.

---

## Open Items

- **BMP280 `WHO_AM_I` liveness check** — replace the temperature sanity range check with
  periodic readback of chip ID register (`0x58`). Use `xTaskNotifyWait` with a value to
  distinguish "read" vs "verify" notifications in `vSPITask`.

- **Run-time stats** — `configGENERATE_RUN_TIME_STATS` requires a dedicated high-resolution
  free-running counter (not TIM2, which stops on `log off`). Add TIM3 at ~10× tick rate.

- **I2C event-driven driver** — replace spin-loop waits with interrupt + semaphore to
  eliminate the blocking window entirely without compromising driver portability.

- **`-Wunused-parameter` cleanup** — add `(void)pvParameters` casts to task functions and
  `(void)xTimer` casts to timer callbacks to suppress `-Wextra` warnings cleanly.