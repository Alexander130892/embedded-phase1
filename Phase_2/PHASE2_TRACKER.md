# 🛠️ Embedded Systems — Phase 2 Study Tracker
**Protocols + RTOS + Linux Basics | Months 4–7 | ~10 hrs/week**

> **Goal:** Handle multi-peripheral systems and understand concurrency; get a first taste of embedded Linux.
> **Strategy:** SPI/I2C before FreeRTOS — peripheral drivers feed naturally into the RTOS project. Keep Linux light here (Raspberry Pi only); Yocto moves to Phase 3.

## 📅 Weekly Schedule

---

### 🟠 BLOCK 1 — Serial Protocols at Register Level (Weeks 13–17)

> No HAL. No CubeMX. Direct register access only — same discipline as Phase 1.

---

#### Week 13 — SPI Fundamentals + First Peripheral
> ~10 hrs | R3 Ch.9 (SPI section) + target sensor datasheet

**Study tasks**
- [x] R3 Ch.9 — SPI protocol: clock polarity (CPOL), clock phase (CPHA), master/slave framing
- [x] Reference Manual: SPI section — SPIx_CR1, CR2, SR, DR register descriptions
- [x] Understand NSS management: software vs hardware — know why software NSS is safer for bare-metal
- [x] Study SPI clock speed constraints: PCLK prescaler, how to calculate actual baud rate
- [x] Pick a target sensor: **MPU-6050 (IMU) via SPI** or substitute with any SPI peripheral you have on hand

**Exercises**
- [x] Exercise: configure SPI1 bare-metal — enable RCC, set GPIO to AF mode (MOSI/MISO/SCK/NSS), write CR1 directly
- [x] Exercise: implement `spi_send_byte()` and `spi_receive_byte()` — poll on TXE/RXNE flags in SR
- [x] Exercise: implement `spi_transfer()` for full-duplex — send + receive simultaneously
- [x] Exercise: read the WHO_AM_I register from your sensor to verify comms are working
- [x] Verify with oscilloscope or logic analyser if available (even a cheap 8-channel clone is enough)
- [x] Commit to `week13/spi_basic/`

> 🏁 **M7 checkpoint:** SPI TX/RX verified; sensor responds to register read.

---

#### Week 14 — SPI: DMA + Multi-byte Transfers
> ~10 hrs | R3 Ch.9 (DMA integration) + Reference Manual DMA section

- [x] R3 Ch.9 — SPI DMA mode: understand why DMA matters for high-throughput sensor reads
- [x] Reference Manual: DMA chapter — channel mapping, CCR, CNDTR, CPAR, CMAR registers
- [x] Understand DMA stream/channel pairing for your specific SPI peripheral
- [x] Study DMA transfer complete and error interrupt flags — how to clear them
- [x] Exercise: configure DMA for SPI1 RX — transfer N bytes into a buffer without CPU polling
- [x] Exercise: implement `spi_read_registers()` — burst read multiple registers in one CS assertion
- [x] Exercise: initialise SH1107 OLED over SPI — send init command sequence via DMA TX
- [x] Exercise: implement `oled_write_buffer()` — DMA TX transfer of full 128x128 framebuffer
- [x] Exercise: read BMP280 temperature + pressure continuously and render values on OLED
- [x] Parse raw BMP280 register values into meaningful units (apply compensation formulas from datasheet)
- [x] Commit to `week14/spi_dma/`

---

#### Week 15 — I2C Fundamentals + Sensor Driver
> ~10 hrs | R3 Ch.9 (I2C section) + sensor datasheet

- [x] R3 Ch.9 — I2C protocol: START/STOP conditions, addressing (7-bit vs 10-bit), ACK/NACK
- [x] Reference Manual: I2C section — CR1, CR2, OAR1, DR, SR1, SR2 register descriptions
- [x] Understand I2C error conditions: bus busy (BUSY flag), arbitration loss (ARLO), ACK failure (AF) — know how to recover
- [x] Study clock stretching and why it matters for slow peripherals
- [x] Study the I2C timing configuration for your MCU speed (standard 100 kHz vs fast 400 kHz)
- [x] Pick a target I2C sensor: **BMP280 (pressure/temp)**, BME280, or any I2C device you have
- [x] Exercise: configure I2C1 bare-metal — enable RCC, set GPIO to open-drain AF mode with pull-ups
- [x] Exercise: implement `i2c_write_byte()` and `i2c_read_byte()` — poll on SR1/SR2 flags
- [x] Exercise: implement `i2c_write_register()` and `i2c_read_register()` convenience wrappers
- [x] Exercise: read WHO_AM_I / chip ID from sensor to verify comms
- [x] Commit to `week15/i2c_basic/`

> 🏁 **M8 checkpoint:** I2C sensor responding; raw readings available.

---

#### Week 16 — I2C: Multi-byte Reads + Error Handling
> ~10 hrs | Reference Manual I2C section + sensor datasheet

- [x] Implement burst read sequence (repeated START for multi-byte I2C reads — critical for IMUs)
- [x] Understand why the "two-byte read" sequence in STM32 I2C is non-obvious (refer to RM errata / application notes)
- [x] Implement robust error recovery: detect BUSY/ARLO/AF flags and attempt bus reset
- [x] Exercise: read temperature + pressure from BMP280 with compensation formula from datasheet
- [x] Exercise: build a minimal `bmp280.c` / `bmp280.h` driver pair — clean API, no register addresses leaked outside the driver
- [x] Study driver layering: HAL → transport layer → device driver. Model your own code on this pattern even without HAL.
- [x] Extra: Do the same for MPU6050
- [x] Commit to `week16/i2c_driver/`

---

#### Week 17 — Protocol Integration + Mini Data Logger
> ~10 hrs | Original code + R7 (Barr Group coding standard — review)

- [x] Wire up at least **two sensors** — one SPI, one I2C — simultaneously
- [x] Exercise: read from both sensors in a polling loop; multiplex over UART to host terminal
- [x] Exercise: implement a simple CSV output format: `timestamp_ms, ax, ay, az, gx, gy, gz, temp, pressure`
- [x] Exercise: add `log start` / `log stop` UART commands from your Phase 1 command parser
- [x] Add proper `volatile` usage for any shared buffers touched by ISRs
- [x] Review Barr Group standard sections on header files, assertions, and data encapsulation — apply to your drivers
- [x] Refactor into clean modules: `spi.c`, `i2c.c`, `mpu6050.c`, `bmp280.c`, `uart.c`, `main.c`
- [x] Write a Makefile that builds the whole project cleanly from scratch
- [x] Commit to `week17/sensor_logger/` — this becomes the seed project for the FreeRTOS port

> 🏁 **M9 checkpoint:** Multi-sensor polling logger working over UART. Codebase modular and Makefile-driven.

---

### 🟣 BLOCK 2 — FreeRTOS (Weeks 18–24)

> Introduce an RTOS incrementally. Port your working sensor code into tasks rather than rewriting from scratch.

---

#### Week 18 — RTOS Concepts + FreeRTOS Basics
> ~10 hrs | R1-RTOS (Shawn Hymel Ep. 1–4) + R2-RTOS Ch. 1–3

**Study tasks**
- [x] R1-RTOS Ep.1–3 — What is an RTOS? Tasks, scheduler, preemption vs cooperative
- [x] R2-RTOS Ch.1 — The FreeRTOS distribution: directory structure, configuration (`FreeRTOSConfig.h`), port selection
- [x] R2-RTOS Ch.3 — The FreeRTOS kernel: tick, task states (running, ready, blocked, suspended), `configTICK_RATE_HZ`
- [x] R2-RTOS Ch.4 — Task management: `xTaskCreate()`, `vTaskDelete()`, `vTaskDelay()`, `uxTaskPriorityGet()`
- [x] Understand stack sizing: why each task needs its own stack, how to estimate size, `uxTaskGetStackHighWaterMark()`
- [x] Understand `configMAX_PRIORITIES` and why you should keep priority levels few and intentional
- [x] Exercise: port FreeRTOS to your Nucleo board manually (no CubeMX) — copy the correct Cortex-M port files, write a minimal `FreeRTOSConfig.h`
- [x] Exercise: create two tasks — `vLEDTask` (blink) and `vUARTTask` (print "tick" every second) — verify both run concurrently
- [x] Commit to `week18/freertos_intro/`

---

#### Week 19 — Queues + Inter-task Communication
> ~10 hrs | R1-RTOS Ep. 5–7 + R2-RTOS Ch. 4

- [x] R1-RTOS Ep.5–6 — Queues: producer/consumer pattern, blocking sends and receives
- [x] R2-RTOS Ch.4 — Queue management: `xQueueCreate()`, `xQueueSend()`, `xQueueReceive()`, `xQueueSendFromISR()`
- [x] Understand queue length vs item size — know when to pass data vs pointers
- [x] Understand `xQueueSendFromISR()` and `portYIELD_FROM_ISR()` — critical for ISR → task handoff
- [x] Exercise: create a sensor task that reads from UART ISR via queue — replace polling RX with `xQueueReceiveFromISR`
- [x] Exercise: implement a `vSensorReadTask` that posts sensor readings to a queue, and a `vLogTask` that pulls from it and transmits over UART
- [x] Verify no data is lost under continuous logging at 115200 baud
- [x] Commit to `week19/freertos_queues/`

> 🏁 **M10 checkpoint:** Queue-based sensor → UART pipeline functional.

---

#### Week 20 — Semaphores + Mutexes
> ~10 hrs | R1-RTOS Ep. 8–10 + R2-RTOS Ch. 5–6

- [x] R1-RTOS Ep.8–9 — Binary semaphores: signalling between ISR and task (vs queues — know the tradeoff)
- [x] R2-RTOS Ch.5 — Semaphore management: binary, counting, `xSemaphoreGive()`, `xSemaphoreTake()`, ISR variants
- [x] R2-RTOS Ch.6 — Mutex: priority inheritance, why a mutex ≠ binary semaphore, deadlock conditions to avoid
- [x] Study priority inversion — understand the scenario and why FreeRTOS mutexes mitigate it
- [x] Exercise: replace timer ISR flag with binary semaphore — `xSemaphoreGiveFromISR()` in TIM2 ISR, `xSemaphoreTake()` in sensor task
- [x] Exercise: protect the shared UART TX function with a mutex — prevent garbled output when two tasks write simultaneously
- [x] Exercise: deliberately create a priority inversion scenario (optional) — observe and explain the outcome -- see video
- [x] Commit to `week20/freertos_sync/`

---

#### Week 21 — Software Timers + Task Notifications
> ~10 hrs | R1-RTOS Ep. 11–12 + R2-RTOS Ch. 7–8

- [x] R2-RTOS Ch.7 — Software timers: one-shot vs auto-reload, timer daemon task, `xTimerCreate()`, `xTimerStart()`
- [x] R2-RTOS Ch.8 — Task notifications: lighter than semaphores, direct-to-task signalling, `xTaskNotifyGive()` / `ulTaskNotifyTake()`
- [x] Understand when to use task notifications vs semaphores (rule of thumb: notifications when only one task waits)
- [x] Exercise: implement a watchdog-style heartbeat using a software timer — reset a flag every 5 s, assert if missed
- [x] Exercise: replace the binary semaphore in Week 20 with a task notification — compare code clarity
- [x] Exercise: implement a `log rate` UART command to change sensor polling interval at runtime via timer period update
- [x] Commit to `week21/freertos_timers/`

> 🏁 **M11 checkpoint:** Dynamic logging rate configurable over UART at runtime.

---

#### Week 22 — Memory Management + Stack Auditing
> ~10 hrs | R2-RTOS Ch. 2 (heap) + FreeRTOS source (heap_4.c)

- [x] R2-RTOS Ch.2 — Heap memory: heap_1 through heap_5, what each trades off, why heap_4 is the typical choice
- [x] Understand `configTOTAL_HEAP_SIZE` — how to estimate total heap needed across all tasks and queues
- [x] Study stack overflow detection: `configCHECK_FOR_STACK_OVERFLOW`, hook function, watermark polling
- [x] Implement `vApplicationStackOverflowHook()` — trap to a UART error message + spin loop for debugging
- [x] Exercise: audit all task stacks using `uxTaskGetStackHighWaterMark()` — print a report over UART on demand
- [x] Exercise: intentionally overflow a task stack (in a test branch) — confirm the hook fires
- [x] Exercise: implement a `mem` UART command — report free heap via `xPortGetFreeHeapSize()`
- [x] Commit to `week22/freertos_memory/`

---

#### Week 23 — FreeRTOS Integration: Multi-sensor Logger
> ~10 hrs | Original code — integration sprint

- [x] Port your Week 17 multi-sensor logger fully into FreeRTOS tasks:
  - [x] `vSPITask` — reads IMU at 100 Hz via SPI DMA complete semaphore
  - [x] `vI2CTask` — reads BMP280 at 1 Hz
  - [x] `vLogTask` — formats and transmits CSV over UART, rate-limited
  - [x] `vCmdTask` — receives UART commands; controls logging, queries status
- [x] All inter-task data passed via queues; shared resources protected by mutex
- [x] Software timer triggers periodic sensor reads; task notifications wake `vLogTask`
- [x] Test continuous operation for at least 30 minutes — verify no hangs, no UART corruption
- [x] Commit to `week23/rtos_sensor_logger/`

> 🏁 **M12 checkpoint:** Full multi-sensor FreeRTOS logger running stably.

---

#### Week 24 — FreeRTOS Deliverable + Documentation
> ~10 hrs | Polish, README, retrospective

- [x] Refactor into clean final module structure: `tasks/`, `drivers/`, `config/`, `main.c`
- [x] Write `README.md` covering:
  - [x] Project overview and feature list
  - [x] Hardware setup (board, sensors, wiring)
  - [x] FreeRTOS task diagram (ASCII or drawn): tasks, queues, semaphores, mutexes
  - [x] Build instructions (make, flash)
  - [x] Design decisions: queue depths, task priorities, heap allocator choice
  - [x] Lessons learned / what you'd do differently
- [x] Add ASCII task architecture diagram to README
- [x] Tag the repo: `git tag v1.0-phase2-rtos`
- [x] Write a short FreeRTOS retrospective in `NOTES.md`

> 🏁 **M13 checkpoint:** FreeRTOS deliverable repo published. ✅

---

### 🟢 BLOCK 3 — Embedded Linux on Raspberry Pi (Weeks 25–28)

> First exposure only. Goal is working mental model and hands-on taste — not mastery. Yocto and production Linux is Phase 3.

---

#### Week 25 — Linux Basics + Raspberry Pi Setup
> ~10 hrs | R3-Linux Ch. 1–3

**Study tasks**
- [ ] R3-Linux Ch.1 — The elements of embedded Linux: toolchain, bootloader, kernel, root filesystem — the four artefacts
- [ ] R3-Linux Ch.2 — Toolchains: native vs cross-compilation, sysroot, what `arm-linux-gnueabihf-gcc` is and why it differs from `arm-none-eabi-gcc`
- [ ] R3-Linux Ch.3 — All about bootloaders: U-Boot basics, boot sequence on Raspberry Pi (GPU → bootcode.bin → kernel)
- [ ] Understand the Pi's boot chain — contrast with STM32's vector table boot
- [ ] Set up Raspberry Pi OS (Lite, no desktop) on Pi 4 or Pi 3 — headless via SSH
- [ ] Verify cross-compiler on your host: compile a Hello World for ARM, SCP to Pi, run it
- [ ] Set up `sshfs` or `rsync` workflow for fast host → Pi file transfer
- [ ] Commit cross-compile Hello World to `week25/cross_hello/`

---

#### Week 26 — Linux Device Drivers Concepts + GPIO from Userspace
> ~10 hrs | R3-Linux Ch. 9 + kernel docs

- [ ] R3-Linux Ch.9 — Interfacing with device drivers: character devices, `/dev` nodes, `ioctl()`, `read()`/`write()`
- [ ] Understand sysfs vs devfs vs `/proc` — know which to use for what
- [ ] Study Linux GPIO interfaces: legacy sysfs (`/sys/class/gpio/`) vs modern `libgpiod` — use `libgpiod`
- [ ] Study `/dev/spidev` and `/dev/i2c-*` userspace access — understand when userspace drivers are acceptable
- [ ] Exercise: use `libgpiod` to blink an LED — `gpiod_get_value()`, `gpiod_set_value()`
- [ ] Exercise: use `/dev/i2c-1` + `ioctl(I2C_SLAVE)` to read BMP280 from Pi userspace in C
- [ ] Exercise: use `/dev/spidev0.0` to communicate with an SPI device from userspace in C
- [ ] Compare userspace driver approach to your bare-metal STM32 register code — reflect on tradeoffs
- [ ] Commit to `week26/linux_gpio_i2c_spi/`

> 🏁 **M14 checkpoint:** GPIO, I2C, and SPI all exercised from Linux userspace in C.

---

#### Week 27 — Device Tree Basics + Simple Kernel Module
> ~10 hrs | R3-Linux Ch. 4, Ch. 11

- [ ] R3-Linux Ch.4 — Configuring the kernel: `menuconfig`, kernel config options, what `CONFIG_` flags do
- [ ] R3-Linux Ch.4 — Device Tree: what it is, why it replaced board files, `.dts` / `.dtsi` / `.dtb` relationship
- [ ] Read the Pi's DT overlay system: understand `/boot/config.txt` overlay entries and what they expand to
- [ ] R3-Linux Ch.11 — Writing a device driver: `module_init()`, `module_exit()`, `printk()`, `insmod`/`rmmod`/`lsmod`
- [ ] Understand the difference between built-in and loadable modules (`.ko`)
- [ ] Exercise: write a minimal "Hello, kernel" loadable module — verify with `dmesg`
- [ ] Exercise: add a module parameter (`module_param()`) — pass a string at `insmod` time, print it with `printk()`
- [ ] Exercise: write a character device module with `open()`, `read()`, `write()` file operations — expose a counter via `/dev/mydev`
- [ ] Commit to `week27/kernel_module/`

---

#### Week 28 — Linux Integration + Phase 2 Deliverable
> ~10 hrs | Polish, cross-compile pipeline, documentation

- [ ] Cross-compile your kernel module on the host for Pi target — verify it loads on Pi without recompiling on device
- [ ] Exercise: build a small C application on the host that opens `/dev/mydev`, reads the counter, prints it — link the full pipeline
- [ ] Exercise: write a simple init script (systemd `.service` unit) that `insmod`s your module at boot and starts your userspace app
- [ ] Write `README.md` for the Linux block covering:
  - [ ] Cross-compile setup (host toolchain, target sysroot)
  - [ ] Module build and load instructions
  - [ ] Device Tree overlay used (if any)
  - [ ] Comparison table: bare-metal STM32 vs Linux userspace vs kernel driver — when to use each
- [ ] Tag the repo: `git tag v1.0-phase2-linux`
- [ ] Write a short Phase 2 retrospective in `NOTES.md`:
  - [ ] What clicked, what was harder than expected
  - [ ] How FreeRTOS changed how you think about sequencing work
  - [ ] First impressions of Linux kernel space vs userspace vs bare-metal

> 🏁 **M15 checkpoint:** Cross-compiled kernel module loading on Pi. Phase 2 complete. ✅

---

## 📊 Progress Summary

| Week | Topic | Status |
|------|-------|--------|
| 13 | SPI Fundamentals + Sensor | ✅ Done |
| 14 | SPI DMA + Multi-byte Transfers | ✅ Done |
| 15 | I2C Fundamentals + Sensor Driver | ✅ Done |
| 16 | I2C Multi-byte Reads + Error Handling | ✅ Done|
| 17 | Protocol Integration + Mini Data Logger | ✅ Done |
| 18 | RTOS Concepts + FreeRTOS Basics | ✅ Done |
| 19 | Queues + Inter-task Communication | ✅ Done |
| 20 | Semaphores + Mutexes | ✅ Done |
| 21 | Software Timers + Task Notifications | ✅ Done |
| 22 | Memory Management + Stack Auditing | ✅ Done |
| 23 | FreeRTOS Integration: Multi-sensor Logger | 🟡 In progress |
| 24 | FreeRTOS Deliverable + Docs | 🟡 In progress |
| 25 | Linux Basics + Raspberry Pi Setup | 🔲 Not started |
| 26 | Linux GPIO/I2C/SPI from Userspace | 🔲 Not started |
| 27 | Device Tree + Kernel Module | 🔲 Not started |
| 28 | Linux Integration + Phase 2 Deliverable | 🔲 Not started |

> Update statuses to: `🟡 In progress` → `✅ Done`

---

## 📦 Resources

| # | Resource | Format | Link |
|---|----------|--------|------|
| R1-RTOS | Introduction to RTOS — Shawn Hymel / DigiKey | YouTube (free) | [DigiKey YouTube playlist](https://www.youtube.com/playlist?list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz) |
| R2-RTOS | Mastering the FreeRTOS Real Time Kernel | Book/PDF (free) | [FreeRTOS.org](https://www.freertos.org/Documentation/RTOS_book.html) |
| R3-Linux | Mastering Embedded Linux Programming — Chris Simmonds | Book | 2nd ed. — covers RPi → Yocto pipeline |
| R4 | STM32 Reference Manual (RM0090 or your variant) | PDF | [st.com](https://www.st.com/resource/en/reference_manual/rm0090-stm32f405415-stm32f407417-stm32f427437-and-stm32f429439-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) |
| R5 | FreeRTOS Source + Port Files | Code | [github.com/FreeRTOS/FreeRTOS-Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel) |
| R6 | Sensor Datasheets | PDF | MPU-6050, BMP280/BME280 — practice reading them directly |
| R7 | Raspberry Pi Linux kernel docs | Web | [kernel.org/doc](https://www.kernel.org/doc/html/latest/) |
| R8 | libgpiod documentation | Web | [git.kernel.org/pub/scm/libs/libgpiod](https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/) |

---

## 🏁 Milestone Overview

| Milestone | Target Week | Description |
|-----------|-------------|-------------|
| M7 | End of Week 13 | SPI TX/RX verified; sensor responds to register read |
| M8 | End of Week 15 | I2C sensor responding; raw readings available |
| M9 | End of Week 17 | Multi-sensor polling logger working over UART; modular codebase |
| M10 | End of Week 19 | Queue-based sensor → UART FreeRTOS pipeline functional |
| M11 | End of Week 21 | Dynamic logging rate configurable over UART at runtime |
| M12 | End of Week 23 | Full multi-sensor FreeRTOS logger running stably for 30+ min |
| **M13** | **End of Week 24** | **FreeRTOS deliverable repo published** ✅ |
| M14 | End of Week 26 | GPIO, I2C, SPI exercised from Linux userspace in C |
| **M15** | **End of Week 28** | **Cross-compiled kernel module loading on Pi. Phase 2 complete.** ✅ |

---

## 🗂️ Suggested Repo Structure

```
embedded-phase2/
├── week13/spi_basic/
├── week14/spi_dma/
├── week15/i2c_basic/
├── week16/i2c_driver/
├── week17/sensor_logger/
├── week18/freertos_intro/
├── week19/freertos_queues/
├── week20/freertos_sync/
├── week21/freertos_timers/
├── week22/freertos_memory/
├── week23/rtos_sensor_logger/        ← FreeRTOS deliverable seed
├── week24/                           ← README, NOTES, tag v1.0-phase2-rtos
├── week25/cross_hello/
├── week26/linux_gpio_i2c_spi/
├── week27/kernel_module/
├── week28/                           ← README, NOTES, tag v1.0-phase2-linux
└── README.md
```

---

## 📝 Notes & Decisions Log

> Use this section to log design decisions, bugs worth remembering, and things to revisit in Phase 3.

| Date | Note |
|------|------|
| | |